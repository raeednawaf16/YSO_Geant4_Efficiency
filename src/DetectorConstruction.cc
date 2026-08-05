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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  

	// MATERIALS

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	// First, elements
	G4Element* H = nist->FindOrBuildElement("H");
	G4Element* C = nist->FindOrBuildElement("C");
	G4Element* N = nist->FindOrBuildElement("N");
	G4Element* O = nist->FindOrBuildElement("O");
	G4Element* Si = nist->FindOrBuildElement("Si");
	G4Element* Y = nist->FindOrBuildElement("Y");
	G4Element* Al = nist->FindOrBuildElement("Al");
	G4Element* Pb = nist->FindOrBuildElement("Pb");

	// World
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

	// YSO
	G4Material* matY2SiO5 = new G4Material("Y2SiO5", 4.4*g/cm3, 3);  //original density = 4.4*g/cm3

	matY2SiO5->AddElement(Y, 2);
	matY2SiO5->AddElement(Si, 1);
	matY2SiO5->AddElement(O, 5);

	// Borosilicate glass
	G4Material* matSiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
	G4Material* matB2O3 = nist->FindOrBuildMaterial("G4_BORON_OXIDE");
	G4Material* matNa2O = nist->FindOrBuildMaterial("G4_SODIUM_MONOXIDE");
	G4Material* matAl2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");   

	// Specs for borosilicate glass from borosilscientific.com. Original density = 2.23*g/cm3
	G4Material *matBorosilicateGlass = new G4Material("BorosilicateGlass", 2.23*g/cm3, 4);
	matBorosilicateGlass->AddMaterial(matSiO2, 80.60*perCent);
	matBorosilicateGlass->AddMaterial(matB2O3, 13.00*perCent);
	matBorosilicateGlass->AddMaterial(matNa2O, 4.0*perCent);
	matBorosilicateGlass->AddMaterial(matAl2O3, 2.30*perCent); 

	// Vacuum - temperature, density, pressure are very rough estimates. should revisit
	G4Material* matVacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material* matPSPMT = nist->BuildMaterialWithNewDensity("matPSPMT","G4_Al",1.*g/cm3);  

	// ABS plastic - unsure about the structure of this material
	G4Material* matABS = new G4Material("ABSPlastic",  1.07*g/cm3, 3);
	matABS->AddElement(C, 15);
	matABS->AddElement(H, 17);
	matABS->AddElement(N, 1);

	G4Material *Al_dense = new G4Material("Al_dense", 13, 207.2*g/mole, 11.35*g/cm3);

	G4Material *matAl = nist->FindOrBuildMaterial("G4_Al");
  
	// Origin defined as center of YSO 
	G4double originx = 0*cm, originy = 0, originz = 0.75*cm-0.125*cm;
	   
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;

	//     
	// World
	//
	G4double world_sizeXY = 84*cm;
	G4double world_sizeZ  = 84*cm;
	  
	G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ); 
	     
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
		                           
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

	//     
	// Implant YSO
	//

	// Box shape
	// original dxy = 4.8 cm
	G4double yso_dxy = 4.8*cm, yso_dz = 1.2*cm;    
	G4Box* yso_solid = new G4Box("YSO", 0.5*yso_dxy, 0.5*yso_dxy, 0.5*yso_dz);                       
	yso_logic = new G4LogicalVolume(yso_solid, matY2SiO5, "YSO");
		     
	new G4PVPlacement(0, G4ThreeVector(originx, originy, originz), yso_logic, "YSO", logicWorld, false,0, checkOverlaps);
 
	//
	// Implant box outer casing
	//

	// Very approximate dimensions here
	G4double box_dxy = 5.6*cm, box_dz = 9.0*cm, box_wall = 0.2*cm;
	G4Box* box_outer_solid = new G4Box("BoxOuter", 0.5*box_dxy, 0.5*box_dxy, 0.5*box_dz);
	G4Box* box_inner_solid = new G4Box("BoxInner", 0.5*(box_dxy-2*box_wall), 0.5*(box_dxy-2*box_wall), 0.5*(box_dz-2*box_wall));
	G4VSolid* box_solid = new G4SubtractionSolid("Box", box_outer_solid, box_inner_solid, 0, G4ThreeVector());

	G4LogicalVolume* box_logic = new G4LogicalVolume(box_solid, matABS, "Box");

	double boxZ = -2.25*cm+originz+0.5*box_dz-0.5*yso_dz-box_wall;
	if(boxZ - 0.5*box_dz != sourceZ)
		G4cout << "WARNING: SOURCE IS NOT ON FACE OF IMPLANT BOX" << G4endl;

	new G4PVPlacement(0, G4ThreeVector(originx, originy, boxZ), box_logic, "Box", logicWorld, false, 0, checkOverlaps); 

	// Plastic block connectors on top & bottom of casing
	/*
	G4double connector_dx = 2.8*cm, connector_dy = 2.8*cm, connector_dz = 1.*cm;
	G4Box *connector_solid = new G4Box("Connector", 0.5*connector_dx, 0.5*connector_dy, 0.5*connector_dz);
	 
	G4LogicalVolume* connector_logic = new G4LogicalVolume(connector_solid, matABS, "Connector");

	G4double connector_xpos[] = {0, 0, 0, 0};
	G4double connector_ypos[] = {0.5*box_dxy+0.5*connector_dy, 0.5*box_dxy+0.5*connector_dy, -(0.5*box_dxy+0.5*connector_dy), -(0.5*box_dxy+0.5*connector_dy)};
	G4double connector_zpos[] = {-0.2*box_dz, 0.2*box_dz, -0.2*box_dz, 0.2*box_dz};

	new G4PVPlacement(0, G4ThreeVector(connector_xpos[0], connector_ypos[0], connector_zpos[0]), connector_logic, "Connector0", box_logic, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(connector_xpos[1], connector_ypos[1], connector_zpos[1]), connector_logic, "Connector1", box_logic, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(connector_xpos[2], connector_ypos[2], connector_zpos[2]), connector_logic, "Connector2", box_logic, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(connector_xpos[3], connector_ypos[3], connector_zpos[3]), connector_logic, "Connector3", box_logic, false, 0, checkOverlaps);
	*/

	// Source housing
	
	G4double discZ = 1*mm, discRad = 0.5*mm;

	G4Tubs *source_disc_solid = new G4Tubs("SourceDisc", 0, discRad, 0.5*discZ, 0*deg, 360*deg);

	G4LogicalVolume* source_disc_logic = new G4LogicalVolume(source_disc_solid, matAl, "SourceDisc");

	//new G4PVPlacement(0, G4ThreeVector(sourceX, sourceY, sourceZ-0.5*discZ), source_disc_logic, "SourceDisc", logicWorld, false, 0, checkOverlaps);

	// Clover detector

	// 4, 5, 6 original: 10.67*cm. 8, 9 original: 19.05*cm. 
	// 10, 11, 12 original: 14.48*cm, 19.05*cm, 14.48*cm
	G4double psiArr[] = {19.05*cm, 19.05*cm, 19.05*cm, 19.05*cm, 12.67*cm, 12.67*cm, 12.67*cm, 10.69*cm, 21.55*cm, 21.55*cm, 15.48*cm, 20.05*cm, 15.48*cm};

	G4double phiArr[] = {315*deg, 270*deg, 224.03*deg, 180*deg, 334.29*deg, 282.86*deg, 231.43*deg, 180*deg, 290*deg, 249.99*deg, 334*deg, 270*deg, 206*deg};

	G4double thetaArr[] = {123*deg, 123*deg, 122.56*deg, 123*deg, 90*deg, 90*deg, 90*deg, 90*deg, 60*deg, 60*deg, 48*deg, 25*deg, 48*deg};

	for(int i = 0; i < nclovers; i++){
		cloverArray[i] = new Clover(logicWorld);
		cloverArray[i]->Construct(psiArr[i], phiArr[i], thetaArr[i], i);
	}
  
	//     
	// Quartz diffuser
	//

	// Box shape
	G4double quartz_dxy = 4.8*cm, quartz_dz = 0.2*cm;    
	G4Box* quartz_solid = new G4Box("Quartz", 0.5*quartz_dxy, 0.5*quartz_dxy, 0.5*quartz_dz);
		              
	G4LogicalVolume* quartz_logic = new G4LogicalVolume(quartz_solid, matSiO2, "Quartz");
		       
	new G4PVPlacement(0, G4ThreeVector(originx, originy, originz+0.5*yso_dz+0.5*quartz_dz), quartz_logic, "Quartz", logicWorld, false, 0, checkOverlaps);

	//
	// PSPMT - Hamamatsu H12700
	//

	// Glass box
	// dz is a very rough guess
	G4double window_dxy = 5.1*cm, window_dz = 3.5*cm, window_wall = 5.*mm;    
	G4Box* window_solid = new G4Box("Window", 0.5*window_dxy, 0.5*window_dxy, 0.5*window_dz);
		              
	G4LogicalVolume* window_logic = new G4LogicalVolume(window_solid, matPSPMT, "Window"); 
		       
	new G4PVPlacement(0, G4ThreeVector(originx, originy, originz + 0.5*yso_dz+quartz_dz+0.5*window_dz), window_logic, "Window", logicWorld, false, 0, checkOverlaps);

	// Vacuum chamber inside box
	/*
	G4Box* vacuum_solid = new G4Box("Vacuum", 0.5*(window_dxy - 2*window_wall), 0.5*(window_dxy - 2*window_wall), 0.5*(window_dz - 2*window_wall));

	G4LogicalVolume* vacuum_logic = new G4LogicalVolume(vacuum_solid, matVacuum, "Vacuum");

	new G4PVPlacement(0, G4ThreeVector(), vacuum_logic, "Vacuum", window_logic, false, 0, checkOverlaps);
	*/

	//
	//always return the physical World
	//
	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{ 
  
	
	TrackerGammaSD* cloverSD = new TrackerGammaSD("cloverSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(cloverSD);

	for(int i = 0; i < nclovers; i++){
		cloverArray[i]->MakeSensitive(cloverSD);
	}
	
	/*
	TrackerGammaSD* YSOSD = new TrackerGammaSD("YSOSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(YSOSD);
	yso_logic->SetSensitiveDetector(YSOSD);
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
