#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "DetectorConstruction.hh"
//#include "EventAction.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
	SteppingAction(/*EventAction* eventAction*/);
	~SteppingAction();

	static SteppingAction* Instance();

	virtual void UserSteppingAction(const G4Step*);

	void Reset();

	void SetVolume(G4LogicalVolume* sv){ scoringVol = sv; };

	G4double GetEnergy() const { return Etotal; };

private:
	static SteppingAction* fgInstance;	

	//EventAction *fEventAction;
	G4LogicalVolume *scoringVol;
	G4double Etotal;
};

#endif
