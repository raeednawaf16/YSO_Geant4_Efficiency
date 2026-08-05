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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	sourceType = "gamma";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
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
		  
		fParticleGun->SetParticleEnergy(1408.5*keV);
		fParticleGun->SetParticleMomentumDirection(momdir);
		fParticleGun->SetParticleDefinition(theParticle);
  	}
	else if(sourceType == "152Eu"){

		G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(63, 152);
		  
		fParticleGun->SetParticleEnergy(1*eV);
		fParticleGun->SetParticleDefinition(ion);
	}
	else{
		G4cout << "ERROR: UNKNOWN SOURCE TYPE. EXITING." << G4endl;
		exit(EXIT_FAILURE);
	}

	//G4ThreeVector pos(1.2*cm, 0.*cm, 0.625*cm);  // Halfway to edge in X
	//G4ThreeVector pos(0.*cm, 0.*cm, 0.925*cm);  // Halfway to edge in Z
	G4ThreeVector pos(0.*cm, 0.*cm, 0.625*cm);  // Center of YSO
	//G4ThreeVector pos(sourceX, sourceY, sourceZ-0.5*mm);
	//G4ThreeVector pos(2.5*cm, 2.5*cm, 0.*cm);
	fParticleGun->SetParticlePosition(pos);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

