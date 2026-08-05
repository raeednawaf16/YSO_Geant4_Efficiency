#ifndef TRACKERGAMMAHIT_HH
#define TRACKERGAMMAHIT_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4VHit.hh"

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

class TrackerGammaHit : public G4VHit
{
public:
	TrackerGammaHit();
	//TrackerGammaHit(const TrackerGammaHit&);
	~TrackerGammaHit();

	// operators
	//const TrackerGammaHit& operator=(const TrackerGammaHit&);
	//G4bool operator==(const TrackerGammaHit&) const;

	//inline void* operator new(size_t);
	//inline void operator delete(void*);

	// methods from base class
	void Draw();
	void Print();

	// set methods
	void SetTrackID(G4int track) {fTrackID = track;};
	void SetEdep(G4double de) {fEdep = de;};
	void SetPos(G4ThreeVector xyz){pos = xyz;};
	void SetDetID(G4int id){fDetID = id;};
	void IncNCompton(){fNCompton++;};

	// get methods
	G4int GetTrackID() {return fTrackID;};
	G4double GetEdep() {return fEdep;};
	G4ThreeVector GetPos() {return pos;};
	G4int GetDetID() {return fDetID;};
	G4int GetNCompton() {return fNCompton;};

private:
	G4int fTrackID = -1;
	G4double fEdep = 0.;
	G4ThreeVector pos;
	G4int fDetID = -1;
	G4int fNCompton = 0;

};

typedef G4THitsCollection<TrackerGammaHit> TrackerGammaHitsCollection;

/*
extern G4Allocator<TrackerGammaHit> TrackerGammaHitAllocator;

inline void* TrackerGammaHit::operator new(size_t)
{
	void *aHit;
	aHit = (void *) TrackerGammaHitAllocator.MallocSingle();
	return aHit;
}

inline void TrackerGammaHit::operator delete(void* aHit)
{
	TrackerGammaHitAllocator.FreeSingle((TrackerGammaHit*) aHit);
}
*/
#endif
