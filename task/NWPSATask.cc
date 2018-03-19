#include "TCanvas.h"
#include "TStyle.h"
#include "NWPSATask.hh"

ClassImp(NWPSATask)

NWPSATask::NWPSATask()
:KBTask("NWPSATask","")
{
}

Double_t NWPSATask::PulseFunction(Double_t *tb, Double_t *par)
{
  auto f1 = 0.;
  auto f2 = 0.;
  auto f3 = 0.;
  auto f4 = 0.;
  auto norm = 1.;

  auto tbf = tb[0]-par[1];

  if (tbf> 0)
  {
    { // value calculation
      auto tau1 = (tbf  )/par[2] + 1;
      auto tau2 = (tbf+1)/par[2] + 1;
      auto x1 = sqrt((tau1+1)*(tau1+1)-1);
      auto x2 = sqrt((tau2+1)*(tau2+1)-1);
      f1 = x1*atan(x1) - 0.5 * log(x1*x1+1);
      f2 = x2*atan(x2) - 0.5 * log(x2*x2+1);

//    Double_t tbi = tbf-par[3];
      Double_t tbi = tbf-3;
      if (tbi < 0) {
        f3 = sqrt(3)*atan(sqrt(3)) - 0.5 * log(4);
        auto tau4 = 1./par[2] + 1;
        auto x4 = sqrt((tau4+1)*(tau4+1)-1);
        f4 = x4*atan(x4) - 0.5 * log(x4*x4+1);
      }
      else {
        auto tau3 = (tbi  )/par[2] + 1;
        auto tau4 = (tbi+1)/par[2] + 1;
        auto x3 = sqrt((tau3+1)*(tau3+1)-1);
        auto x4 = sqrt((tau4+1)*(tau4+1)-1);
        f3 = x3*atan(x3) - 0.5 * log(x3*x3+1);
        f4 = x4*atan(x4) - 0.5 * log(x4*x4+1);
      }
    }

    { // normalize calculation
//    auto tau1 = (par[3]  )/par[2] + 1;
//    auto tau2 = (par[3]+1)/par[2] + 1;
      auto tau1 = 3/par[2] + 1;
      auto tau2 = 4/par[2] + 1;
      auto x1 = sqrt((tau1+1)*(tau1+1)-1);
      auto x2 = sqrt((tau2+1)*(tau2+1)-1);
      auto f1_norm = x1*atan(x1) - 0.5 * log(x1*x1+1);
      auto f2_norm = x2*atan(x2) - 0.5 * log(x2*x2+1);

      auto f3_norm = sqrt(3)*atan(sqrt(3)) - 0.5 * log(4);
      auto tau4 = 1./par[2] + 1;
      auto x4= sqrt((tau4+1)*(tau4+1)-1);
      auto f4_norm = x4*atan(x4) - 0.5 * log(x4*x4+1);

      norm = fNormAmp*((f2_norm-f1_norm)-(f4_norm-f3_norm));
    }
  }

  auto val = (f2-f1)-(f4-f3);
  val = par[0] * val / norm;
  val = exp(-tbf/7.) * val; 

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
  fFitFunction -> SetParNames("amplitude","position","#alpha (L/c)","#Deltatb");
//fFitFunction -> SetParameters(100,50,5,1);
  fFitFunction -> SetParameters(100,50,5);

  fF1 = fFitFunction;

  fNormAmp = exp(-3./7);

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

  ++fIdx;

  return true;
}

bool NWPSATask::NoFit()
{
  Clear();

  if (fN < 50) {
    kb_error << "Array smaller than 50 (" << fN << ")" << endl;
    return false;
  }

  if (! PedestalSubtraction ()) return false;
  if (! ThresholdHandling   ()) return false;

  ++fIdx;

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

  fTotalSum = 0;
  for (auto pos = 0; pos < fN; ++pos)
    fTotalSum += fData[pos];

  fPartSum = 0;
  for (auto pos = 0; pos < fPosBeforeThreshold+30; ++pos)
    fPartSum += fData[pos];

  return true;
}

