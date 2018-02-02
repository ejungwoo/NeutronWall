#ifndef NWDETECTORPLANE_HH
#define NWDETECTORPLANE_HH

#include "KBDetectorPlane.hh"

class NWDetectorPlane : public KBDetectorPlane
{
  public:
    NWDetectorPlane();
    virtual ~NWDetectorPlane() {}

    virtual bool Init();
    virtual bool IsInBoundary(Double_t i, Double_t j);
    virtual Int_t FindChannelID(Double_t i, Double_t j);
    virtual TCanvas *GetCanvas(Option_t *option = "");
    virtual void DrawFrame(Option_t *option = "");
    virtual TH2* GetHist(Option_t *option = "");

  public:
    void IsTopReadout(bool val = true);

  private:
    bool fIsTopReadout = true;

    Int_t fNumChannels = 0;
    Int_t fNumTimeBuckets = 0;
    Int_t fNumTimeBucketsLite = 0;

    Short_t fRefNormal  = -1;
    Short_t fRefReadout = -1;
    Short_t fRefSide    = -1;

    Double_t fLowLimitI  = 0;
    Double_t fLowLimitJ  = 0;
    Double_t fHighLimitI = 0;
    Double_t fHighLimitJ = 0;

  ClassDef(NWDetectorPlane, 1)
};

#endif
