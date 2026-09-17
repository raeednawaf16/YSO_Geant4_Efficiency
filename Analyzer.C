#define Analyzer_cxx
#include "Analyzer.hh"
#include <TH2.h>
#include <TStyle.h>

void Analyzer::Begin(TTree * /*tree*/) {}

void Analyzer::SlaveBegin(TTree * /*tree*/)
{
   // Book Total Array Spectrum
   h_total = new TH1D("Total_Spectrum", "Total Energy All Crystals;Energy (keV);Counts", 3000, 0, 3000);
   fOutput->Add(h_total);

   // Book Individual Crystal Spectra
   for (int i = 0; i < ncrystals; i++) {
      h_clover[i] = new TH1D(Form("Crystal_%d", i), Form("Spectrum Crystal %d;Energy (keV);Counts", i), 3000, 0, 3000);
      fOutput->Add(h_clover[i]);
   }
}

Bool_t Analyzer::Process(Long64_t entry)
{
   GetEntry(entry);

//    double threshold_keV = 20.0; // Filter low-energy noise and non-hits

   for (int i = 0; i < ncrystals; i++) {
      if (clover_energy[i] > 0) {
         h_total->Fill(clover_energy[i]);
         h_clover[i]->Fill(clover_energy[i]);
      }
   }

   return kTRUE;
}

void Analyzer::SlaveTerminate() {}

void Analyzer::Terminate()
{
   TFile *outFile = new TFile("Simulated.root", "RECREATE");
   fOutput->Write();
   outFile->Close();
   
   std::cout << "Successfully created Simulated.root with 1D spectra!" << std::endl;
}