#include <string>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TKey.h"
#include "THStack.h"
#include "TLegend.h"
#include "TROOT.h"

using namespace std;

int main() {
    gROOT->SetBatch();
    TFile* file = new TFile("histograms_lclc_jpsi2mumu_jets.root", "read");
    TFile* file2 = new TFile("histograms_sm_jets.root", "read");

    TCanvas* c = new TCanvas("", "", 1200, 1200);

    c->SetLeftMargin(0.15);

    string hist_names[] = {"leading_muons_pt", "leading_jets_pt"};
    
    // leading muons
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

    // leading jets

    TH1D* jet_pt_hexa = (TH1D*)file->Get("leading_jets_pt");
    TH1D* jet_pt_sm = (TH1D*)file2->Get("leading_jets_pt");

    jet_pt_hexa->Scale(1/jet_pt_hexa->Integral());
    jet_pt_sm->Scale(1/jet_pt_sm->Integral());

    jet_pt_hexa->SetLineColor(kRed);
    jet_pt_sm->SetLineColor(kBlue);

    THStack hs_jet("hs_jet", "leading jet pt; p_{T} [GeV]; probability");
    hs_jet.Add(jet_pt_hexa);
    hs_jet.Add(jet_pt_sm);

    TLegend leg_jet(0.8, 0.8, 0.9, 0.9);
    leg_jet.AddEntry(jet_pt_sm, "jet_pt_sm");
    leg_jet.AddEntry(jet_pt_hexa, "jet_pt_hexa");


    c->Clear();
    hs_jet.Draw("hist nostack");
    leg_jet.Draw();
    hs_jet.GetYaxis()->SetTitleOffset(2.);

    c->SaveAs("images_special/leading_jets_pt.png");

    return 0;
}
