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
    void Exec(Option_t*);

    bool Fit(const Short_t  *adc, Int_t n);
    bool Fit(      Short_t  *adc, Int_t n);
    bool Fit(const Double_t *adc, Int_t n);
    bool Fit(      Double_t *adc, Int_t n);

    bool Fit(TArrayS *tarray);
    bool Fit(TArrayD *tarray);

  // Getters
  public:
            TH1D *H() { return CopyOfHistogram(); }   ///< Get copy of histogram of fData
    TGraphErrors *G() { return CopyOfFitSample(); }   ///< Get copy of TGraphErrors of sample points
             TF1 *F() { return CopyOfFitFunction(); } ///< Get copy of fitted function
         TLegend *L() { return CopyOfLegend(); }      ///< Get copy of fitted function

            TH1D *CopyOfHistogram(TString name = ""); ///< Get copy of histogram of fData
    TGraphErrors *CopyOfFitSample();                  ///< Get copy of TGraphErrors of sample points
             TF1 *CopyOfFitFunction();                ///< Get copy of fitted function
         TLegend *CopyOfLegend();                     ///< Get legend of basic and fit information

       Int_t GetN()        { return fN; }        ///< Get size of data
    Double_t *GetData()    { return fData; }     ///< Get data[]
    Double_t GetPedestal() { return fPedestal; } ///< Get pedestal value

       Int_t GetPosBeforeThreshold() { return fPosBeforeThreshold; } ///< Get position where data is just below threshold
    Double_t GetThreshold()          { return fThreshold; }          ///< Get threshold value
    Double_t GetThresholdLL()        { return fThresholdLL; }        ///< Get threshold low limit
    Double_t GetThresholdRatio()     { return fThresholdRatio; }     ///< Get threshold ratio (threshold/max)

       Int_t GetPosMax() { return fPosMax; } ///< Get position at max value
    Double_t GetMax()    { return fMax; }    ///< Get max value

    /* 
    Double_t GetAmplitude()  { return 1; }
    Double_t GetPosition()   { return 1; }
    Double_t GetAmplitudeError()  { return fAmplitude - fMax; }
    */

    Double_t GetChiSquare()  { return fFitFunction -> GetChisquare(); }

  // Steer
  private:
    bool Fit(); ///< Master run method

    bool PedestalSubtraction ();
    bool ThresholdHandling   ();
    bool CollectSample       ();
    bool FitSample           ();

    Double_t PulseFunction(Double_t *tb, Double_t *par);

  private:
    Int_t fN = kNWPSA_NDATA_MAX; ///< number of data point
    Double_t fData[kNWPSA_NDATA_MAX]; ///< data to fit

    Double_t fPedestal; ///< pedestal value

    Int_t fPosBeforeThreshold; ///< position just before value rise above threshold
    Double_t fThreshold; ///< threshold
    Double_t fThresholdLL = 0; ///< low limit of threshold
    Double_t fThresholdRatio = 0.01; ///< threshold = fThresholdRatio * fMax

    Int_t fPosMax; ///< position at max value
    Double_t fMax; ///< max value (-pedestal after the PedestalSubtraction)

    TGraphErrors *fSample; ///< Sample to fit
    TF1 *fFitFunction; ///< TF1 of PulseFunction()

  // Below members are only for debugging and checking data
  private:
    Int_t fIdx = 0; //! last index for histogram
    TH1D *fH1 = nullptr; //! last hist made
    TF1 *fF1 = nullptr; //! last function made
    TGraphErrors *fG1 = nullptr; //! last graph made
 
  ClassDef(NWPSATask, 1)
};

#endif
