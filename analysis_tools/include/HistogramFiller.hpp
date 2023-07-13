//
//  HistogramFiller.hpp
//  selections
//
//  Created by Jeremi Niedziela on 06/04/2023.
//

#ifndef HistogramFiller_hpp
#define HistogramFiller_hpp

#include <string>
#include <map>

#include "Particle.hpp"
#include "HistogramSet.hpp"
#include "Event.hpp"

class HistogramFiller {
public:
  HistogramFiller();
  ~HistogramFiller();
  
  void fill_hists(const Particle* particle);
  
  void save_histograms(std::string output_path);
  std::map<std::string, HistogramSet*> histSets;
  
};

#endif /* HistogramFiller_hpp */
