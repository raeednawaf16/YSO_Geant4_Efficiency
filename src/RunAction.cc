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

    // Create the Ntuple structure here
    man->CreateNtuple("tree", "tree");

    for(int i = 0; i < ncrystals; i++){
        char buff[50];
        int n = sprintf(buff, "clover_energy_%d", i);
        man->CreateNtupleDColumn(buff);
    }

    man->FinishNtuple(0);


    // EDIT: second ntuple with the decay position of every event (id 1)
    man->CreateNtuple("vertex", "Decay positions");
    man->CreateNtupleDColumn("x_mm");   // shifted to 0-48 mm like ysoProfile.root
    man->CreateNtupleDColumn("y_mm");
    man->CreateNtupleDColumn("z_mm");
    man->FinishNtuple(1);

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

