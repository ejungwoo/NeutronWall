#ifndef NWPSATASK_HH
#define NWPSATASK_HH

#define kNWPSA_NDATA_MAX 512

#include "KBTask.hh"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TLine.h"

class NWPSATask : public KBTask
{ 
  public:
    NWPSATask();
    virtual ~NWPSATask() {};

    bool Init();
    void Clear(Option_t *option="");
    void Draw(Option_t *option="");
    void Exec(Option_t*);

    bool Exec(const Short_t  *adc, Int_t n);
    bool Exec(      Short_t  *adc, Int_t n);
    bool Exec(const Double_t *adc, Int_t n);
    bool Exec(      Double_t *adc, Int_t n);

    bool Exec(TArrayS *tarray);
    bool Exec(TArrayD *tarray);

    void SetFit(bool val) { fFitFlag = val; }
    void SetUseShortRange(bool val) { fUseShortRange = val; }

    void SetAlphaLL(Double_t val = 0) { fAlphaLL = (val > 0) * val; }

  // Getters
  public:
            TH1D *H() { return CopyOfHistogram(); }   ///< Get copy of histogram of fData
    TGraphErrors *G() { return CopyOfFitSample(); }   ///< Get copy of TGraphErrors of sample points
             TF1 *F() { return CopyOfFitFunction(); } ///< Get copy of fitted function
         TLegend *L(Option_t *opt = "hgf") { return MakeLegend(opt); } ///< make and get legend

            TH1D *CopyOfHistogram(TString name = ""); ///< Get copy of histogram of fData
    TGraphErrors *CopyOfFitSample();                  ///< Get copy of TGraphErrors of sample points
             TF1 *CopyOfFitFunction();                ///< Get copy of fitted function
         TLegend *MakeLegend(Option_t *opt);        ///< Get legend of basic and fit information

       Int_t GetN()        { return fN; }        ///< Get size of data
    Double_t *GetData()    { return fData; }     ///< Get data[]
    Double_t GetPedestal() { return fPedestal; } ///< Get pedestal value

       Int_t GetPosBeforeThreshold() { return fPosBeforeThreshold; } ///< Get position where data is just below threshold
    Double_t GetThreshold()          { return fThreshold; }          ///< Get threshold value
    Double_t GetThresholdLL()        { return fThresholdLL; }        ///< Get threshold low limit
    Double_t GetThresholdRatio()     { return fThresholdRatio; }     ///< Get threshold ratio (threshold/max)

       Int_t GetPosMax() { return fPosMax; } ///< Get position at max value
    Double_t GetMax()    { return fMax; }    ///< Get max value

    Double_t GetTotalSum() { return fTotalSum; } ///< Get total sum of all ADC values
    Double_t GetPartSum()  { return fPartSum; }  ///< Get part  sum of all ADC values

    Double_t GetFitAmplitude()  { return fFitFunction -> GetParameter(0); }
    Double_t GetFitPosition()   { return fFitFunction -> GetParameter(1); }
    Double_t GetAlphaLL()       { return fAlphaLL; }
    Double_t GetFitAlpha()      { return fFitFunction -> GetParameter(2); }
    Double_t GetFitChiSquare()  { return fFitFunction -> GetChisquare(); }

    Double_t GetPosition(Double_t r = .5) {
      return fPosBeforeThreshold + (fPosMax-fPosBeforeThreshold)/(1.-fData[fPosBeforeThreshold]/fMax)*(r-fData[fPosBeforeThreshold]/fMax);
    }

  // Steer
  private:
    bool Fit(); ///< Master run method
    bool NoFit();

    bool PedestalSubtraction ();
    bool ThresholdHandling   ();
    bool CollectSample       ();
    bool FitSample           ();

    Double_t PulseFunction(Double_t *tb, Double_t *par);

  private:
    bool fFitFlag = true;
    bool fUseShortRange = false;

    Double_t fNormAmp;

    Int_t fN = kNWPSA_NDATA_MAX; ///< number of data point
    Double_t fData[kNWPSA_NDATA_MAX]; ///< data to fit

    Double_t fPedestal; ///< pedestal value

    Int_t fPosBeforeThreshold; ///< position just before value rise above threshold
    Double_t fThreshold; ///< threshold
    Double_t fThresholdLL = 4; ///< low limit of threshold
    Double_t fThresholdRatio = 0.01; ///< threshold = fThresholdRatio * fMax

    Int_t fPosMax; ///< position at max value
    Double_t fMax; ///< max value (-pedestal after the PedestalSubtraction)

    Double_t fTotalSum; ///< 
    Double_t fPartSum; ///< 

    TGraphErrors *fSample; ///< Sample to fit
    TF1 *fFitFunction; ///< TF1 of PulseFunction()

    Double_t fAlphaLL = 0; // Low limit of alpha

  // Below members are only for debugging and checking data
  private:
    Int_t fIdx = 0; //! last index for histogram
    TH1D *fH1 = nullptr; //! last hist made
    TF1 *fF1 = nullptr; //! last function made
    TGraphErrors *fG1 = nullptr; //! last graph made
 
  ClassDef(NWPSATask, 1)
};

#endif
