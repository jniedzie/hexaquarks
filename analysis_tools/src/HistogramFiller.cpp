//
//  HistogramFiller.cpp
//  selections
//
//  Created by Jeremi Niedziela on 06/04/2023.
//

#include "HistogramFiller.hpp"
#include "Helpers.hpp"

using namespace std;

HistogramFiller::HistogramFiller()
{
  vector<string> particle_names = {
    "single_particles",
    "single_pions",
    "single_kaons",
    "double_pions",
    "double_pions_from_rho_decay",
    "leading_muons",
    "leading_jets",
    "pi+pi-jpsi",
    "pi+pi-jpsi_from_lclc",
    "pi+pi-k+",
    "pi+pi-k+_from_pl"
  };

  for(string particle : particle_names) histSets[particle] = new HistogramSet(particle);
}

HistogramFiller::~HistogramFiller()
{
  for(auto &[setName, histSet] : histSets){
    for(auto &[histName, hist] : histSet->hists) delete hist;
  }
}

void HistogramFiller::fill_hists(const Particle* particle)
{
  if(!particle) return;
  histSets["single_particles"]->fill(particle);

  if(particle->is_pion()) histSets["single_pions"]->fill(particle);
  if(particle->is_kaon()) histSets["single_kaons"]->fill(particle);
  if(particle->index == particle->event->leading_muon_index) histSets["leading_muons"]->fill(particle);
  if(particle->index == particle->event->leading_jet_index) histSets["leading_jets"]->fill(particle);
}

void HistogramFiller::fill_hists(const Particle* particle1, const Particle* particle2)
{
  if(!particle1 || !particle2) return;
  // pi+ and pi-
if(particle1->is_pion() && particle2->is_pion() && 
   particle1->pdgid == -1*particle2->pdgid && 
   particle1->is_final() && particle2->is_final())
{
     histSets["double_pions"]->fill(particle1, particle2);
     
     if(particle1->has_mother_with_id(113) && particle2->has_mother_with_id(113)){
          histSets["double_pions_from_rho_decay"]->fill(particle1, particle2);
     }
}
}

void HistogramFiller::fill_hists(const Particle* particle1, const Particle* particle2, const Particle* particle3)
{
  if(!particle1 || !particle2 || !particle3) return;
  if(particle1->pdgid == 443 && particle2->pdgid == 211 && particle3->pdgid == -211)
  {
    histSets["pi+pi-jpsi"]->fill(particle1, particle2, particle3);
    if(particle1->has_mother_with_id(511) && particle2->has_mother_with_id(511) && particle3->has_mother_with_id(511))
    {
      histSets["pi+pi-jpsi_from_lclc"]->fill(particle1, particle2, particle3);
    }
  }

  if(particle1->pdgid == 321 && particle2->pdgid == 211 && particle3->pdgid == -211)
  {
    histSets["pi+pi-k+"]->fill(particle1, particle2, particle3);
    if(particle1->has_mother_with_id(521) && particle2->has_mother_with_id(521) && particle3->has_mother_with_id(521))
    {
      histSets["pi+pi-k+_from_pl"]->fill(particle1, particle2, particle3);
    }
  }

}


void HistogramFiller::save_histograms(std::string output_path)
{
  auto output_file = new TFile(output_path.c_str(), "recreate");
  output_file->cd();
  
  for(auto &[hist_name, hist_set] : histSets){
    for(auto &[tmp, hist] : hist_set->hists) hist->Write();
  }

  output_file->Close();
}
