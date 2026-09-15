#include <TFile.h>
#include <TH2.h>
#include <TH1D.h>
#include <TString.h>

void histogram_single_projection() {
    // 1. Open the source ROOT file containing the 2D histogram
    TFile *inFile = TFile::Open("hist1240-00.root", "READ");
    if (!inFile || inFile->IsZombie()) {
        printf("Error: Cannot open input file.\n");
        return;
    }

    // 2. Retrieve the 2D histogram
    TH2F *h2 = (TH2F*)inFile->Get("hpge/clovers"); // Update if your histogram name differs
    if (!h2) {
        printf("Error: Histogram 'clovers' not found.\n");
        inFile->Close();
        return;
    }

    // 3. Create a new output ROOT file
    TFile *outFile = TFile::Open("output_individual_projection.root", "RECREATE");

    // 4. Loop through all 52 channels individually
    for (int i = 1; i <= 52; ++i) {
        // Unique name and title for each channel projection
        TString projName  = Form("Channel_%d", i);
        TString projTitle = Form("X Projection (Y bin %d)", i);

        // ProjectionX creates a TH1D for the single Y-bin window (i to i)
        TH1D *h1_proj = h2->ProjectionX(projName, i, i);
        h1_proj->SetTitle(projTitle);

        // 5. Write the 1D projection directly into the output ROOT file
        outFile->cd();
        h1_proj->Write();
    }

    // Clean up file handles
    outFile->Close();
    inFile->Close();

    printf("Successfully created individual channel projections in 'output_individual_channels.root'\n");
}