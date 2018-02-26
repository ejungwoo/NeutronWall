#ifndef NWEVENTANATASK_HH
#define NWEVENTANATASK_HH

#include "KBTask.hh"
#include "TClonesArray.h"
#include "NWPSATask.hh"

class NWEventAnaTask : public KBTask
{ 
  public:
    NWEventAnaTask();
    virtual ~NWEventAnaTask() {};

    bool Init();
    void Exec(Option_t*);

  private:
    TClonesArray* fBarArray;

    NWPSATask *fPSATask;

  ClassDef(NWEventAnaTask, 1)
};

#endif
