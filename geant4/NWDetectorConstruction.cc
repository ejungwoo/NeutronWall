#include "NWDetectorConstruction.hh"
#include "KBG4RunManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "TString.h"

NWDetectorConstruction::NWDetectorConstruction()
: G4VUserDetectorConstruction()
{
}

NWDetectorConstruction::~NWDetectorConstruction()
{
}

G4VPhysicalVolume* NWDetectorConstruction::Construct()
{  
  auto runManager = (KBG4RunManager *) G4RunManager::GetRunManager();
  auto par = runManager -> GetParameterContainer();
  auto nist = G4NistManager::Instance();

  // -----------------------------------------------------
  // World

  auto materialWorld = nist -> FindOrBuildMaterial("G4_AIR");
  auto sizeOfWorld = par -> GetParDouble("worldSize") * mm;

  auto solidWorld = new G4Box("World", 0.5*sizeOfWorld, 0.5*sizeOfWorld, 0.5*sizeOfWorld);
  auto logicWorld = new G4LogicalVolume(solidWorld, materialWorld, "World");
  auto physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, -1, true);

  // -----------------------------------------------------
  // Neturon Wall

  auto numWalls = par -> GetParInt("numWalls");
  for (auto iWall = 0; iWall < numWalls; ++iWall)
  {
    // Position of first detector copy
    auto xDetector  = par -> GetParDouble(Form("xDetector%d", iWall)) * mm; 
    auto yDetector  = par -> GetParDouble(Form("yDetector%d", iWall)) * mm;
    auto zDetector  = par -> GetParDouble(Form("zDetector%d", iWall)) * mm;

    // Distance between detectors (in same wall) in x,y,z
    auto dxDetector = par -> GetParDouble(Form("dxDetector%d", iWall)) * mm;
    auto dyDetector = par -> GetParDouble(Form("dyDetector%d", iWall)) * mm;
    auto dzDetector = par -> GetParDouble(Form("dzDetector%d", iWall)) * mm;

    // Length of detector in x,y,z
    auto lxDetector = par -> GetParDouble(Form("lxDetector%d", iWall)) * mm;
    auto lyDetector = par -> GetParDouble(Form("lyDetector%d", iWall)) * mm;
    auto lzDetector = par -> GetParDouble(Form("lzDetector%d", iWall)) * mm;

    auto nameOfMaterial = par -> GetParString(Form("nameOfMaterial%d", iWall));
    auto material = nist -> FindOrBuildMaterial(nameOfMaterial.Data());

    auto nameSolid = TString(Form("DetectorWall%d", iWall));
    auto solidDetector = new G4Box(nameSolid.Data(), 0.5*lxDetector, 0.5*lyDetector, 0.5*lzDetector);
    auto logicDetector = new G4LogicalVolume(solidDetector, material, nameSolid.Data());

    auto numDetectors = par -> GetParInt(Form("numDetectors%d", iWall));
    for (auto iCopy = 0; iCopy < numDetectors; ++iCopy)
    {
      auto nameCopy = TString(Form("DetectorWall%d_Copy%d", iWall, iCopy));
      auto copyNumber = 100*iWall + iCopy;
      auto offset = G4ThreeVector(xDetector + iCopy*dxDetector, yDetector + iCopy*dyDetector, zDetector + iCopy*dzDetector);
      auto pvp = new G4PVPlacement(0, offset, logicDetector, nameCopy.Data(), logicWorld, false, copyNumber, true);
      runManager -> SetSensitiveDetector(pvp);
    }
  }

  return physiWorld;
}
