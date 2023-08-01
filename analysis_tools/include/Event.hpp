//
//  Event.hpp
//  selections
//
//  Created by Jeremi Niedziela on 10/02/2023.
//

#ifndef Event_hpp
#define Event_hpp

#include <vector>

#include "Particle.hpp"

class Particle;

class Event
{
public:
  Event(){}
  
  void print_all_particles();
  
  void add_particle(Particle *particle){particles.push_back(particle);}
  
  void setup();
  
  int get_n_muons();
  
  std::vector<Particle*> particles;
  
private:
  void setup_particle_mothers();
  
  std::vector<int> get_sisters_indices(Particle *mother, int i_particle);
};

#endif /* Event_hpp */
