//
//  Event.cpp
//  selections
//
//  Created by Jeremi Niedziela on 10/02/2023.
//

#include <iostream>


#include "Event.hpp"

using namespace std;

void Event::print_all_particles()
{
  for(auto particle : particles) particle->print();
}

void Event::setup()
{
  setup_particle_mothers();
  leading_muon_index = get_leading_muon_index();
}

void Event::setup_particle_mothers()
{
  for(int i_particle=0; i_particle<particles.size(); i_particle++){
    for(int daughter_index : particles[i_particle]->daughters){
      if(daughter_index >= 0){
        if(daughter_index != i_particle) particles.at(daughter_index)->mothers.push_back(i_particle);
        else particles.at(daughter_index)->is_selfmother = true;
      }
    }
  }
}

int Event::get_n_muons()
{
  int n_muons = 0;
  for(auto particle : particles) if(abs(particle->pdgid)==13) n_muons++;
  return n_muons;
}

vector<int> Event::get_sisters_indices(Particle *mother, int i_particle)
{
  vector<int> sister_indices;
  
  for(int daughter_index : mother->daughters){
    if(daughter_index != i_particle) sister_indices.push_back(daughter_index);
  }
  
  return sister_indices;
}

int Event::get_leading_muon_index()
{
  int index = -1;
  // find first muon
  for(auto particle : particles)
  {
    if(abs(particle->pdgid) == 13)
    {
      index = particle->index;
      break;
    }
  }

  for(auto particle : particles)
  {
    if(abs(particle->pdgid) != 13) continue;
    if(particle->four_vector.Pt() > particles[index]->four_vector.Pt()) index = particle->index;
  }

  return index;
}