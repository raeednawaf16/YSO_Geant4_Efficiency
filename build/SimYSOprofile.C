// drawImplantXY.C
// Draws the simulated decay positions as an "implantXY" histogram with the
// same binning as the experimental ysoProfile.root:
//   TH2F, 2000 x 2000 bins, 0-48 mm in x and y (0.024 mm per bin)
//
// Run in the folder containing output.root:
//   root -l drawImplantXY.C
//
// Output: simImplantXY.root (histogram + canvas), simImplantXY.png

#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>

void SimYSOprofile()
{
    TFile *fIn = TFile::Open("output.root");
    if (!fIn || fIn->IsZombie()) { std::cerr << "Cannot open output.root\n"; return; }
    TTree *vertex = fIn->Get<TTree>("vertex");
    if (!vertex) { std::cerr << "No 'vertex' tree in output.root\n"; return; }

    // Same name, type and binning as the experimental histogram
    TH2F *h = new TH2F("implantXY", "implantXY", 2000, 0., 48., 2000, 0., 48.);
    h->GetXaxis()->SetTitle("x (mm)");
    h->GetYaxis()->SetTitle("y (mm)");

    // Fill from the vertex ntuple (x_mm, y_mm already shifted to 0-48 mm)
    vertex->Draw("y_mm:x_mm>>implantXY", "", "goff");

    gStyle->SetOptStat(1111);   // Entries, Mean x/y, Std Dev x/y
    TCanvas *c = new TCanvas("c_implantXY", "Simulated implantXY", 1000, 800);
    c->SetRightMargin(0.14);
    h->Draw("COLZ");
    c->Update();
    c->SaveAs("simImplantXY.png");

    TFile *fOut = TFile::Open("simImplantXY.root", "RECREATE");
    h->Write();
    c->Write();
    fOut->Close();

    std::cout << "Entries " << h->GetEntries()
              << "  Mean x " << h->GetMean(1) << "  Mean y " << h->GetMean(2)
              << "  Std Dev x " << h->GetStdDev(1) << "  Std Dev y " << h->GetStdDev(2) << "\n";
    std::cout << "Written: simImplantXY.root, simImplantXY.png\n";
}