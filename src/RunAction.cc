#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    // Enable merging in the constructor so the master thread sees it
    man->SetNtupleMerging(true);

    // 3. Create the Ntuple structure here
    man->CreateNtuple("tree", "tree");

    for(int i = 0; i < ncrystals; i++){
        char buff[50];
        int n = sprintf(buff, "clover_energy_%d", i);
        man->CreateNtupleDColumn(buff);
    }

    man->FinishNtuple(0);

    // // 1D Histograms for HDTV
    // man->CreateH1("Total_Spectrum", "Total Energy All Crystals;Energy (keV);Counts", 3000, 0, 3000);

    // for(int i = 0; i < ncrystals; i++){
    //     char name[50], title[100];
    //     sprintf(name, "spec_clover_%d", i);
    //     sprintf(title, "Spectrum of Clover Crystal %d;Energy (keV);Counts", i);
    //     man->CreateH1(name, title, 3000, 0, 3000);
    // }
}


RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->OpenFile("output.root");

}

void RunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}

