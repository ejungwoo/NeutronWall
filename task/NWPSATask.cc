#include "NWPSATask.hh"

ClassImp(NWPSATask)

NWPSATask::NWPSATask()
:KBTask("NWPSATask","")
{
}

Double_t NWPSATask::PulseFunction(Double_t *tb, Double_t *par)
{
  if (tb[0] < par[1])
    return 0;

  auto tau1 = (tb[0]-par[1])/par[2];
  auto tau2 = (tb[0]-par[1]+1)/par[2];

  auto tt1 = (tau1+1)*(tau1+1);
  auto tt2 = (tau1+1)*(tau2+1);

  auto val = par[0] * (atan(sqrt(tt2-1)) - atan(sqrt(tt1-1)));
  val = atan(tau1) * val;

  return val;
}

bool NWPSATask::Init()
{
  fSample = new TGraphErrors();
  fSample -> SetMarkerColor(kBlack);
  fSample -> SetMarkerStyle(20);
  fSample -> SetMarkerSize(0.5);
  fSample -> SetLineWidth(1);
  fSample -> SetLineColor(kGray);

  fG1 = fSample;

  Clear();

  fFitFunction = new TF1("Pulse",this,&NWPSATask::PulseFunction,0,kNWPSA_NDATA_MAX,3,"NWPSATask","PulseFunction");
  fFitFunction -> SetNpx(1000);
  fFitFunction -> SetParNames("amplitude","position","#alpha (L/c)");
  fFitFunction -> SetParameters(100,50,1);

  fF1 = fFitFunction;

  return true;
}

void NWPSATask::Clear(Option_t *)
{
  fSample -> Set(0);
  fSample -> Clear(0);
}

bool NWPSATask::Fit()
{
  Clear();

  if (fN < 50) {
    kb_error << "Array smaller than 50 (" << fN << ")" << endl;
    return false;
  }


  if (! PedestalSubtraction ()) return false;
  if (! ThresholdHandling   ()) return false;
  if (! CollectSample       ()) return false;
  if (! FitSample           ()) return false;


  return true;
}

bool NWPSATask::PedestalSubtraction()
{
  fMax = -1.e+10;
  for (auto pos = 0; pos < fN; ++pos) {
    if (fData[pos] > fMax) {
      fMax = fData[pos];
      fPosMax = pos;
    }
  }

  auto numPedestal = fPosMax - 10;

  fPedestal = 0.;
  for (auto pos = 0; pos < numPedestal; ++pos)
    fPedestal += fData[pos];
  fPedestal = fPedestal/numPedestal;

  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = fData[pos] - fPedestal;

  fMax = fMax - fPedestal;
  if (fMax < 8) {
    kb_error << "Maximum value of the array is smaller than 8" << endl;
    return false;
  }

  return true;
}

bool NWPSATask::ThresholdHandling()
{
  fThreshold = fThresholdRatio * fMax;
  if (fThreshold < fThresholdLL)
    fThreshold = fThresholdLL;

  for (fPosBeforeThreshold = fPosMax - 1; fPosBeforeThreshold >= fPosMax - 9; --fPosBeforeThreshold) {
    if (fData[fPosBeforeThreshold] < fThreshold)
      break;
  }

  for (auto pos = 0; pos < fPosBeforeThreshold; ++pos)
    fData[pos] = 0;

  return true;
}

bool NWPSATask::CollectSample()
{
  auto xerror = 0.5;
  auto yerror = 0.02*fMax;
  if (yerror < 0.5)
    yerror = 0.5;

  Int_t    stepRegions[]  = {4,9,25,kNWPSA_NDATA_MAX};
  Double_t scaleRegions[] = {1., 8.,4.,2.};

  auto pos = 0;
  for (; pos <= fPosBeforeThreshold; ++pos) {
    fSample -> SetPoint(fSample->GetN(), pos+0.5, 0);
    fSample -> SetPointError(fSample->GetN()-1, xerror, yerror);
  }

  fSample -> SetPoint(fSample->GetN(),fPosMax-1+0.5, fData[fPosMax-1]);
  fSample -> SetPointError(fSample->GetN()-1, xerror, 2*yerror);

  pos = fPosMax;
  for (auto iRegion = 0; iRegion < 4; ++iRegion)
  {
    auto step = stepRegions[iRegion];
    auto scale = scaleRegions[iRegion];

    for (auto ipos = 0; ipos < step && pos < fN; ++ipos) {
      auto val = fData[pos];
      if (val > fThreshold) {
        fSample -> SetPoint(fSample->GetN(), pos+0.5, val);
        fSample -> SetPointError(fSample->GetN()-1, xerror, scale*yerror);
      }
      ++pos;
    }
  }

  return true;
}

