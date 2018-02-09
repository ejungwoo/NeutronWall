#ifndef NWBAR_HH
#define NWBAR_HH

#include "NWChannel.hh"

class NWBar : public TObject
{
  public:
    NWBar() { Clear(); };
    virtual ~NWBar() {}

    virtual void Clear(Option_t *option = "");
    virtual void Copy (TObject &object) const;
    virtual void Print(Option_t *option = "") const;

    void SetChannelLeft(NWChannel *val) { fLeft = val; }
    NWChannel *GetChannelLeft() { return fLeft; }

    void SetChannelRight(NWChannel *val) { fRight = val; }
    NWChannel *GetChannelRight() { return fRight; }

    void SetBarID(Int_t val) { fBarID = val; }
    Int_t GetBarID() { return fBarID; }

    void SetGeoMean(Double_t val) { fGeoMean = val; }
    Double_t GetGeoMean() { return fGeoMean; }

  private:
    NWChannel *fLeft = nullptr;
    NWChannel *fRight = nullptr;

    Int_t fBarID;
    Double_t fGeoMean;

  ClassDef(NWBar, 1)
};

#endif
