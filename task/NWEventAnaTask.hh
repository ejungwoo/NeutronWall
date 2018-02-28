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

    void SetFit(bool val) { fFitFlag = val; }
    void SetReuseBranch(bool val) { fReuseBranchFlag = val; }

  private:
    TClonesArray* fBarArray;
    TClonesArray* fBarArrayNew;

    NWPSATask *fPSA;

    bool fFitFlag = true;
    bool fReuseBranchFlag = true;

  ClassDef(NWEventAnaTask, 1)
};

#endif
