#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

#include "Particle.hpp"
#include "Event.hpp"
#include "EventReader.hpp"
#include "HistogramSet.hpp"
#include "Helpers.hpp"
#include "HistogramFiller.hpp"

using namespace std;

int max_events = -1;
int n_daughters = 100;

TFile *input_file;
TH1D *baseline_hist;

TTree* get_input_tree(string input_path)
{
  input_file = TFile::Open(input_path.c_str());
  
  if(!input_file){
    cout<<"ERROR -- could not open input file: "<<input_path<<endl;
    exit(0);
  }
  
  return (TTree*)input_file->Get("Events");
}

TH1D* fill_and_save_histograms(const vector<Event*> &events, string output_path, bool reduce_hists=false, bool include_alp_ancestor_hists=false)
{
  bool displaced_mass_cuts = true;
  bool include_lxy_selection = true;
  auto histogramFiller = HistogramFiller(reduce_hists, include_alp_ancestor_hists, displaced_mass_cuts);
  for(auto event : events){
    if(!event->has_ttbar_pair()) continue;
    int preselection_code = event->passes_preselection(include_lxy_selection);
    
    if(preselection_code == 0){
      continue;
    }
    else if(preselection_code == 2 || preselection_code == 3){ // pair or non-pair category
      auto [muon_1, muon_2] = event->get_smallest_deltaLxyRatio_opposite_sign_muons();
      
      histogramFiller.fill_hists(muon_1, muon_2, event, "os");
      histogramFiller.fill_final_selection_hists(muon_1, muon_2, event, "os");

      if(reduce_hists) continue;
      if(!muon_1 || !muon_2) continue;
      // Selection of "first" and "last" muons and muon mothers
      auto first_muon_1 = muon_1;
      auto first_muon_2 = muon_2;
      while (abs(event->particles[first_muon_1->mothers[0]]->pdgid) == 13) {
        first_muon_1 = event->particles[first_muon_1->mothers[0]];
      }
      while (abs(event->particles[first_muon_2->mothers[0]]->pdgid) == 13) {
        first_muon_2 = event->particles[first_muon_2->mothers[0]];
      }
      histogramFiller.fill_hists(first_muon_1, first_muon_2, event, "os_first");
      histogramFiller.fill_2d_hists(muon_1, first_muon_1, "os_first");
      histogramFiller.fill_final_selection_hists(first_muon_1, first_muon_2, event, "os_first");
      auto last_mother_1 = event->particles[first_muon_1->mothers[0]];
      auto first_mother_1 = event->particles[first_muon_1->mothers[0]];
      auto last_mother_2 = event->particles[first_muon_2->mothers[0]];
      auto first_mother_2 = event->particles[first_muon_2->mothers[0]];
      while (abs(event->particles[first_mother_1->mothers[0]]->pdgid) == first_mother_1->pdgid) {
        first_mother_1 = event->particles[first_mother_1->mothers[0]];
      }
      while (abs(event->particles[first_mother_2->mothers[0]]->pdgid) == first_mother_2->pdgid) {
        first_mother_2 = event->particles[first_mother_2->mothers[0]];
      }
      histogramFiller.fill_hists(first_mother_1, first_mother_2, event, "os_first_mother");
      histogramFiller.fill_hists(last_mother_1, last_mother_2, event, "os_last_mother");
      histogramFiller.fill_2d_hists(last_mother_1, first_mother_1, "os_first_mother");

      // Require the muons to have an alp ancestor
      if(!include_alp_ancestor_hists) continue;
      if(!muon_1->has_alp_ancestor(event->particles)) continue;
      if(!muon_2->has_alp_ancestor(event->particles)) continue;
      if(last_mother_1 != last_mother_2) continue;
      histogramFiller.fill_alp_in_preselection_hists(muon_1, muon_2, last_mother_1, event, "os");

      TVector3 boost = last_mother_1->get_boost();
      auto muon_reboosted_1 = new Particle(boost, muon_1);
      auto muon_reboosted_2 = new Particle(boost, muon_2);
      auto mother_reboosted_1 = new Particle(boost, last_mother_1);
      histogramFiller.fill_alp_in_preselection_hists(muon_reboosted_1, muon_reboosted_2, mother_reboosted_1, event, "reboosted_os");
    }
    
    for(auto muon : event->particles){
      if(!muon->is_muon()) continue;;
      if(!muon->has_alp_ancestor(event->particles)) continue;
      histogramFiller.fill_first_muon_from_alp_selection_hists(muon, event);
      break;
    }
  }
  auto baseline_hist = new TH1D(*histogramFiller.histSets["alp_selection_os_minlxy-muon"]->hists["proper_ctau"]);
  
  histogramFiller.save_histograms(output_path);
  
  return baseline_hist;
}

