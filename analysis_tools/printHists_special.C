#include <string>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TKey.h"
#include "THStack.h"
#include "TLegend.h"


void printHists_special()
{
    gROOT->SetBatch();
    TFile* file = new TFile("histograms_lclc.root", "read");
    TFile* file2 = new TFile("histograms_sm.root", "read");

    TCanvas* c = new TCanvas("", "", 1200, 1200);

    c->SetLeftMargin(0.15);

    TH1D* mu_pt_hexa = (TH1D*)file->Get("leading_muons_pt");
    TH1D* mu_pt_sm = (TH1D*)file2->Get("leading_muons_pt");

    mu_pt_hexa->Scale(1/mu_pt_hexa->Integral());
    mu_pt_sm->Scale(1/mu_pt_sm->Integral());

    mu_pt_hexa->SetLineColor(kRed);
    mu_pt_sm->SetLineColor(kBlue);

    THStack hs("hs", "leading mu pt; p_{T} [GeV]; probability");
    hs.Add(mu_pt_hexa);
    hs.Add(mu_pt_sm);

    TLegend leg(0.8, 0.8, 0.9, 0.9);
    leg.AddEntry(mu_pt_sm, "mu_pt_sm");
    leg.AddEntry(mu_pt_hexa, "mu_pt_hexa");


    c->Clear();
    hs.Draw("hist nostack");
    leg.Draw();
    hs.GetYaxis()->SetTitleOffset(2.);

    c->SaveAs("images_special/leading_muons_pt.png");
}
