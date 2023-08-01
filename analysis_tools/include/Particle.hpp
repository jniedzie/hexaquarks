//
// Created by Jeremi Niedziela on 10/02/2023.
//

#ifndef SELECTION_TOOLS_PARTICLE_H
#define SELECTION_TOOLS_PARTICLE_H

#include <vector>

#include <TLorentzVector.h>
#include "Event.hpp"

class Event;

class Particle
{
public:
  Particle(float _x, float _y, float _z, float _px, float _py, float _pz, float _energy, float _mass, float _ctau,
           int _pdgid, std::vector<int> _daughters, int _status, int _index, int _barcode, Event* _event);
  
  ~Particle(){}
  
  void print();
  
  bool is_gluon(){return pdgid==21;}
  bool is_electron(){return abs(pdgid)==11;}
  bool is_muon(){return abs(pdgid)==13;}
  bool is_photon(){return pdgid==22;}
  bool is_pion() const;
  bool is_kaon() const;
  bool is_neutrino();
  bool is_quark();

  bool has_top_ancestor(const std::vector<Particle*> &other_particles);
  
  bool is_final() const;
  
  bool has_daughters() const;

  double eta();
  double momentum() const;
  double pt() const;
  TVector3 get_boost() const;  

  float x, y, z, px, py, pz, energy, mass, ctau;
  std::vector<int> mothers, daughters;
  int pdgid, status, index, barcode;
  bool is_selfmother;
  
  TLorentzVector four_vector;

  Event* event;
};


#endif //SELECTION_TOOLS_PARTICLE_H
