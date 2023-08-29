#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include <cstdlib>

using namespace Pythia8;


// check if an array contains any of the elements in another array
template <typename T, typename U>
bool contains(T&& arr, U elem){
  for(auto it = std::begin(arr); it != std::end(arr); ++it){
    if(*it == elem) return true;
  }
  return false;
}


bool are_args_ok(int argc){
  if (argc == 3) return true;
  cerr << " Unexpected number of command-line arguments. \n You are expected to provide: "<<endl;
  cerr << "1. pythia config path" << endl;
  cerr << "2. output file name" << endl;
  cerr << " Program stopped! " << endl;
  return false;
}

bool is_file_ok(char* path){
  ifstream is(path);
  if(is) return true;
  cerr << " Command-line file " << path << " was not found.\nProgram stopped! " << endl;
  return false;
}

int main(int argc, char* argv[]) {
  if(!are_args_ok(argc)) return 1;
  if(!is_file_ok(argv[1])) return 1;

  cout << "\n >>> PYTHIA settings will be read from file " << argv[1]
    << " <<< \n >>> HepMC events will be written to file "
    << argv[2] << " <<< \n" << endl;
  
  // Prepare Pythia generator
  Pythia pythia;
  // beams
  pythia.readString("Beams:idA =  2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:eCM = 14000.");
  // enable processes
  pythia.readString("HardQCD:hardbbbar = on");


  pythia.readFile(argv[1]);
  pythia.readString("Print:quiet = on");
  int nEvent = pythia.mode("Main:numberOfEvents");
  int nAbort = pythia.mode("Main:timesAllowErrors");

  // Enable rescattering. Retune pT0Ref to get correct charged multiplicity.
  pythia.readString("HadronLevel:rescatter = on");
  pythia.readString("MultipartonInteractions:pT0Ref = 2.345");
  pythia.particleData.checkTable();

  pythia.init();

  SlowJet slowjet(-1, 0.7);
  
  Pythia8ToHepMC toHepMC(argv[2]);

  // Begin event loop.
  int iAbort = 0;

  cout<<"Will generate "<<nEvent<<" events"<<endl;

  // ids of particles modified to imitate hexaquarks
  int hexaquarks_ids[] = {521, -521};

  int iEvent = 0, hexa_count = 0;
  while(iEvent < nEvent) {
    iEvent++;
    if(iEvent%100 == 0)cout<<"Event "<<iEvent<<endl;
    if (!pythia.next()) {
     cout<<"There was some failure"<<endl;
      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) {
        cout << " Aborted since reached end of Les Houches Event File\n";
        break;
      }

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      
      cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }

    bool is_hexaquark = false;
    for(int i = 1; i < pythia.event.size(); i++)
    {
      if(contains(hexaquarks_ids, abs(pythia.event[i].id()))){
        is_hexaquark = true;
        hexa_count++;
        break;
      }
    }
    // save only events containing hexaquarks
    if(is_hexaquark == false) continue;

    // find jets
    slowjet.analyze(pythia.event);
    for(int i = 0; i < slowjet.sizeJet(); i++)
    {
      pythia.event.append(99, 1, 0, 0, slowjet.p(i), slowjet.m(i));
    }

    if(iEvent%100 == 0) cout<<"Saving to HepMC"<<endl;
    try{ toHepMC.writeNextEvent(pythia); }
    catch(std::out_of_range&){ cout<<"vector out of range"<<endl; }
    if(iEvent%100 == 0) cout<<"Event finished"<<endl;
  }
  

  cout<<"Events loop finished"<<endl;
  pythia.stat();

  return 0;
}
