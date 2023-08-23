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

int max_events = 5000;
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


void trigger_pass_count(const vector<Event*> &events)
{
  int i_event = 0;
  int n_passed = 0;
  for(auto event : events){
    int n_mu = 0;
    bool mu_pt_con = false;
    bool jpsi_pt_con = false;
    for(auto particle : event->particles){
      if(abs(particle->pdgid) == 13){
        n_mu++;
        if(particle->four_vector.Pt() > 2) mu_pt_con = true;
      }
      if(particle->pdgid == 443 && particle->four_vector.Pt() > 3.5) jpsi_pt_con = true;
    }
    if(n_mu >= 2 && mu_pt_con && jpsi_pt_con) n_passed++;
    i_event++;
  }
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
  
  trigger_pass_count(events);
  
  input_file->Close();
  return 0;
}
