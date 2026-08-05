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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);


  // Default physics
  //RegisterPhysics(new G4RadioactiveDecayPhysics());
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  //RegisterPhysics(new G4EmStandardPhysics());
  //RegisterPhysics(new G4EmStandardPhysics_option1());
  RegisterPhysics(new G4EmStandardPhysics_option4());
  //RegisterPhysics(new G4EmLivermorePhysics());
  //RegisterPhysics(new G4EmPenelopePhysics());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ 
}
/*
void PhysicsList::ConstructParticle()
{
	G4Gamma::GammaDefinition();
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();

	G4IonConstructor iConstructor;
  	iConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  // Define transportation process

  AddTransportation();
  ConstructEM();
}

void PhysicsList::ConstructEM()
{

  auto theParticleIterator=G4ParticleTable::GetParticleTable()->GetIterator();
  theParticleIterator->reset();
  
  
  while( (*theParticleIterator)() ){
	 
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
 

    //  G4cout<<"++++ Particle name ="<<particleName<<G4endl;
    if (particleName == "gamma") {
		// Standard EM 
		
      		//pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
     		//pmanager->AddDiscreteProcess(new G4ComptonScattering());
      		//pmanager->AddDiscreteProcess(new G4GammaConversion());
				
		// Variation for Low Energy Processes
		
	      	//pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
	      	//pmanager->AddDiscreteProcess(new G4LowEnergyPhotoElectric);
	      	//pmanager->AddDiscreteProcess(new G4LowEnergyCompton);
	      	//pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion);
		
		// Variation for Low Energy Penelope
		
 		//pmanager->AddDiscreteProcess(new G4PenelopeCompton);
 		//pmanager->AddDiscreteProcess(new G4PenelopeGammaConversion);
 		//pmanager->AddDiscreteProcess(new G4PenelopePhotoElectric);
 		//pmanager->AddDiscreteProcess(new G4PenelopeRayleigh); 		
		
               //--- High precision gammaray interation physics.  This physics was used for e12015 C17 analysis. Important for low energies.
   		//--- from G4examples advanced gammaray_telescope/src/GammaRayTelEMlowePhysics.cc
		
		//pmanager = G4Gamma::Gamma()->GetProcessManager();
		
		//G4RayleighScattering *theRayleigh = new G4RayleighScattering();
		//theRayleigh->SetEmModel( new G4LivermoreRayleighModel() );
		
		//G4PhotoElectricEffect *thePhotoElectricEffect = new G4PhotoElectricEffect();
		//thePhotoElectricEffect->SetEmModel( new G4LivermorePhotoElectricModel() );
		
		//G4ComptonScattering *theComptonScattering = new G4ComptonScattering();
		//theComptonScattering->SetEmModel( new G4LivermoreComptonModel() );
		
		//G4GammaConversion *theGammaConversion = new G4GammaConversion();
		//theGammaConversion->SetEmModel( new G4LivermoreGammaConversionModel() );
		
		//pmanager->AddDiscreteProcess( theRayleigh );
		//pmanager->AddDiscreteProcess( thePhotoElectricEffect );
		//pmanager->AddDiscreteProcess( theComptonScattering );
		//pmanager->AddDiscreteProcess( theGammaConversion );		
		
    }
    else if (particleName == "e-") {
		pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
		pmanager->AddProcess(new G4eIonisation,       -1,2,2);
		pmanager->AddProcess(new G4eBremsstrahlung,   -1,3,3);     
    }
    else if (particleName == "e+") {

		pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
		pmanager->AddProcess(new G4eIonisation,       -1,2,2);
		pmanager->AddProcess(new G4eBremsstrahlung,   -1,3,3);     
		pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4); 
    }
    else if(particleName == "GenericIon") 
		{
		  
		  //G4hMultipleScattering* aMultipleScattering = new G4hMultipleScattering();
		  //pmanager->AddProcess(aMultipleScattering,-1,1,1);
		  //G4ionIonisation* aIon = new G4ionIonisation();
		  //G4IonParametrisedLossModel* ionModel = new G4IonParametrisedLossModel();
  		  //aIon ->SetEmModel(ionModel);
		  //pmanager->AddProcess(aIon,        -1,2, 2);
		  //pmanager->AddProcess(new G4StepLimiter,        -1,-1, 4);
		  //pmanager->AddProcess(new Reaction(BeamOut),    -1,-1, 3);
		  
		  pmanager->AddProcess(new G4Decay,    -1,-1, 5);
		  
		
    }

  }

  
  AddRadioactiveDecay();

}

void PhysicsList::AddRadioactiveDecay()
{  
  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();

  //G4Radioactivation* radioactiveDecay = new G4Radioactivation();

  G4bool ARMflag = true;
  radioactiveDecay->SetARM(ARMflag);                //Atomic Rearangement

  // need to initialize atomic deexcitation
  //
  G4LossTableManager* man = G4LossTableManager::Instance();
  G4VAtomDeexcitation* deex = man->AtomDeexcitation();
  if (!deex) {
     ///G4EmParameters::Instance()->SetFluo(true);
     G4EmParameters::Instance()->SetAugerCascade(ARMflag);
     G4EmParameters::Instance()->SetDeexcitationIgnoreCut(ARMflag);    
     deex = new G4UAtomicDeexcitation();
     deex->InitialiseAtomicDeexcitation();
     man->SetAtomDeexcitation(deex);
  }
  
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());

  // mandatory for G4NuclideTable
  //
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);

  //printout
  //
  G4cout << "\n  Set atomic relaxation mode " << ARMflag << G4endl;
}
*/

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();

  
  /*
  SetCutValue(1 * nm, "gamma");
  SetCutValue(1 * nm, "e-");
  SetCutValue(1 * nm, "e+");
  
  SetCutValue(1 * keV, "gamma");
  SetCutValue(1 * keV, "e-");
  SetCutValue(1 * keV, "e+");
  */
}  
