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
  };

  for(string particle : particle_names){
      histSets[particle] = new HistogramSet(particle);
  }
  
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