bool NWPSATask::CollectSample()
{
  auto posStart = fPosBeforeThreshold - 5;
  auto posEnd = fN;
  if (posEnd - posStart + 1 > 100)
    posEnd = posStart - 1 + 100;

  auto xerror = 0.5;
  auto yerror = 0.02*fMax;
  if (yerror < 0.5)
    yerror = 0.5;

  Int_t    stepRegions[]  = {4,3,6,25,kNWPSA_NDATA_MAX};
  Double_t scaleRegions[] = {1.,2.,8.,4.,2.};

  if (fUseShortRange) {
    stepRegions[2] = 0;
    stepRegions[3] = 0;
    stepRegions[4] = 0;
  }

  auto pos = 0;
  for (pos = posStart; pos <= fPosBeforeThreshold; ++pos) {
    fSample -> SetPoint(fSample->GetN(), pos+0.5, 0);
    fSample -> SetPointError(fSample->GetN()-1, xerror, yerror);
  }

  auto thresholdCollect = 0.;

  for (; pos < fPosMax; ++pos) {
    auto val = fData[pos];
    if (val > thresholdCollect) {
      fSample -> SetPoint(fSample->GetN(), pos+0.5, val);
      fSample -> SetPointError(fSample->GetN()-1, xerror, yerror);
    }
  }

  for (auto iRegion = 0; iRegion < 4; ++iRegion)
  {
    auto step = stepRegions[iRegion];
    auto scale = scaleRegions[iRegion];

    for (auto ipos = 0; ipos < step && pos < posEnd; ++ipos) {
      auto val = fData[pos];
      if (val > thresholdCollect) {
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
//fFitFunction -> SetParameters(fMax, fPosBeforeThreshold, 5, 1);
  fFitFunction -> SetParameters(fMax, fPosBeforeThreshold, 5);
  fFitFunction -> SetParLimits(0, fMax, fMax*1.2);
  fFitFunction -> SetParLimits(1, fPosBeforeThreshold-3.5, fPosBeforeThreshold+1.5);
  fFitFunction -> SetParLimits(2, 0.1, 15);
//fFitFunction -> SetParLimits(3, 0.1, 10);
  if (fAlphaLL > 0) {
    fFitFunction -> SetParameter(2,fAlphaLL);
    fFitFunction -> SetParLimits(2,fAlphaLL,fAlphaLL+30);
  }

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
  kb_warning << "  psa -> Init();" << endl;
  kb_warning << "  psa -> Exec(adc, n);" << endl;
  kb_warning << "  ..." << endl;
  kb_warning << "}" << endl;
}

bool NWPSATask::Exec(const Short_t  *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(adc[pos]);

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

bool NWPSATask::Exec(Short_t  *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(adc[pos]);

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

bool NWPSATask::Exec(const Double_t *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = adc[pos];

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

bool NWPSATask::Exec(Double_t *adc, Int_t n)
{
  fN = n;
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = adc[pos];

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

bool NWPSATask::Exec(TArrayS *tarray)
{
  fN = tarray -> GetSize();
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = Double_t(tarray -> At(pos));

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

bool NWPSATask::Exec(TArrayD *tarray)
{
  fN = tarray -> GetSize();
  for (auto pos = 0; pos < fN; ++pos)
    fData[pos] = tarray -> At(pos);

  if (fFitFlag)
    return Fit();
  else
    return NoFit();
}

TH1D *NWPSATask::CopyOfHistogram(TString name)
{
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
  //f1 -> SetParameter(3, fFitFunction -> GetParameter(3));
  f1 -> SetNpx(2000);

  fF1 = f1;

  return f1;
}

TLegend *NWPSATask::MakeLegend(Option_t *opt)
{
  TString opts = TString(opt);
  opts.ToLower();

  auto l = new TLegend(0.5, 0.4, 0.90, 0.88);

  l -> SetFillStyle(0);
  l -> SetBorderSize(0);

  auto minit = new TMarker(0,0,23); minit -> SetMarkerColor(kBlack);
  auto mmax = new TMarker(0,0,22); mmax -> SetMarkerColor(kPink);
  auto lx = new TLine(0,0,0,0); lx -> SetLineStyle(7);
  auto lf = new TLine(0,0,0,0); lf -> SetLineColor(kRed);

  if (opts.Index("h") >= 0)
  {
    if (fH1 != nullptr)
      l -> AddEntry(fH1, "Data", "l");
    l -> AddEntry((TObject*)0, Form("pedestal: %.1f", fPedestal), "");
    l -> AddEntry((TObject*)0, Form("pos/adc-thr.: %d / %.1f", fPosBeforeThreshold, fThreshold), "");
    l -> AddEntry((TObject*)0, Form("pos/adc-max.: %d / %.1f", fPosMax, fNormAmp*fMax), "");
  }

  if (opts.Index("g") >= 0)
    l -> AddEntry(fG1, "Sample points for fit", "pl");


  if (opts.Index("f") >= 0) {
    l -> AddEntry(fF1, "Fit line", "l");
    l -> AddEntry((TObject*)0, Form("Amplitude: %.1f", fFitFunction -> GetParameter(0)), "");
    l -> AddEntry((TObject*)0, Form("Position: %.1f",  fFitFunction -> GetParameter(1)), "");
    l -> AddEntry((TObject*)0, Form("Eff. Tail-L: %.1f", fFitFunction -> GetParameter(2)), "");
    if (fAlphaLL > 0)
      l -> AddEntry((TObject*)0, Form("#alpha_0: %.1f",  fAlphaLL), "");
    l -> AddEntry((TObject*)0, Form("#chi^{2}: %.1f",  fFitFunction -> GetChisquare()),  "");
  }

  return l;
}

void NWPSATask::Draw(Option_t *option)
{
  gStyle -> SetOptStat(0);
  gStyle -> SetStatStyle(0);
  gStyle -> SetStatW(0.25);
  gStyle -> SetStatH(0.18);
  gStyle -> SetStatX(0.95);
  gStyle -> SetStatY(0.88);
  gStyle -> SetTitleFontSize(0.08);

  TString name = Form("PSA_%d",fIdx);

  auto h = H();
  auto g = G();
  auto f = F();
  auto l = L();

  h -> SetMinimum(-1);
  h -> GetXaxis() -> CenterTitle();
  h -> GetXaxis() -> SetTitleOffset(1.15);
  h -> GetXaxis() -> SetTitleSize(0.06);
  h -> GetXaxis() -> SetLabelSize(0.05);
  h -> GetYaxis() -> CenterTitle();
  h -> GetYaxis() -> SetTitleOffset(1.35);
  h -> GetYaxis() -> SetTitleSize(0.06);
  h -> GetYaxis() -> SetLabelSize(0.05);
  h -> GetZaxis() -> SetLabelSize(0.05);

  new TCanvas(name,name,680,550);
  h -> Draw();
  g -> Draw("samep");
  f -> Draw("samel");
  l -> Draw();
}
