#include "EventAction.hh"

EventAction::EventAction(RunAction*)
{
	gammaCollectionID = -1;
	evt = NULL;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* ev)
{
	evt = ev;

	G4SDManager * SDman = G4SDManager::GetSDMpointer();

	
	if(gammaCollectionID < 0){
		gammaCollectionID=SDman->GetCollectionID("gammaCollection");
	}
	
	SteppingAction::Instance()->Reset();

}

void EventAction::EndOfEventAction(const G4Event* ev)
{	
	evt = ev;
	
	G4HCofThisEvent * HCE = evt->GetHCofThisEvent();

	if(HCE){
		TrackerGammaHitsCollection *gammaCollection = (TrackerGammaHitsCollection*)(HCE->GetHC(gammaCollectionID));

		G4int Nhits = gammaCollection->entries();

		if(Nhits > 0){
			G4double totalEdep[ncrystals] = {};	

			for(G4int i = 0; i < Nhits; i++){
				G4int detid = (*gammaCollection)[i]->GetDetID();

				G4double en = (*gammaCollection)[i]->GetEdep();
				totalEdep[detid] += en;

			}

			G4double nCrystalsHit = 0;

			for (G4int i = 0; i < ncrystals; i++){
				if (totalEdep[i] > 0) 
					nCrystalsHit++;
			}
		
			G4AnalysisManager *man = G4AnalysisManager::Instance();
			
		
			for(int i = 0; i < ncrystals; i++){					
				// uncomment to "turn on" energy resolution
				man->FillNtupleDColumn(0, i, totalEdep[i]/keV /*+  CLHEP::RandGauss::shoot(0, enRes)*/);
			}	
			

			man->AddNtupleRow(0);

		}

	}


}
