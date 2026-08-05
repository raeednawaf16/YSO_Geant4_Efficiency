#include "SteppingAction.hh"

SteppingAction* SteppingAction::fgInstance = 0;

SteppingAction* SteppingAction::Instance()
{
	return fgInstance;
}

SteppingAction::SteppingAction(/*EventAction *eventAction*/)
{
	//fEventAction = eventAction;
	fgInstance = this;
}

SteppingAction::~SteppingAction()
{
	fgInstance = 0;
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
	G4LogicalVolume* preVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4double edep = step->GetTotalEnergyDeposit();
	Etotal += edep;

	G4String physName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
	G4int repNum = step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber();

	G4double density = preVol->GetMaterial()->GetDensity();
	

}

void SteppingAction::Reset()
{
	Etotal = 0.;
}