TH1D* get_weights_histogram(TH1D* baseline_hist, double destination_lifetime)
{
  double fit_max = 0.1;
  
  auto scaled_lifetime_function = new TF1("scaled_lifetime_function", "[0]*exp(-x/[1])", 0, fit_max);
  scaled_lifetime_function->SetParameter(0, 1);
  scaled_lifetime_function->FixParameter(1, destination_lifetime);
  
  baseline_hist->Fit(scaled_lifetime_function, "", "", 0, fit_max);
  
  auto hist_tmp= new TH1D("hist_scaled", "hist_scaled",
                           baseline_hist->GetNbinsX(),
                           baseline_hist->GetXaxis()->GetBinLowEdge(1),
                           baseline_hist->GetXaxis()->GetBinLowEdge(baseline_hist->GetNbinsX())+baseline_hist->GetXaxis()->GetBinWidth(baseline_hist->GetNbinsX()));
  
  auto weights = new TH1D("weights", "weights",
                          baseline_hist->GetNbinsX(),
                          baseline_hist->GetXaxis()->GetBinLowEdge(1),
                          baseline_hist->GetXaxis()->GetBinLowEdge(baseline_hist->GetNbinsX())+baseline_hist->GetXaxis()->GetBinWidth(baseline_hist->GetNbinsX()));
  
  for(int i_bin=1; i_bin<baseline_hist->GetNbinsX()+1; i_bin++){
    double value = baseline_hist->GetXaxis()->GetBinCenter(i_bin);
    double content = baseline_hist->GetBinContent(i_bin);
    double weight = 0;
    if(content > 0) weight = scaled_lifetime_function->Eval(value)/content;
    
    hist_tmp->SetBinContent(i_bin, content*weight);
    weights->SetBinContent(i_bin, weight);
  }
  double scale = baseline_hist->Integral()/hist_tmp->Integral();
  weights->Scale(scale);
  
  return weights;
}

int main(int argc, char *argv[])
{
  if(argc != 4){
    cout<<"Usage: ./prepare_histograms input_path output_path ctau_option"<<endl;
    exit(0);
  }
  
  string input_path = argv[1];
  string output_path = argv[2];
  double ctau_option = atof(argv[3]);
  
  auto input_tree = get_input_tree(input_path);
  
// load events
  auto event_reader = EventReader(max_events, n_daughters);
  auto events = event_reader.read_events(input_tree);

// Settings for including hists
  bool reweighting_on = false;
  bool include_alp_ancestor_hists = false;
  bool reduce_hists = false;
  if(ctau_option != 0.0) reduce_hists = true;
  
  TH1D *baseline_hist = fill_and_save_histograms(events, output_path, reduce_hists, include_alp_ancestor_hists);

  if(reweighting_on && ctau_option != 0.0)
  {
    double lifetime = ctau_option;
    vector<double> destination_lifetimes = {lifetime, 5*lifetime/10, lifetime/10}; // mm
    std::cout << "reweigthing lifetime " << lifetime << " to ";
    
    for(double destination_lifetime : destination_lifetimes){
      
      TH1D *weights_hist = get_weights_histogram(baseline_hist, destination_lifetime);
      
      // set new event weights
      for(auto event : events){
        double event_proper_lifetime = event->get_proper_lifetime();
        int weight_bin = weights_hist->GetXaxis()->FindFixBin(event_proper_lifetime);
        double weight = weights_hist->GetBinContent(weight_bin);
        if(weight < 0){
          cout<<"Found event for which weight cannot be calculated..."<<endl;
          weight = 1;
        }
        event->weight = weight;
      }
      
      string lifetime_output_path = replace_all(output_path, ".root", "_ctau-"+to_nice_string(destination_lifetime, 6)+"mm.root");
      fill_and_save_histograms(events, lifetime_output_path);
    }
  }
  
  input_file->Close();
  return 0;
}