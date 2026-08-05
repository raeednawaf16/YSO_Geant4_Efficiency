#ifndef TRACKERGAMMASD_HH
#define TRACKERGAMMASD_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"

#include "globals.hh"

#include "TrackerGammaHit.hh"

//class G4Step;
//class G4HCofThisEvent;
//class G4TouchableHistory;

class TrackerGammaSD : public G4VSensitiveDetector
{
public:
	TrackerGammaSD(G4String);
	~TrackerGammaSD();

	// methods from base class
	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);

private:	

	TrackerGammaHitsCollection* gammaCollection;

	double enRes = 0.0015;  // 1.5 keV
};

#endif
