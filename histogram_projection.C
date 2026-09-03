#include <TFile.h>
#include <TH2.h>
#include <TH1D.h>
#include <TString.h>

void histogram_projection() {
    // 1. Open the source ROOT file containing the 2D histogram
    TFile *inFile = TFile::Open("hist1238-00.root", "READ");
    if (!inFile || inFile->IsZombie()) {
        printf("Error: Cannot open input file.\n");
        return;
    }

    // 2. Retrieve the 2D histogram
    TH2F *h2 = (TH2F*)inFile->Get("hpge/clovers"); // Replace with your TH2 name
    if (!h2) {
        printf("Error: Histogram not found.\n");
        inFile->Close();
        return;
    }

    // 3. Create a new output ROOT file
    TFile *outFile = TFile::Open("output_projections_all.root", "RECREATE");

    // 4. Project ALL Y-bins onto the X-axis
    // By not specifying firstYBin and lastYBin, ROOT defaults to [1, GetNbinsY()]
    TH1D *h1_proj_all = h2->ProjectionX("xproj");
    h1_proj_all->SetTitle("X projection bins [1 .. 100]");


    // 5. Write the 1D projection directly into the output ROOT file
    outFile->cd();
    h1_proj_all->Write();


    // Clean up file handles
    outFile->Close();
    inFile->Close();

    printf("Successfully created projections in 'output_projections.root'\n");
}