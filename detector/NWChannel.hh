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


  private:
    Int_t fModuleID;
    Int_t fMChannelID;

    Double_t fPedestal;     ///< Pedestal value with ADC
    Double_t fTDC;          ///< TPC at peak position
    Double_t fADC;          ///< ADC at peak position
    Double_t fADCTotalSum;  ///< total sum of ADC (slow component)
    Double_t fADCPartSum;   ///< part sum of ADC (fast component)

  ClassDef(NWChannel, 1)
};

#endif
