#include <string>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TKey.h"
#include "TROOT.h"

using namespace std;

bool are_args_ok(int argc){
  if (argc == 3) return true;
  cerr << " Unexpected number of command-line arguments. \n You are expected to provide: "<<endl;
  cerr << "1. input file name" << endl;
  cerr << "2. output directory name" << endl;
  cerr << " Program stopped! " << endl;
  return false;
}

bool is_file_ok(char* path){
  ifstream is(path);
  if(is) return true;
  cerr << " Command-line file " << path << " was not found.\nProgram stopped! " << endl;
  return false;
}

int main(int argc, char* argv[])
{
    string infile = argv[1], outdir = argv[2];
    if(!are_args_ok(argc)) return 1;
    if(!is_file_ok(argv[1])) return 1;
    gROOT->SetBatch();
    TFile* file = new TFile(argv[1], "read");
    TCanvas* c = new TCanvas("", "", 1200, 1200);

    c->SetLeftMargin(0.15);

    for (TObject* keyAsObj : *file->GetListOfKeys())
    {
        TKey* key = (TKey*)keyAsObj;
        TH1D* h = (TH1D*)file->Get(key->GetName());

        h->GetYaxis()->SetTitleOffset(2.);
        c->Clear();
        h->Draw();

        if(outdir.back() != '/') outdir.append("/");
        c->SaveAs((outdir + (string)key->GetName() + ".png").c_str());
    }
    return 0;
}
