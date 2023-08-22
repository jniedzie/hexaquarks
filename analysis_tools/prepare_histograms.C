#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

#include "Particle.hpp"
#include "Event.hpp"
#include "EventReader.hpp"
#include "HistogramSet.hpp"
#include "Helpers.hpp"
#include "HistogramFiller.hpp"

using namespace std;

int max_events = 70000;
int n_daughters = 100;
const bool loop2 = false;
const bool loop3 = false;

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

void fill_and_save_histograms(const vector<Event*> &events, string output_path)
{
  auto histogramFiller = HistogramFiller();
  int i_event=0;
  for(auto event : events){
    for(auto particle : event->particles){
      if(particle->is_photon()) continue;
      if(particle->is_neutrino()) continue;
      if(particle->is_electron()) continue;
      if(particle->is_gluon()) continue;
      if(particle->is_quark()) continue;

      // if(particle->has_daughters()) continue;

      //if(i_event == 0) particle->print();

      // single particles
      histogramFiller.fill_hists(particle);

      // double particles
      if(!loop2) continue;
      for(auto particle2 : event->particles){
        if(particle2->is_photon()) continue;
        if(particle2->is_neutrino()) continue;
        if(particle2->is_electron()) continue;
        if(particle2->is_gluon()) continue;
        if(particle2->is_quark()) continue;
        if(particle == particle2) continue;

        histogramFiller.fill_hists(particle, particle2);

        // triple particles
        if(!loop3) continue;
        for(auto particle3 : event->particles){
          if(particle3->is_photon()) continue;
          if(particle3->is_neutrino()) continue;
          if(particle3->is_electron()) continue;
          if(particle3->is_gluon()) continue;
          if(particle3->is_quark()) continue;
          if(particle == particle2 || particle == particle3 || particle2 == particle3) continue;

          histogramFiller.fill_hists(particle, particle2, particle3);
        }
      }
    }

    i_event++;
  }

  histogramFiller.save_histograms(output_path);
}

int main(int argc, char *argv[])
{
  if(argc != 3){
    cout<<"Usage: ./prepare_histograms input_path output_path"<<endl;
    exit(0);
  }
  
  string input_path = argv[1];
  string output_path = argv[2];
  
  // load events
  auto input_tree = get_input_tree(input_path);
  auto event_reader = EventReader(max_events, n_daughters);
  auto events = event_reader.read_events(input_tree);
  
  fill_and_save_histograms(events, output_path);
  
  input_file->Close();
  return 0;
}
