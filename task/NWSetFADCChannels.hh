#ifndef NWSETCHANNELS_HH
#define NWSETCHANNELS_HH

#include "KBTask.hh"
#include "TClonesArray.h"
#include "TChain.h"

#include "NWDetector.hh"

class NWSetFADCChannels : public KBTask
{ 
  public:
    NWSetFADCChannels();
    NWSetFADCChannels(const char *treeName = "EventTree");
    virtual ~NWSetFADCChannels() {};

    bool Init();
    void Exec(Option_t*);

    void SetVerbosity(Int_t verbose);

    void CreateTree();
    void AddData(TString fileName);
    void InitTree();

  private:
    NWDetector *fNW;

    TObjArray* fMCStepArrayArray;
    TClonesArray* fChannelArray;

    Int_t fVerbosity = 0;

    TString fTreeName = "EventTree";
    TChain *fDataTree = nullptr;

    Int_t fNumChannels;
    Int_t fChannelIDs[112];//TODO
    Short_t fRawADC[112][240]; //TODO
    Int_t fNumTbs = 240; //TODO

  ClassDef(NWSetFADCChannels, 1)
};

#endif
