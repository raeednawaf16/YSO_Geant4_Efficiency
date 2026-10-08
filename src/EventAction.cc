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

	// ================= EDIT: record the source (decay) position =================

	G4AnalysisManager *am = G4AnalysisManager::Instance();
	G4PrimaryVertex *vtx = evt->GetPrimaryVertex();
	if (vtx) {
		G4ThreeVector pos = vtx->GetPosition();
		am->FillNtupleDColumn(1, 0, pos.x()/mm + 24.);
		am->FillNtupleDColumn(1, 1, pos.y()/mm + 24.);
		am->FillNtupleDColumn(1, 2, pos.z()/mm);
		am->AddNtupleRow(1);
	}
	// ============================================================================
	
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
				if (totalEdep[i] > 50) 
					nCrystalsHit++;
			}
		
			G4AnalysisManager *man = G4AnalysisManager::Instance();
			
			
			for(int i = 0; i < ncrystals; i++){					
				// uncomment to "turn on" energy resolution
				man->FillNtupleDColumn(0, i, totalEdep[i]/keV +  CLHEP::RandGauss::shoot(0, enRes));
			}	
			

			man->AddNtupleRow(0);

		}

	}


}
