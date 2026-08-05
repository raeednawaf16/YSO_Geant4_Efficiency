#include "TrackerGammaHit.hh"

//G4Allocator<TrackerGammaHit> TrackerGammaHitAllocator;

TrackerGammaHit::TrackerGammaHit() {}

TrackerGammaHit::~TrackerGammaHit() {}


void TrackerGammaHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if (pVVisManager) {
		G4Circle circle(pos);
		circle.SetScreenSize(4.);
		circle.SetFillStyle(G4Circle::filled);
		G4VisAttributes attribs(G4Colour::Red());
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}

void TrackerGammaHit::Print()
{
	G4cout << "\ttrackID: " << fTrackID << " Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy") << " Position: " << std:: setw(7) << G4BestUnit(pos, "Length") << " DetID: " << fDetID << G4endl;
}

