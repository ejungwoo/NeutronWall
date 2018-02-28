#ifndef NWCHANNEL_HH
#define NWCHANNEL_HH

#include "KBChannelBufferS.hh"

class NWChannel: public KBChannelBufferS
{
  public:
    NWChannel() { Clear(); };
    virtual ~NWChannel() {}

    virtual void Clear(Option_t *option = "");
    virtual void Copy (TObject &object) const;
    virtual void Print(Option_t *option = "") const; 

    void SetModuleID(Int_t val) { fModuleID = val; }
    Int_t GetModuleID() { return fModuleID; }

    void SetMChannelID(Int_t val) { fMChannelID = val; }
    Int_t GetMChannelID() { return fMChannelID; }


    void SetPedestal(Double_t val) { fPedestal = val; }
    Double_t GetPedestal() { return fPedestal; }

    void SetTDC(Double_t val) { fTDC = val; }
    Double_t GetTDC() { return fTDC; }

    void SetADC(Double_t val) { fADC = val; }
    Double_t GetADC() { return fADC; }

    void SetADCTotalSum(Double_t val) { fADCTotalSum = val; }
    Double_t GetADCTotalSum() { return fADCTotalSum; }

    void SetADCPartSum(Double_t val) { fADCPartSum = val; }
    Double_t GetADCPartSum() { return fADCPartSum; }


    void SetIsFitted(bool val = true) { fIsFitted = val; }
    bool GetIsFitted() { return fIsFitted; }

    void SetPosition(Double_t val) { fPosition = val; }
    Double_t GetPosition() { return fPosition; }

    void SetAlpha(Double_t val) { fAlpha = val; }
    Double_t GetAlpha() { return fAlpha; }

    void SetChiSquare(Double_t val) { fChiSquare = val; }
    Double_t GetChiSquare() { return fChiSquare; }

  private:
    Int_t fModuleID;
    Int_t fMChannelID;

    Double_t fPedestal;     ///< Pedestal value with ADC
    Double_t fTDC;          ///< TPC at peak position
    Double_t fADC;          ///< ADC at peak position
    Double_t fADCTotalSum;  ///< total sum of ADC (slow component)
    Double_t fADCPartSum;   ///< part sum of ADC (fast component)

    bool fIsFitted = false;
    Double_t fPosition;
    Double_t fAlpha;
    Double_t fChiSquare;

  ClassDef(NWChannel, 1)
};

#endif
