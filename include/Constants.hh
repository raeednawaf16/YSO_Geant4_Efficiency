#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

const int nclovers = 13; 
const int ncrystals = 4*nclovers;
const double sourceX = -1.2*cm;
const double sourceY = 1.2*cm;
const double sourceZ = -2.3*cm-0.125*cm; // actually the "inner edge" of the source disc

#endif
