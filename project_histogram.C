#include <TFile.h>
#include <TH2.h>
#include <TH1D.h>
#include <TString.h>

void project_histogram() {
    // 1. Open the source ROOT file containing the 2D histogram
    TFile *inFile = TFile::Open("hists415.root", "READ");
    if (!inFile || inFile->IsZombie()) {
        printf("Error: Cannot open input file.\n");
        return;
    }

    // 2. Retrieve the 2D histogram
    TH2F *h2 = (TH2F*)inFile->Get("labr/summary_ecal"); // Replace with your TH2 name
    if (!h2) {
        printf("Error: Histogram QDCsummary' not found.\n");
        inFile->Close();
        return;
    }

    // 3. Create a new output ROOT file
    TFile *outFile = TFile::Open("output_projections.root", "RECREATE");

    // 4. Define Y-bin ranges to project out
    // Example: Projecting 3 separate slices on the Y axis
    int sliceRanges[][2] = {
        {0, 0},    // Slice 1: Y-bins 1 through 5
        {1, 1},    // Slice 1: Y-bins 1 through 5
        {2, 2},    // Slice 1: Y-bins 1 through 5
        {3, 3},    // Slice 1: Y-bins 1 through 5
        {4, 4},    // Slice 1: Y-bins 1 through 5
        {5, 5},    // Slice 1: Y-bins 1 through 5
        {6, 6},    // Slice 1: Y-bins 1 through 5
        {7, 7},    // Slice 1: Y-bins 1 through 5
        {8, 8},    // Slice 1: Y-bins 1 through 5
        {9, 9},    // Slice 1: Y-bins 1 through 5
        {10, 10},    // Slice 1: Y-bins 1 through 5
        {11, 11},    // Slice 1: Y-bins 1 through 5
        {12, 13},    // Slice 1: Y-bins 1 through 5
        {14, 14},    // Slice 1: Y-bins 1 through 5
        {15, 15},    // Slice 1: Y-bins 1 through 5
        {16, 16},    // Slice 1: Y-bins 1 through 5
        
    };
    int numSlices = sizeof(sliceRanges) / sizeof(sliceRanges[0]);

    for (int i = 0; i < numSlices; ++i) {
        int firstYBin = sliceRanges[i][0];
        int lastYBin  = sliceRanges[i][1];

        // Unique name and title for each projection
        TString projName  = Form("h1_proj_y_%d_%d", firstYBin -1 , lastYBin -1);
        TString projTitle = Form("X Projection (Y bins %d to %d)", firstYBin - 1, lastYBin -1);

        // ProjectionX creates a TH1D of the specified Y-bin window
        TH1D *h1_proj = h2->ProjectionX(projName, firstYBin , lastYBin);
        h1_proj->SetTitle(projTitle);

        // 5. Write the 1D projection directly into the output ROOT file
        outFile->cd();
        h1_proj->Write();
    }

    // Clean up file handles
    outFile->Close();
    inFile->Close();

    printf("Successfully created projections in 'output_projections.root'\n");
}