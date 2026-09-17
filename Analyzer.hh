#ifndef Analyzer_hh
#define Analyzer_hh

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include<iostream>

class Analyzer : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   static const Int_t ncrystals = 52;

   Double_t clover_energy[ncrystals];
   
   TH1D *h_total;
   TH1D *h_clover[ncrystals];

// Fixed size dimensions of array or collections stored in the TTree if any.

   Analyzer(TTree * /*tree*/ =0) : fChain(0), h_total(0) { 
      for(int i = 0; i < ncrystals; i++) h_clover[i] = 0;
   }
   virtual ~Analyzer() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(Analyzer,0);
};

#endif

#ifdef Analyzer_cxx
void Analyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).


   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   // Bind all 52 crystal energy branches
   for(int i = 0; i < ncrystals; i++){
      fChain->SetBranchAddress(Form("clover_energy_%d", i), &clover_energy[i]);
   }
}

Bool_t Analyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef Analyzer_cxx
