

void printHists()
{
    gROOT->SetBatch();
    TFile* file = new TFile("histograms.root", "read");

    TCanvas* c = new TCanvas("", "", 800, 800);

    for (TObject* keyAsObj : *file->GetListOfKeys())
    {
        TKey* key = (TKey*)keyAsObj;
        TH1D* h = (TH1D*)file->Get(key->GetName());

        c->Clear();
        h->Draw();
        c->SaveAs(("images/" + (string)key->GetName() + ".png").c_str());
    }

}