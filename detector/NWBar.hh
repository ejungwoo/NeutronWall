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

    void SetLeft(NWChannel *val) { fLeft = val; }
    NWChannel *GetLeft() { return fLeft; }

    void SetRight(NWChannel *val) { fRight = val; }
    NWChannel *GetRight() { return fRight; }

    void SetBarID(Int_t val) { fBarID = val; }
    Int_t GetBarID() { return fBarID; }

    void SetGeoMean(Double_t val) { fGeoMean = val; }
    Double_t GetGeoMean() { return fGeoMean; }

    void SetWall(bool val) { fWallID = (val?1:0); } ///< 1:A 0:B
    void SetWallID(Int_t val) { fWallID = val; } ///< 1:A 0:B
    Int_t GetWallID() { return fWallID; }

  private:
    NWChannel *fLeft = nullptr; //->
    NWChannel *fRight = nullptr; //->

    Int_t fWallID; ///< 1:A 0:B
    Int_t fBarID;
    Double_t fGeoMean;

  ClassDef(NWBar, 1)
};

#endif
