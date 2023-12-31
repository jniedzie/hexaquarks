//
// Created by Jeremi Niedziela on 10/02/2023.
//
#include <cstdlib>
#include <vector>
#include <iostream>
#include <TLorentzRotation.h>

#include "Particle.hpp"
#include "Event.hpp"
#include "Helpers.hpp"

using namespace std;

Particle::Particle(float _x, float _y, float _z, float _px, float _py, float _pz, float _energy, float _mass, float _ctau,
                   int _pdgid, vector<int> _daughters, int _status, int _index, int _barcode, Event* _event):
x(_x), y(_y), z(_z), px(_px), py(_py), pz(_pz), energy(_energy), mass(_mass), ctau(_ctau),
pdgid(_pdgid), daughters(_daughters), status(_status), index(_index), barcode(_barcode), event(_event)
{
  is_selfmother = false;
  
  four_vector = TLorentzVector();
  four_vector.SetPxPyPzE(px, py, pz, energy);
}

void Particle::print()
{
  cout<<"Particle "<<index<<" (pdg: "<<pdgid<<", status: "<<status<<"), daughters: ";
  for(int daughter : daughters) cout<<"\t"<<daughter;
  cout<<"\tmothers: ";
  for(int mother : mothers) cout<<"\t"<<mother;
  cout<<"\tbar: "<<barcode<<endl;
}

bool Particle::is_neutrino()
{
  if(abs(pdgid) == 12) return true;
  if(abs(pdgid) == 14) return true;
  if(abs(pdgid) == 16) return true;
  return false;
}

bool Particle::is_pion() const
{
  if(abs(pdgid)==111) return true;
  if(abs(pdgid)==211) return true;
  return false;
}

bool Particle::is_kaon() const
{
  if(abs(pdgid)==130) return true;
  if(abs(pdgid)==310) return true;
  if(abs(pdgid)==311) return true;
  if(abs(pdgid)==321) return true;
  return false;
}

bool Particle::is_quark()
{
  if(abs(pdgid) >= 1 && abs(pdgid) <= 8) return true;
  return false;
}

bool Particle::has_top_ancestor(const vector<Particle*> &other_particles)
{
  if(abs(pdgid) == 6 && status >= 60) return true;
  if(mothers.size()==0) return false;
  
  for(int mother_index : mothers){    
    auto mother = other_particles.at(mother_index);
    if(mother->has_top_ancestor(other_particles)) return true;
  }
  return false;
}

bool Particle::is_final() const
{
  if(abs(pdgid) == 6) return status == 62;
  if(abs(pdgid) == 13) return status == 1;
  if(abs(pdgid) == 211) return status == 1;
  
  return true;
}

bool Particle::has_daughters() const
{
  for(auto daughter : daughters){
    if(daughter != -1) return true;  
  }
  return false;
}

bool Particle::has_mother_with_id(int id) const{
  vector<int> mothers_id;
  for(auto mother : mothers) if(mother > 0) mothers_id.push_back(event->particles.at(mother)->pdgid);
  return contains(mothers_id, id);
}


double Particle::eta()
{
  return four_vector.Eta();
}

double Particle::momentum() const
{
  return four_vector.P();
}

double Particle::pt() const
{
  return four_vector.Pt();
}

TVector3 Particle::get_boost() const
{
  return four_vector.BoostVector();
}

