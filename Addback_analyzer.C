#define Addback_analyzer_cxx
// The class definition in Analyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("Analyzer.C")
// root> T->Process("Analyzer.C","some options")
// root> T->Process("Analyzer.C+")
//

#include "Addback_analyzer.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TH1F.h>

#include <TCanvas.h>

void Addback_analyzer::Begin(TTree * /*tree*/) {}

void Addback_analyzer::SlaveBegin(TTree * /*tree*/)
{
   // Book Total Array Spectrum
   h_total = new TH1D("Total_Spectrum", "Total Energy All Crystals;Energy (keV);Counts", 3000, 0, 3000);
   fOutput->Add(h_total);

   // Book 2D Matrix (X = Energy, Y = Crystal ID)
   // 3000 bins from 0 to 3000 on X-axis, 52 bins from 0 to 52 on Y-axis
   h2_clovers = new TH2D("clovers", "Energy vs Crystal ID;Energy (keV);Crystal ID", 3000, 0, 3000, ncrystals, 0, ncrystals);
   fOutput->Add(h2_clovers);

   // Book Individual Crystal Spectra
   for (int i = 0; i < ncrystals; i++) {
      h_clover[i] = new TH1D(Form("Crystal_%d", i), Form("Spectrum Crystal %d;Energy (keV);Counts", i), 3000, 0, 3000);
      fOutput->Add(h_clover[i]);
   }

   //==== Addback Histogram Calculation ====//
   int nclovers = ncrystals / 4;

   h_total_addback = new TH1D("Total_Addback_Spectrum", "Total Addback energy All Clovers;Addback Energy (keV);Counts", 3000, 0, 3000);
   fOutput->Add(h_total_addback);

   h2_clovers_addback = new TH2D("clovers_addback", "Addback Energy vs Clover ID;Addback Energy (keV);Clover ID", 3000, 0, 3000, nclovers, 0, nclovers);
   fOutput->Add(h2_clovers_addback);

   for (int c = 0; c < nclovers; c++) {
      h_clover_addback[c] = new TH1D(Form("Clover_%d_Addback", c), Form("Addback Spectrum Clover %d;Addback Energy (keV);Counts", c), 3000, 0, 3000);
      fOutput->Add(h_clover_addback[c]);
   }
   //==============================//
}

Bool_t Addback_analyzer::Process(Long64_t entry)
{
   GetEntry(entry);

   double threshold_keV = 40.0; // Filter low-energy noise and non-hits
   // double total_energy_keV = 0.0;
   // double energy_kev = 0.0;

   //=== Processing Individual Crystals (No addback!!)
   for (int i = 0; i < ncrystals; i++) {
    if (clover_energy[i] > threshold_keV) {
         
      // Fill the simple sum of all hits (1D)
      h_total->Fill(clover_energy[i]);
      
      // Fill the 2D histogram (Energy on X, Crystal ID on Y)
      h2_clovers->Fill(clover_energy[i], i);
      
      // Fill individual crystal (1D)
      h_clover[i]->Fill(clover_energy[i]);
      }
   }
   // h_total->Fill(total_energy_keV);

      // if (total_energy_keV > 0) {
      //    h_total->Fill(total_energy_keV);
      // }
    //==============================//


    //===== Processing addback spectra =====//
    int nclovers = ncrystals / 4;

    for (int a = 0; a < nclovers; a++){
        double addback_energy = 0.0;

        // Loop through the 4 crystals belonging to Clover a
        for (int j = 0; j < 4; j++){
            int crystal_id = (a*4) + j;
            if(clover_energy[crystal_id] > threshold_keV){
                addback_energy += clover_energy[crystal_id];
            }
        }

        // If at least one crystal in this clover fired above threshold, fill the addback spectra
        if(addback_energy > 0.0){
            h_total_addback->Fill(addback_energy);
            h2_clovers_addback->Fill(addback_energy, a);
            h_clover_addback[a]->Fill(addback_energy);
        }
    }

   return kTRUE;
}

void Addback_analyzer::SlaveTerminate() {}

void Addback_analyzer::Terminate()
{
TFile *outFile = new TFile("Simulated_Addback_Implant.root", "RECREATE");

// === Write single crytsal histograms directly to file ===//
if (h_total) h_total->Write();
if (h2_clovers) h2_clovers->Write();

for (int i = 0; i < ncrystals; i++) {
    if (h_clover[i]) h_clover[i]->Write();
}

// === Write addback histogram === //
int nclovers = ncrystals / 4;
if (h_total_addback) h_total_addback->Write();
if (h2_clovers_addback) h2_clovers_addback->Write();
for (int a = 0; a < nclovers; a++) {
    if (h_clover_addback[a]) h_clover_addback[a]->Write();
}

outFile->Close();
delete outFile;

std::cout << "Successfully created Simulated_Addback_Implant.root" << std::endl;
}

