//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "G4AnalysisManager.hh"
    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	sourceType = "152Eu"; // default source type
	// sourceType = "gamma"; // default source type

	//---Source position mode---//
	fPosMode = "implant"; // position mode "implant" or "fixed"

	//---Used in fixed mode---//
	fFixedPos = G4ThreeVector(0.0*cm, 0.0*cm, -2.425*cm); // default fixed position

	// z used in "implant" mode.
	// -2.425 cm : outer face of the implant box (same plane as your 13-point
	//             scan, so the result is directly comparable to the
	//             interpolated column of Weighted_efficiency.C)
	// Inside YSO: front face is at z = 0.625 - 0.6 = 0.025 cm, so for decays
	//             of implanted ions use e.g. 0.025*cm + implantDepth
	fImplantZ = 0.625*cm;

	if (fPosMode == "implant") {
		// Histogram 0..48 mm  ->  YSO -24..+24 mm, so centre = (24, 24) mm.
		// Histogram +x is assumed to be Geant4 +x (towards the clovers).
		fSampler = new ImplantProfileSampler("implantXY_bins.txt", 24.0, 24.0, 0.024);
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fSampler;
  //fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

	if(sourceType == "gamma"){
		G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName = "gamma";
		G4ParticleDefinition *theParticle = particleTable->FindParticle(particleName);

		//G4ThreeVector momdir(0.*cm, 0.*cm, 1.*cm);
		G4ThreeVector momdir = G4RandomDirection();
		  
		fParticleGun->SetParticleEnergy(121.3*keV);
		fParticleGun->SetParticleMomentumDirection(momdir);
		fParticleGun->SetParticleDefinition(theParticle);
  	}
	else if(sourceType == "152Eu"){

		G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(63, 152);
		// G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(27, 60);
		  
		fParticleGun->SetParticleEnergy(1*eV);
		fParticleGun->SetParticleDefinition(ion);
	}
	else{
		G4cout << "ERROR: UNKNOWN SOURCE TYPE. EXITING." << G4endl;
		exit(EXIT_FAILURE);
	}

	// G4ThreeVector pos(1.2*cm, 0.*cm, 0.625*cm);  // Halfway to edge in X
	//G4ThreeVector pos(0.*cm, 0.*cm, 0.925*cm);  // Halfway to edge in Z
	// G4ThreeVector pos(0.*cm, 0.*cm, 0.625*cm);  // Center of YSO
	//G4ThreeVector pos(sourceX, sourceY, sourceZ-0.5*mm);

	G4ThreeVector pos;
	if(fPosMode == "implant"){
		G4double x, y;
		fSampler->Sample(x, y);
		pos = G4ThreeVector(x, y, fImplantZ);
	}
	else{
		pos = fFixedPos;
	}
	// G4ThreeVector pos(0.0*cm, 0.0*cm, -2.425*cm);	//Change the position of the source

	fParticleGun->SetParticlePosition(pos);


	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

