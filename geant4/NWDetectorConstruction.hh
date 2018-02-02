#ifndef NWDETECTORCONSTRUCTION_HH 
#define NWDETECTORCONSTRUCTION_HH 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class NWDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    NWDetectorConstruction();
    virtual ~NWDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
};

#endif
