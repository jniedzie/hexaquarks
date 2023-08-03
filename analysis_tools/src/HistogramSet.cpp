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
  hists["pt"]       = new TH1D((prefix+"_pt").c_str(),        (prefix+"_pt;p_{T} [GeV]; # events").c_str(),           100, 0,      5   );
  hists["mass_log"] = new TH1D((prefix+"_mass_log").c_str(),  (prefix+"_mass_log; log10(m/1GeV); # events").c_str(),     1000,  logxBins(1000,0.1,100));
  hists["mass"]     = new TH1D((prefix+"_mass").c_str(),      (prefix+"_mass; m [GeV]; # events").c_str(),         1500,  0,     2   );
  hists["eta"]      = new TH1D((prefix+"_eta").c_str(),       (prefix+"_eta; #eta; # events").c_str(),          1000, -5,     5     );
  hists["phi"]      = new TH1D((prefix+"_phi").c_str(),       (prefix+"_phi; #phi; # events").c_str(),          1000, -5,     5     );
}

void HistogramSet::fill(const Particle *particle)
{
  hists["pt"]->Fill(particle->four_vector.Pt());
  hists["mass"]->Fill(particle->four_vector.M());
  hists["mass_log"]->Fill(particle->four_vector.M());
  hists["eta"]->Fill(particle->four_vector.Eta());
  hists["phi"]->Fill(particle->four_vector.Phi());
}

void HistogramSet::fill(const Particle* particle_1, const Particle* particle_2)
{    
  TLorentzVector diparticle = particle_1->four_vector + particle_2->four_vector;
  hists["pt"]->Fill(diparticle.Pt());
  hists["mass"]->Fill(diparticle.M());
  hists["mass_log"]->Fill(diparticle.M());
  hists["eta"]->Fill(diparticle.Eta());
  hists["phi"]->Fill(diparticle.Phi());
}

float* HistogramSet::logxBins(const int n_bins, const float min, const float max)
{
  float* binList = new float[n_bins+1];
  for (int i=0; i<n_bins+1; i++) {
    binList[i] = (pow(10,log10(min)+((log10(max)-log10(min))/n_bins)*i));
  }
  return binList;
}
