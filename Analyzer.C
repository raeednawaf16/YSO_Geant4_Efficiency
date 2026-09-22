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

   // Book 2D Matrix (X = Energy, Y = Crystal ID)
   // 3000 bins from 0 to 3000 on X-axis, 52 bins from 0 to 52 on Y-axis
   h2_clovers = new TH2D("clovers", "Energy vs Crystal ID;Energy (keV);Crystal ID", 3000, 0, 3000, ncrystals, 0, ncrystals);
   fOutput->Add(h2_clovers);

   // Book Individual Crystal Spectra
   for (int i = 0; i < ncrystals; i++) {
      h_clover[i] = new TH1D(Form("Crystal_%d", i), Form("Spectrum Crystal %d;Energy (keV);Counts", i), 3000, 0, 3000);
      fOutput->Add(h_clover[i]);
   }
}

Bool_t Analyzer::Process(Long64_t entry)
{
   GetEntry(entry);

   double threshold_keV = 0.0; // Filter low-energy noise and non-hits
   // double total_energy_keV = 0.0;
   // double energy_kev = 0.0;

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

   return kTRUE;
}

void Analyzer::SlaveTerminate() {}

void Analyzer::Terminate()
{
TFile *outFile = new TFile("Simulated.root", "RECREATE");
   
   // Write histograms directly to file
   if (h_total) h_total->Write();
   if (h2_clovers) h2_clovers->Write();
   
   for (int i = 0; i < ncrystals; i++) {
      if (h_clover[i]) h_clover[i]->Write();
   }

   outFile->Close();
   delete outFile;

   std::cout << "Successfully created Simulated.root with 2D spectra!" << std::endl;
}