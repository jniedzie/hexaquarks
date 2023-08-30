#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>

#include "Particle.hpp"
#include "Event.hpp"
#include "EventReader.hpp"
#include "Helpers.hpp"

using namespace std;

int max_events = 10000;
int n_daughters = 100;


TFile *input_file;

TTree* get_input_tree(string input_path)
{
  input_file = TFile::Open(input_path.c_str());
  
  if(!input_file){
    cout<<"ERROR -- could not open input file: "<<input_path<<endl;
    exit(0);
  }
  
  return (TTree*)input_file->Get("Events");
}


void jpsi_trigger(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    bool mu_con = false;
    bool jpsi_con = false;
    vector<Particle*> muons;

    for(auto particle : event->particles){
      if(abs(particle->pdgid) == 13 && abs(particle->four_vector.Eta()) < 3.5) muons.push_back(particle);
      if(particle->pdgid == 443 && particle->four_vector.Pt() > 3.5) jpsi_con = true;
    }
    for(int i = 0; i < muons.size(); i++){
      for(int j = i + 1; j < muons.size(); j++){
        if(muons[i]->pdgid == -1*muons[j]->pdgid && (muons[i]->four_vector.Pt() > 2 || muons[j]->four_vector.Pt() > 2)) mu_con = true;
      }
    }

    if(mu_con && jpsi_con) n_passed++;
    i_event++;
  }
  cout << "J/psi trigger" << endl;
  cout << n_passed << " out of " << i_event << " events passed" << endl;
}

void jet_trigger(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    bool jet_pt_con = false;
    for(auto particle : event->particles){
      if(particle->pdgid == 99 && particle->four_vector.Pt() > 550 && abs(particle->four_vector.Eta()) < 3.0) jet_pt_con = true;
    }
    if(jet_pt_con == true) n_passed++;
    i_event++;
  }
  cout << "jet trigger" << endl;
  cout << n_passed << " out of " << i_event << " events passed" << endl;
}

void jet_trigger2(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    float jet_pt_sum=0;
    for(auto particle : event->particles){
      if(particle->pdgid == 99 && abs(particle->four_vector.Eta()) < 3.0) jet_pt_sum += particle->four_vector.Pt();
    }
    if(jet_pt_sum > 1050) n_passed++;
    i_event++;
  }
  cout << "jet trigger2" << endl;
  cout << n_passed << " out of " << i_event << " events passed" << endl;
}

void single_muon_trigger(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    bool mu_pt_con = false;
    for(auto particle : event->particles){
      if(abs(particle->pdgid) == 13 && particle->four_vector.Pt() > 24 && abs(particle->four_vector.Eta()) < 2.1) mu_pt_con = true;
    }
    if(mu_pt_con == true) n_passed++;
    i_event++;
  }
  cout << "single_muon" << endl;
  cout << n_passed << " out of " << i_event << " events passed" << endl;
}

void double_muon_trigger(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    vector<Particle*> muons;
    bool mu_con = false;
    for(auto particle : event->particles){
      if(abs(particle->pdgid) == 13 && abs(particle->four_vector.Eta()) < 3.5) muons.push_back(particle);
    }

    for(int i = 0; i < muons.size(); i++){
      for(int j = i + 1; j < muons.size(); j++){
        if(muons[i]->pdgid == -1*muons[j]->pdgid && 
        ((muons[i]->four_vector.Pt() > 17 && muons[j]->four_vector.Pt() > 8) || (muons[i]->four_vector.Pt() > 8 && muons[j]->four_vector.Pt() > 17)) && 
        (muons[i]->four_vector + muons[j]->four_vector).M() > 3.8) mu_con = true;
      }
    }
    if(mu_con == true) n_passed++;
    i_event++;
  }
  cout << "double_muon" << endl;
  cout << n_passed << " out of " << i_event << " events passed" << endl;
}

int main(int argc, char *argv[])
{
  if(argc != 2){
    cout<<"Usage: ./prepare_histograms input_path"<<endl;
    exit(0);
  }
  
  string input_path = argv[1];
  
  // load events
  auto input_tree = get_input_tree(input_path);
  auto event_reader = EventReader(max_events, n_daughters);
  auto events = event_reader.read_events(input_tree);
  
  jpsi_trigger(events);
  jet_trigger(events);
  jet_trigger2(events);
  single_muon_trigger(events);
  double_muon_trigger(events);
  
  input_file->Close();
  return 0;
}
