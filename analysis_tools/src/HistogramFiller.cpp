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
    "triple_meson"
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
    int meson_ids[] = {311, 321, 111, 211};
    if( particle1->has_mother_with_id(511) && particle2->has_mother_with_id(511) && particle3->has_mother_with_id(511) &&
    contains(meson_ids, abs(particle1->pdgid)) && contains(meson_ids, abs(particle2->pdgid)) && contains(meson_ids, abs(particle3->pdgid)) )
    {
      histSets["triple_meson"]->fill(particle1, particle2, particle3);
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
