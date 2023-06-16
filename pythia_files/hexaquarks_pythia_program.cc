#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/EvtGen.h"
#include "Pythia8Plugins/HepMC2.h"

using namespace Pythia8;

bool are_args_ok(int argc){
  if (argc == 6) return true;
  cerr << " Unexpected number of command-line arguments. \n You are expected to provide: "<<endl;
  cerr << "1. pythia config path" << endl;
  cerr << "2. output file name" << endl;
  cerr << "3. EvtGen decay file (e.g. DECAY_2010.DEC)" << endl;
  cerr << "4. EvtGen particle data (e.g. evt.pdl)" << endl;
  cerr << "5. PYTHIA8DATA path" << endl;
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
    pythia.readFile(argv[1]);
    pythia.readString("Print:quiet = on");
    pythia.readString("HardQCD:hardbbbar = on");
    int nEvent = pythia.mode("Main:numberOfEvents");
    int nAbort = pythia.mode("Main:timesAllowErrors");
    pythia.init();

    // Prepare EvtGen decayer
    EvtGenDecays *evtgen = 0;
    setenv("PYTHIA8DATA", argv[5], 1);
    evtgen = new EvtGenDecays(&pythia, argv[3], argv[4]);
    evtgen->readDecayFile("hexaquarks.dec");
    
    Pythia8ToHepMC toHepMC(argv[2]);

    // Begin event loop.
    int iAbort = 0;
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

      if (!pythia.next()) {
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

    // Perform the decays with EvtGen.
    if (evtgen) evtgen->decay();

    
    toHepMC.writeNextEvent( pythia );
  }
  pythia.stat();

  if (evtgen) delete evtgen;
  return 0;
}
