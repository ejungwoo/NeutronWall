#ifndef NWDIGITASK_HH
#define NWDIGITASK_HH

#include "KBTask.hh"
#include "TClonesArray.h"

class NWDigiTask : public KBTask
{ 
  public:
    NWDigiTask();
    virtual ~NWDigiTask() {};

    bool Init();
    void Exec(Option_t*);

  private:
    TObjArray* fMCStepArrayArray;
    TClonesArray* fChannelArray;

  ClassDef(NWDigiTask, 1)
};

#endif
