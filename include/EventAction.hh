#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"

#include "g4root.hh"

#include "RunAction.hh"
#include "SteppingAction.hh"
#include "TrackerGammaSD.hh"
#include "Constants.hh"

class EventAction : public G4UserEventAction
{
public:
	EventAction(RunAction*);
	~EventAction();

	virtual void BeginOfEventAction(const G4Event*);
	virtual void EndOfEventAction(const G4Event*);

private:
	G4int gammaCollectionID;
	const G4Event* evt;

	G4double enRes = 1.;  // keV
};

#endif
