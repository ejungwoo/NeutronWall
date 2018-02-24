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

//  channelIDs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103};
