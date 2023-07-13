//
//  HistogramSet.cpp
//  selections
//
//  Created by Jeremi Niedziela on 24/02/2023.
//

#include "HistogramSet.hpp"
#include <iostream>
#include <random>

using namespace std;

HistogramSet::HistogramSet(string prefix)
{
  hists["pt"]       = new TH1D((prefix+"_pt").c_str(),        (prefix+"_pt").c_str(),           1000, 0,      200   );
  hists["mass_log"] = new TH1D((prefix+"_mass_log").c_str(),  (prefix+"_mass_log").c_str(),     1000,  logxBins(1000,0.1,100));
  hists["mass"]     = new TH1D((prefix+"_mass").c_str(),      (prefix+"_mass").c_str(),         1500,  0,     15   );
  hists["eta"]      = new TH1D((prefix+"_eta").c_str(),       (prefix+"_eta").c_str(),          1000, -5,     5     );
  hists["phi"]      = new TH1D((prefix+"_phi").c_str(),       (prefix+"_phi").c_str(),          1000, -5,     5     );
}

void HistogramSet::fill(const Particle *particle, const Event *event)
{
  double weight = 1;
  if(event) weight = event->weight;

  hists["pt"]->Fill(particle->four_vector.Pt(), weight);
  hists["mass"]->Fill(particle->four_vector.M(), weight);
  hists["mass_log"]->Fill(particle->four_vector.M(), weight);
  hists["eta"]->Fill(particle->four_vector.Eta(), weight);
  hists["phi"]->Fill(particle->four_vector.Phi(), weight);
}


void HistogramSet::fill(const Particle* particle_1, const Particle* particle_2, const Event *event)
{
  double weight = 1;
  if(event) weight = event->weight;
      
  TLorentzVector diparticle = particle_1->four_vector + particle_2->four_vector;
  hists["pt"]->Fill(diparticle.Pt(), weight);
  hists["mass"]->Fill(diparticle.M(), weight);
  hists["mass_log"]->Fill(diparticle.M(), weight);
  hists["eta"]->Fill(diparticle.Eta(), weight);
  hists["phi"]->Fill(diparticle.Phi(), weight);
}


float* HistogramSet::logxBins(const int n_bins, const float min, const float max)
{
  float* binList = new float[n_bins+1];
  for (int i=0; i<n_bins+1; i++) {
    binList[i] = (pow(10,log10(min)+((log10(max)-log10(min))/n_bins)*i));
  }
  return binList;
}
