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
    /* TFile* file = new TFile("histograms/histograms_lclc_jpsi2mumu_jets.root", "read");
    TFile* file2 = new TFile("histograms/histograms_sm_jets.root", "read");

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

    THStack hs("hs", "leading #\mu p_{T}; p_{T} [GeV]; probability");
    hs.Add(mu_pt_hexa);
    hs.Add(mu_pt_sm);

    TLegend leg(0.7, 0.7, 0.9, 0.9);
    leg.AddEntry(mu_pt_sm, "background");
    leg.AddEntry(mu_pt_hexa, "signal");


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

    THStack hs_jet("hs_jet", "leading jet p_{T}; p_{T} [GeV]; probability");
    hs_jet.Add(jet_pt_hexa);
    hs_jet.Add(jet_pt_sm);

    TLegend leg_jet(0.8, 0.8, 0.9, 0.9);
    leg_jet.AddEntry(jet_pt_sm, "background");
    leg_jet.AddEntry(jet_pt_hexa, "signal");


    c->Clear();
    hs_jet.Draw("hist nostack");
    leg_jet.Draw();
    hs_jet.GetYaxis()->SetTitleOffset(2.);

    c->SaveAs("images_special/leading_jets_pt.png"); */


    TFile* file = new TFile("histograms/histograms_lclc_pi+pi-jpsi_jets4_100000.root", "read");
    TFile* file2 = new TFile("histograms/histograms_pl_pi+pi-k+_jets4_100000.root", "read");

    TCanvas* c = new TCanvas("", "", 1200, 1200);

    c->SetLeftMargin(0.15);

    
    // 3 meson masses
    TH1D* mu_pt_hexa = (TH1D*)file->Get("pi+pi-jpsi_mass");
    TH1D* mu_pt_sm = (TH1D*)file2->Get("pi+pi-k+_mass");

    mu_pt_hexa->SetLineColor(kRed);
    mu_pt_sm->SetLineColor(kBlue);

    THStack hs("hs", "invariant mass of meson triplets; m [GeV]; # entries");
    hs.Add(mu_pt_hexa);
    hs.Add(mu_pt_sm);

    TLegend leg(0.7, 0.7, 0.9, 0.9);
    leg.AddEntry(mu_pt_sm, "#\pi^{+} #\pi^{-} K^{+}");
    leg.AddEntry(mu_pt_hexa, "#\pi^{+} #\pi^{-} J/#\psi");


    c->Clear();
    hs.Draw("hist nostack");
    leg.Draw();
    hs.GetYaxis()->SetTitleOffset(2.);

    c->SaveAs("images_special/3meson_mass.png");

    // leading jets

    TH1D* jet_pt_hexa = (TH1D*)file->Get("pi+pi-jpsi_from_lclc_mass");
    TH1D* jet_pt_sm = (TH1D*)file2->Get("pi+pi-k+_from_pl_mass");

    jet_pt_hexa->SetLineColor(kRed);
    jet_pt_sm->SetLineColor(kBlue);

    THStack hs_jet("hs_jet", "invariant mass of hexaquarks' decay products; m [GeV]; # entries");
    hs_jet.Add(jet_pt_hexa);
    hs_jet.Add(jet_pt_sm);

    TLegend leg_jet(0.7, 0.7, 0.9, 0.9);
    leg_jet.AddEntry(jet_pt_sm, "#\pi^{+} #\pi^{-} K^{+}");
    leg_jet.AddEntry(jet_pt_hexa, "#\pi^{+} #\pi^{-} J/#\psi");


    c->Clear();
    hs_jet.Draw("hist nostack");
    leg_jet.Draw();
    hs_jet.GetYaxis()->SetTitleOffset(2.);

    c->SaveAs("images_special/3meson_mass_from_hexa.png");



    return 0;
}
