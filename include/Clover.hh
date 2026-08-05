#ifndef CLOVER_HH
#define CLOVER_HH

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4AssemblyVolume.hh"

#include "G4RotationMatrix.hh"
#include "globals.hh"

#include "TrackerGammaSD.hh"
#include "Constants.hh"

class Clover
{
public:

	G4LogicalVolume *worldLV;

	Clover(G4LogicalVolume*);
	~Clover();

	void Construct(G4double, G4double, G4double, G4int);

	void setX(G4double x){ DetPos.setX(x); };
	void setY(G4double y){ DetPos.setX(y); };
	void setZ(G4double z){ DetPos.setX(z); };

	void setTheta(G4double th){ DetTheta = th; };
	void setPhi(G4double ph){ DetPhi = ph; };
	void setPsi(G4double psi){ DetPsi = psi; };

	void setCode(G4int c){ DetCode = c; };

	void MakeSensitive(TrackerGammaSD*);

private:

	G4IntersectionSolid* Bulletized_Boxed_Cylinder(G4double);

	G4int DetCode;

	// Logical volumes
	G4LogicalVolume *leaf_log;
	G4LogicalVolume *cover_log;
	G4LogicalVolume *cover_log_dense;
	G4LogicalVolume *Cubox_log;

	G4AssemblyVolume* assemblyclover;

	// position
	G4ThreeVector Pos0;
	G4ThreeVector DetPos;
	G4double DetTheta;
	G4double DetPhi;
	G4double DetPsi;
	G4double thetad;
	G4RotationMatrix Rot0;
	G4RotationMatrix DetRot;
	G4double LeafShift;	
	G4ThreeVector Leaf0Shift;
	G4ThreeVector Leaf1Shift;
	G4ThreeVector Leaf2Shift;
	G4ThreeVector Leaf3Shift;
	G4ThreeVector Leaf0Pos;
	G4ThreeVector Leaf1Pos;
	G4ThreeVector Leaf2Pos;
	G4ThreeVector Leaf3Pos;
	G4RotationMatrix wallrot;
	G4ThreeVector covershift;
	G4ThreeVector coverpos;
	G4ThreeVector cornershift;
	G4ThreeVector cornerpos;
	G4ThreeVector corner1shift;
	G4ThreeVector corner1pos;
	G4ThreeVector corner2shift;
	G4ThreeVector corner2pos;
	G4ThreeVector corner3shift;
	G4ThreeVector corner3pos;
	G4ThreeVector Cuboxshift;
	G4ThreeVector Cuboxpos;

	// Materials
	G4Material* HpGe;
	G4Material* Ge;
	G4Material* Al;
	G4Material* Cu;
	G4Material* Al_dense;
	G4Material* mat_vac;
	G4Material* Pb;
	G4Material* Fe;

	// dimensions
	G4double Length;
	G4double Radius;
	G4double boxlength;
	G4double torusradius;
	G4double covergap;
	G4double CCoffset;
	G4double CCradius;
	G4double CCdepth;
	G4double coaxialDLThickness;
	G4double outerDLThickness;
	G4double startAngle;
	G4double spanningAngle;
	G4double wallZoffset;
	G4double coverlength;
	G4double coverwidth;
	G4double coverthickness;
	G4double cornerRadius;
	G4double corneroffset;
	G4double Cuboxlength;

	G4Tubs* CCsub;
	G4Box* boxout;
	G4Box* boxin;
	G4SubtractionSolid* cover;
	G4Tubs* cornerCut;
	G4SubtractionSolid* coversub;
	G4Tubs* corner;
	G4UnionSolid* coveru;
	G4Box* Cubox;
	G4Tubs* CuboxCut;
	G4SubtractionSolid* CuboxCut1;
	G4SubtractionSolid* CuboxCut2;
	G4SubtractionSolid* CuboxCut3;
	G4SubtractionSolid* CuboxCut4;

};

#endif
