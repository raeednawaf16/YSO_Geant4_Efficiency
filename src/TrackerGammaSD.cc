#include "TrackerGammaSD.hh"

TrackerGammaSD::TrackerGammaSD(G4String name)
	: G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname="gammaCollection");
}

TrackerGammaSD::~TrackerGammaSD()
{}


void TrackerGammaSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection
	gammaCollection = new TrackerGammaHitsCollection(SensitiveDetectorName, collectionName[0]);

	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection(hcID, gammaCollection);
}


G4bool TrackerGammaSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{	

	// Energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();

	if (edep == 0.) return false;

	G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();

	auto newHit = new TrackerGammaHit();

	newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
	newHit->SetEdep(edep);
	newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
	newHit->SetDetID(touchable->GetReplicaNumber()-5);
	//newHit->SetDetID(0);

	//newHit->Print();

	gammaCollection->insert(newHit);

	/*
	G4cout << "Energy deposition = " << edep << G4endl;

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, evt);
	man->FillNtupleDColumn(1, edep);
	man->AddNtupleRow(0);
	*/

	return true;
	
}


void TrackerGammaSD::EndOfEvent(G4HCofThisEvent*)
{
	if(verboseLevel > 1){
		std::size_t nofHits = gammaCollection->entries();
		if(nofHits > 0){
			G4cout << G4endl << "-------->Hits Collection:: in this event there are " << nofHits << " hits in the detector: " << G4endl;
			for (std::size_t i = 0; i < nofHits; i++)
				(*gammaCollection)[i]->Print();
		}
	}
}

