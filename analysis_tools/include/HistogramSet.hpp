//
//  HistogramSet.hpp
//  selections
//
//  Created by Jeremi Niedziela on 24/02/2023.
//

#ifndef HistogramSet_hpp
#define HistogramSet_hpp

#include <map>
#include <string>

#include <TH1D.h>
#include <TH2D.h>

#include "Particle.hpp"
#include "Event.hpp"

class HistogramSet {
public:
  HistogramSet(std::string prefix);
  
  void fill(const Particle* particle);
  void fill(const Particle* particle_1, const Particle* particle_2);
  void fill(const Particle* particle_1, const Particle* particle_2, const Particle* particle_3);
  
  std::map<std::string, TH1D*> hists;

private:
  float* logxBins(const int n_bins, const float min, const float max);
};

#endif /* HistogramSet_hpp */
