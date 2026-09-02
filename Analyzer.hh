#ifndef Analyzer_hh
#define Analyzer_hh

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

class Analyzer : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           nTraj;
   std::vector<double>  *energy;      // Added std::
   std::vector<double>  *stepLength;   

   // --- NEW: BGO Leaf types ---
   std::vector<double>  *bgoE;
   std::vector<double>  *bgoL;
   
   Double_t        bEnergy;
   Double_t        theta;
   Double_t        phi;
   Double_t        totalBGO; // --- NEW: Total BGO energy ---

   // List of branches
   TBranch        *b_nTraj;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_stepLength;   //!

   // --- NEW: BGO Branches ---
   TBranch        *b_bgoE;     //!
   TBranch        *b_bgoL;     //!

   TBranch        *b_bEnergy;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_phi;   //!

   TBranch        *b_totalBGO;  //! --- NEW: Total BGO Branch ---

   Analyzer(TTree * /*tree*/ =0) : fChain(0) { }
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

   // Set object pointer
   energy = 0;
   stepLength = 0;

   bgoE = 0; // --- NEW: Initialize BGO vector pointers to 0 ---
   bgoL = 0; // --- NEW ---

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nTraj", &nTraj, &b_nTraj);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("stepLength", &stepLength, &b_stepLength);

   // --- NEW: Set BGO Branch Addresses ---
   fChain->SetBranchAddress("bgoEnergy", &bgoE, &b_bgoE);
   fChain->SetBranchAddress("bgoStepLength", &bgoL, &b_bgoL);

   fChain->SetBranchAddress("bEnergy", &bEnergy, &b_bEnergy);
   fChain->SetBranchAddress("theta", &theta, &b_theta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);

   // --- NEW: Set totalBGO Branch Address ---
   fChain->SetBranchAddress("totalBGOenergy", &totalBGO, &b_totalBGO);
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
