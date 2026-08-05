#include "Constants.hh"

// AAD: I attempted to write code here to define materials globably and clear some warning messages, but it never worked. I'm leaving this commented, broken code in here in case anyone wants to revisit this and try to get it to work.

/*
Constants:: Constants()
{

	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
	myMaterials.push_back(world_mat);
	
	//G4Material* Al_dense = nist->BuildMaterialWithNewDensity("Al_dense","G4_Al",11.35*g/cm3);
	//myMaterials.push_back(Al_dense);
	
}

Constants::~ Constants()
{;}
*/

/*
G4Material*  Constants::FindMaterial(G4String materialName)
{

	int len = myMaterials.size();
	G4Material* pttoMaterial;

	for(int i = 0; i < len; i++){
		if(myMaterials[i]->GetName() == materialName){
			pttoMaterial = myMaterials[i];
			break;
		}
		else if(myMaterials[i]->GetName() != materialName && i == len){
			G4cout << "ERROR: CANNOT FIND MATERIAL " << materialName << G4endl;
		}
	}

	// search the material by its name 
	//G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  

	return pttoMaterial;
  
}
*/
