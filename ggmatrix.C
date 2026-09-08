#include <TFile.h>
#include <TH2.h>
#include <TH1D.h>
#include <iostream>

void ggmatrix() {
    TFile *inFile = TFile::Open("hist1240-00.root", "READ"); // Use your actual file name
    if (!inFile || inFile->IsZombie()) {
        std::cout << "Error: Cannot open input file." << std::endl;
        return;
    }
    TH2F *h2 = (TH2F*)inFile->Get("hpge/gg"); // Using the name from your image
    if (!h2) {
        std::cout << "Error: Matrix 'gg' not found!" << std::endl;
        delete inFile;
        return;
    }

    // Convert energy values to bin numbers

    // Peak gate for 1173 keV (from 1169 to 1177)
    int peakBinMin = h2->GetYaxis()->FindBin(1169.0); // 1173 keV peak
    int peakBinMax = h2->GetYaxis()->FindBin(1177.0); // 1173 keV peak

    int bgBinMin = h2->GetYaxis()->FindBin(1181.0); //background lower edge
    int bgBinMax = h2->GetYaxis()->FindBin(1189.0); //background upper edge
    
    // Peak gate for 1332 keV (from 1328 to 1336)
    // int peakBinMin = h2->GetYaxis()->FindBin(1328.0); // 1332 keV peak
    // int peakBinMax = h2->GetYaxis()->FindBin(1336.0); // 1332 keV peak
    
    // int bgBinMin = h2->GetYaxis()->FindBin(1340.0);
    // int bgBinMax = h2->GetYaxis()->FindBin(1348.0);

    // Project the peak gate and background gate
    TH1D *h1_peak = h2->ProjectionX("peak_gate", peakBinMin, peakBinMax);
    TH1D *h1_bg = h2->ProjectionX("bg_gate", bgBinMin, bgBinMax);

    // Scale background (if peak gate is 8 bins wide and bg gate is 8 bins wide, scale is 1.0)
    double numPeakBins = peakBinMax - peakBinMin + 1;
    double numBgBins = bgBinMax - bgBinMin + 1;
    double scaleFactor = numPeakBins / numBgBins;

    // Subtract background from the peak gate
    TH1D *h1_clean_gate = (TH1D*)h1_peak->Clone("clean_1173_gate");
    h1_clean_gate->SetTitle("1332 keV region gated on 1173 keV");
    h1_clean_gate->Add(h1_bg, -scaleFactor);

    // Save the resulting gated 1D spectrum
    TFile *outFile = TFile::Open("coincidence_1173_gated_1332.root", "RECREATE");
    h1_clean_gate->Write();
    
    outFile->Close();
    delete outFile;
    delete inFile;

    std::cout << "Successfully generated background-subtracted gated spectrum!" << std::endl;
}