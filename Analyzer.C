#define Analyzer_cxx
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

#include "Analyzer.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TH1F.h>

#include <TCanvas.h>


TH1F * hEnergy[64];
TH1F * hAddback[16];


void Analyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   
   for( int i = 0; i < 52; i++){
        hEnergy[i] = new TH1F(Form("hE%02d", i), Form("hE%02d", i), 4000, 0, 4000);
    
        if( i < 13  ){
            hAddback[i] = new TH1F(Form("AE%02d",i), Form("AE%02d",i), 4000,0,4000);

           
        }

   }
   
}

void Analyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t Analyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either Analyzer::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   b_energy->GetEntry(entry);

   // Check your Analyzer.h to ensure this matches your branch pointer name!

   double addback[16] ={0};

   //----Process HPGe crystals----//
   for(int i =0; i < 64; i++){
     if((*energy)[i] <= 0.) { continue ; }
     ///   printf("%2d, %f \n", i, (*energy)[i]);
     hEnergy[i]->Fill((*energy)[i]);

     int iclover = i/4;

     addback[iclover] += (*energy)[i];
  //   if( entry < 10 ) printf("%d , %f, %d, %f \n", i, (*energy)[i], iclover, addback[iclover]);  
   }
   

   // --- Process Addback //
   for( int i = 0; i < 16; i++){
     if(addback[i] <= 0.){continue;}
     hAddback[i]->Fill(addback[i]);

   }

   return kTRUE;
}

void Analyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

// void Analyzer::Terminate()
// {
//    TFile * out = new TFile("CloverAddback.root","recreate");
//    out->cd();

//    for(int j = 0; j< 64; j++){
       
//         hEnergy[j]->Write(); 

//    }
//    for(int k = 0; k< 16; k++){
//         hAddback[k]->Write();
//    }
//     out->Close();
//    // The Terminate() function is the last function to be called during
//    // a query. It always runs on the client, it can be used to present
//    // the results graphically or save the results to file.

//     hAddback[14]->Draw();
// }

void Analyzer::Terminate()
{
    TFile *out = new TFile("CloverAddback.root","recreate");
    out->cd();

    // Write Crystals
    for(int j = 0; j < 64; j++){
        hEnergy[j]->Write();
    }

    for(int k = 0; k < 16; k++){
        hAddback[k]->Write();
    }

    out->Close();

    //--- New:  Draw the Addback canvas with the suppressed spectrum overlaid
    TCanvas *c = new TCanvas("cAddback","Clover Addback: Raw (Blue) vs Suppressed (Red)",1200,800);    
    c->Divide(4,4);

    for(int k = 0; k < 16; k++){
        c->cd(k+1);
        hAddback[k]->Draw();
    }

}

// void Analyzer::Terminate()
// {
//     TFile out("CloverAddback.root", "RECREATE");

//     for (int i = 0; i < 64; i++)
//         hEnergy[i]->Write();

//     for (int c = 0; c < 16; c++)
//         hAddback[c]->Write();

//     out.Close();
// }