bool NWPSATask::FitSample()
{
  fFitFunction -> SetParameters(10*fMax, fPosMax, 5);
  fFitFunction -> SetParLimits(1, fPosBeforeThreshold+0.5, fPosMax+0.5);
  //fFitFunction -> SetParLimits(0, fPosBeforeThreshold+0.5, fPosMax+0.5);

  fSample -> Fit(fFitFunction, "Q");

  return true;
}

void NWPSATask::Exec(Option_t*)
{
  kb_warning << "This method is empty!" << endl;
  kb_warning << "How to use of this task: " << endl;
  kb_warning << "{" << endl;
  kb_warning << "  Short_t *adc = ..." << endl;
  kb_warning << "  auto psa = new NWPSATask();" << endl;
  kb_warning << "  psa -> Fit(adc, n);" << endl;
  kb_warning << "  psa -> Fit(adc2, n);" << endl;
  kb_warning << "  ..." << endl;
  kb_warning << "}" << endl;
}

bool NWPSATask::Fit(const Short_t  *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(adc[pos]);

  return Fit();
}

bool NWPSATask::Fit(Short_t  *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(adc[pos]);

  return Fit();
}

bool NWPSATask::Fit(const Double_t *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = adc[pos];

  return Fit();
}

bool NWPSATask::Fit(Double_t *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = adc[pos];

  return Fit();
}

bool NWPSATask::Fit(TArrayS *tarray)
{
  fN = tarray -> GetSize();
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(tarray -> At(pos));

  return Fit();
}

bool NWPSATask::Fit(TArrayD *tarray)
{
  fN = tarray -> GetSize();
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = tarray -> At(pos);

  return Fit();
}

TH1D *NWPSATask::CopyOfHistogram(TString name)
{
  ++fIdx;

  if (name.IsNull())
    name = Form("histPSA_%d",fIdx);

  auto hist = new TH1D(name, Form("PSA_%d",fIdx), fN, 0, fN);
  for (auto pos = 0; pos < fN; ++pos)
    hist -> SetBinContent(pos+1, fData[pos]);

  fH1 = hist;

  return hist;
}

TGraphErrors *NWPSATask::CopyOfFitSample()
{
  auto graph = new TGraphErrors();
  graph -> SetMarkerColor(kBlack);
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.5);
  graph -> SetLineWidth(1);
  graph -> SetLineColor(kGray);

  for (auto i = 0; i < fSample -> GetN(); ++i)
  {
    Double_t x, y;
    fSample -> GetPoint(i, x, y);
    auto ex = fSample -> GetErrorX(i);
    auto ey = fSample -> GetErrorY(i);

    graph -> SetPoint(i, x, y);
    graph -> SetPointError(i, ex, ey);
  }

  fG1 = graph;

  return graph;
}

TF1 *NWPSATask::CopyOfFitFunction()
{
  auto f1 = new TF1(Form("fit_%d",fIdx),this,&NWPSATask::PulseFunction,0,fN,3,"NWPSATask","PulseFunction");
  f1 -> SetParameter(0, fFitFunction -> GetParameter(0));
  f1 -> SetParameter(1, fFitFunction -> GetParameter(1));
  f1 -> SetParameter(2, fFitFunction -> GetParameter(2));
  f1 -> SetNpx(2000);

  fF1 = f1;

  return f1;
}

TLegend *NWPSATask::CopyOfLegend()
{
  auto l = new TLegend(0.5, 0.4, 0.945, 0.88);

  l -> SetFillStyle(0);
  l -> SetBorderSize(0);

  auto minit = new TMarker(0,0,23); minit -> SetMarkerColor(kBlack);
  auto mmax = new TMarker(0,0,22); mmax -> SetMarkerColor(kPink);
  auto lx = new TLine(0,0,0,0); lx -> SetLineStyle(7);
  auto lf = new TLine(0,0,0,0); lf -> SetLineColor(kRed);

  if (fH1 != nullptr)
    l -> AddEntry(fH1, "Data", "l");
  l -> AddEntry((TObject*)0, Form("pedestal: %.1f", fPedestal), "");
  l -> AddEntry((TObject*)0, Form("pos/adc-thr.: %d / %.1f", fPosBeforeThreshold+1, fThreshold), "");
  l -> AddEntry((TObject*)0, Form("pos/adc-max.: %d / %.1f", fPosMax, fMax), "");

  l -> AddEntry(fG1, "Sample points for fit", "pl");

  l -> AddEntry(fF1, "Fit line", "l");
  l -> AddEntry((TObject*)0, Form("Amplitude: %.1f", fFitFunction -> GetParameter(0)), "");
  l -> AddEntry((TObject*)0, Form("Position: %.1f",  fFitFunction -> GetParameter(1)), "");
  l -> AddEntry((TObject*)0, Form("#alpha: %.1f",    fFitFunction -> GetParameter(2)), "");
  l -> AddEntry((TObject*)0, Form("#chi^{2}: %.1f",  fFitFunction -> GetChisquare()),  "");

  return l;
}
