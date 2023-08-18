#include <string>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TKey.h"


void printHists()
{
    gROOT->SetBatch();
    TFile* file = new TFile("histograms_lclc.root", "read");

    TCanvas* c = new TCanvas("", "", 1200, 1200);

    c->SetLeftMargin(0.15);

    for (TObject* keyAsObj : *file->GetListOfKeys())
    {
        TKey* key = (TKey*)keyAsObj;
        TH1D* h = (TH1D*)file->Get(key->GetName());

        h->GetYaxis()->SetTitleOffset(2.);

        c->Clear();
        h->Draw();
        c->SaveAs(("images_lclc/" + (string)key->GetName() + ".png").c_str());
    }

}
