#ifndef NWDOSOMETHINGTASK_HH
#define NWDOSOMETHINGTASK_HH

#include "KBTask.hh"
#include "TClonesArray.h"

class NWDoSomethingTask : public KBTask
{ 
  public:
    NWDoSomethingTask();
    virtual ~NWDoSomethingTask() {};

    bool Init();
    void Exec(Option_t*);

  private:
    TClonesArray* fChannelArray;

  ClassDef(NWDoSomethingTask, 1)
};

#endif
