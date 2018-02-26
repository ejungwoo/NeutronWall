#ifndef NWEVENTBUILDTASK_HH
#define NWEVENTBUILDTASK_HH

#include "KBTask.hh"
#include "TClonesArray.h"
#include "TChain.h"

#include "NWDetector.hh"
#include "NWChannelMap.hh"

#include "DataStruct.hh"

class NWEventBuildTask : public KBTask
{ 
  public:
    NWEventBuildTask();
    virtual ~NWEventBuildTask() {};

    bool Init();
    void Exec(Option_t*);

    void SetRunName(TString name) { fRunName = name; }

  private:
    TClonesArray* fBarArray;

    TString fRunName;

    TTree* fTreeCh[112];
    FADC*  fFADCCh[112];
    Long64_t fEntryCh[112];
    Long64_t fNumEntriesCh[112];

    TTree* fTreeTCB;
    TCB* fTCB;
    Long64_t fEntryTCB;
    Long64_t fNumEvents;

    NWChannelMap *fMap;

  ClassDef(NWEventBuildTask, 1)
};

#endif
