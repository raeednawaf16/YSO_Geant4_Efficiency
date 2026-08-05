#include "RunAction.hh"

RunAction::RunAction()
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->OpenFile("output.root");

	man->CreateNtuple("tree", "tree");



	for(int i = 0; i < ncrystals; i++){
		char buff[50];
		int n = sprintf(buff, "clover_energy_%d", i);
		man->CreateNtupleDColumn(buff);
	}

	man->FinishNtuple(0);

}

void RunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}

