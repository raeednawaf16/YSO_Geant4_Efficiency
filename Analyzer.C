#define Analyzer_cxx

#include "Analyzer.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TTree.h>

// Histograms
TH1F * hEnergy[52];
TH1F * hAddback[13];

// Variables to hold the dynamically loaded branches from Geant4
Double_t clover_energy[52];
TBranch  *b_clover_energy[52];
Bool_t   branches_initialized = kFALSE;

void Analyzer::Begin(TTree * /*tree*/)
{
    TString option = GetOption();

    // Initialize 52 crystal histograms and 13 addback histograms
    for(int i = 0; i < 52; i++) {
        hEnergy[i] = new TH1F(Form("hE%02d", i), Form("Crystal %02d Energy;Energy (keV);Counts", i), 4000, 0, 4000);
        
        if(i < 13) {
            hAddback[i] = new TH1F(Form("AE%02d", i), Form("Clover Addback %02d;Energy (keV);Counts", i), 4000, 0, 4000);
        }
    }
}

void Analyzer::SlaveBegin(TTree * /*tree*/)
{
    TString option = GetOption();
}

Bool_t Analyzer::Process(Long64_t entry)
{
    // 1. Link the Geant4 branches dynamically on the first event
    if (!branches_initialized) {
        for(int i = 0; i < 52; i++) {
            fChain->SetBranchAddress(Form("clover_energy_%d", i), &clover_energy[i], &b_clover_energy[i]);
        }
        branches_initialized = kTRUE;
    }

    // 2. Load the data for this entry
    for(int i = 0; i < 52; i++) {
        if(b_clover_energy[i]) {
            b_clover_energy[i]->GetEntry(entry);
        } else {
            clover_energy[i] = 0; // Fallback if branch is missing
        }
    }

    double addback[13] = {0};

    // 3. Process HPGe crystals
    for(int i = 0; i < 52; i++){
        if(clover_energy[i] <= 0.) { continue; }
        
        // Fill individual crystal
        hEnergy[i]->Fill(clover_energy[i]);

        // Calculate addback (4 crystals per clover)
        int iclover = i / 4;
        addback[iclover] += clover_energy[i];
    }
   
    // 4. Process Addback
    for(int i = 0; i < 13; i++){
        if(addback[i] <= 0.) { continue; }
        hAddback[i]->Fill(addback[i]);
    }

    return kTRUE;
}

void Analyzer::SlaveTerminate()
{
}

void Analyzer::Terminate()
{
    TFile *out = new TFile("OutputAddback.root","recreate");
    out->cd();

    // Write Crystals
    for(int j = 0; j < 52; j++){
        hEnergy[j]->Write();
    }

    // Write Addback
    for(int k = 0; k < 13; k++){
        hAddback[k]->Write();
    }

    out->Close();

    // Draw the Addback canvas
    TCanvas *c = new TCanvas("cAddback","Clover Addback Spectra", 1200, 800);    
    c->Divide(4, 4);

    for(int k = 0; k < 13; k++){
        c->cd(k+1);
        hAddback[k]->Draw();
    }
}