#ifndef NWDETECTOR_HH
#define NWDETECTOR_HH

#include "KBDetector.hh"

class NWDetector : public KBDetector
{
  public:
    NWDetector();
    virtual ~NWDetector() {};

    virtual bool Init();

  protected:
    bool BuildGeometry();
    bool BuildDetectorPlane();

  ClassDef(NWDetector, 1)
};

#endif
