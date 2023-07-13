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

int max_events = -1;
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

void fill_and_save_histograms(const vector<Event*> &events, string output_path)
{
  auto histogramFiller = HistogramFiller();

  for(auto event : events){
    for(auto particle : event->particles){
      histogramFiller.fill_hists(particle);
    }
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
