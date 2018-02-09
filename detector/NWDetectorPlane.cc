#include "NWDetectorPlane.hh"
#include "KBVector3.hh"
#include "TCanvas.h"
#include "TH2Poly.h"
#include "TGraph.h"
#include "TArrow.h"
#include "TText.h"
#include <iostream>
using namespace std;

ClassImp(NWDetectorPlane)

NWDetectorPlane::NWDetectorPlane()
:KBDetectorPlane("NWDetectorPlane", "")
{
}

bool NWDetectorPlane::Init()
{
  TString axisNormal = fPar -> GetParString(Form("axisNormal%d",fPlaneID));
       if (axisNormal == "x") fRefNormal = 0;
  else if (axisNormal == "y") fRefNormal = 1;
  else if (axisNormal == "z") fRefNormal = 2;
  else {
    cout << "[NWDetectorPlane] axisNormal is not one of x, y, z" << endl;
    return false;
  }

  TString axisReadout = fPar -> GetParString(Form("axisReadout%d",fPlaneID));
       if (axisReadout == "x") fRefReadout = 0;
  else if (axisReadout == "y") fRefReadout = 1;
  else if (axisReadout == "z") fRefReadout = 2;
  else {
    cout << "[NWDetectorPlane] axisReadout is not one of x, y, z" << endl;
    return false;
  }

  for (Short_t i = 0; i < 3; ++i)
    if (fRefReadout != i && fRefNormal != i)
      fRefSide = i;

  fName = fPar -> GetParString(Form("wallName%d",fPlaneID));
  if (fIsTopReadout) {
    if (fRefReadout == 1) fName = fName + " Top";
    else                  fName = fName + " Right";
  }
  else {
    if (fRefReadout == 1) fName = fName + " Bottom";
    else                  fName = fName + " Left";
  }

  fNumTimeBuckets = fPar -> GetParInt("numTimeBuckets");
  fNumTimeBuckets = fPar -> GetParInt("numTimeBuckets");
  fNumTimeBucketsLite = fPar -> GetParInt("numTimeBucketsLite");

  fNumChannels = fChannelArray -> GetEntries();
  for (auto iChannel = 0; iChannel < fNumChannels; ++iChannel)
  {
    auto channel = (KBChannel *) fChannelArray -> At(iChannel);
    auto pos  = channel -> GetPos1();
    auto dpos = channel -> GetPos2();
    auto low  = KBVector3(pos - dpos, fRefNormal);
    auto high = KBVector3(pos + dpos, fRefNormal);

    if ( low.I() <  fLowLimitI)  fLowLimitI =  low.I();
    if (high.I() > fHighLimitI) fHighLimitI = high.I();
    if ( low.J() <  fLowLimitJ)  fLowLimitJ =  low.J();
    if (high.J() > fHighLimitJ) fHighLimitJ = high.J();
  }

  return true;
}


bool NWDetectorPlane::IsInBoundary(Double_t i, Double_t j)
{
  if (i >= fLowLimitI && i < fHighLimitI && j >= fLowLimitJ && j < fHighLimitJ)
    return true;

  return false;
}

Int_t NWDetectorPlane::FindChannelID(Double_t i, Double_t j)
{
  for (auto iChannel = 0; iChannel < fNumChannels; ++iChannel)
  {
    auto channel = (KBChannel *) fChannelArray -> At(iChannel);
    auto pos  = channel -> GetPos1();
    auto dpos = channel -> GetPos2();
    auto low  = KBVector3(pos - dpos, fRefNormal);
    auto high = KBVector3(pos + dpos, fRefNormal);

    if (i >= low.I() && i < high.I() && j >= low.J() && j < high.J())
      return iChannel;
  }

  return -1;
}

TH2* NWDetectorPlane::GetHist(Option_t *option)
{
  if (fH2Plane != nullptr)
    return fH2Plane;

  TH2D *h2 = nullptr;
  if (fRefReadout == 1)
    h2 = new TH2D(fName,fName+";x (mm); y (mm)",fNumChannels,fLowLimitI,fHighLimitI,fNumTimeBucketsLite,fLowLimitJ,fHighLimitJ);
  else
    h2 = new TH2D(fName,fName+";x (mm); y (mm)",fNumTimeBucketsLite,fLowLimitI,fHighLimitI,fNumChannels,fLowLimitJ,fHighLimitJ);

  fH2Plane = (TH2 *) h2;
  fH2Plane -> SetStats(0);
  fH2Plane -> GetXaxis() -> SetTickSize(0.01);
  fH2Plane -> GetYaxis() -> SetTickSize(0.01);

  auto fMainTitleSize = 0.06;
  auto axisLabelSize = 0.04;
  auto axisTitleSize = 0.04;
  auto xTitleOffset = 1.18;
  auto yTitleOffset = 1.70;
  fH2Plane -> GetXaxis() -> CenterTitle();
  fH2Plane -> GetXaxis() -> SetTitleOffset(xTitleOffset);
  fH2Plane -> GetXaxis() -> SetTitleSize(axisTitleSize);
  fH2Plane -> GetXaxis() -> SetLabelSize(axisLabelSize);
  fH2Plane -> GetYaxis() -> CenterTitle();
  fH2Plane -> GetYaxis() -> SetTitleOffset(yTitleOffset);
  fH2Plane -> GetYaxis() -> SetTitleSize(axisTitleSize);
  fH2Plane -> GetYaxis() -> SetLabelSize(axisLabelSize);

  return fH2Plane;
}

void NWDetectorPlane::DrawFrame(Option_t *option)
{
  auto line = new TLine();
  line -> SetLineColor(kGray+1);
  line -> SetLineStyle(2);

  auto arrow = new TArrow();
  arrow -> SetLineColor(kBlack);

  auto text = new TText();
  text -> SetTextSize(0.4/fNumChannels);
  text -> SetTextAlign(22);

  KBVector3 addToReadout(TVector3(0,0,0),fRefReadout);;
  addToReadout.SetK(1);
  for (auto iChannel = 0; iChannel < fNumChannels; ++iChannel)
  {
    auto channel = (KBChannel *) fChannelArray -> At(iChannel);

    auto pos   = channel -> GetPos1();
    auto dpos  = channel -> GetPos2();

    auto low   = KBVector3(pos - dpos, fRefNormal);
    auto high  = KBVector3(pos + dpos, fRefNormal);

    auto ref1   = KBVector3();
    auto ref2   = KBVector3();
    auto start1 = KBVector3();
    auto start2 = KBVector3();
    auto ttpos1 = KBVector3();
    auto ttpos2 = KBVector3();

    auto height = KBVector3(high-low,fRefNormal).J();
    auto width  = KBVector3(high-low,fRefNormal).I();

    if (fIsTopReadout) {
      ref1 = KBVector3(pos);
      ref1.AddAtXYZ(height/2.02,fRefReadout);
      start1 = ref1;
      start1.AddAtXYZ(-height/15,fRefReadout);
      ttpos1 = start1;
      ttpos1.AddAtXYZ(-height/30,fRefReadout);

      ref2 = KBVector3(pos);
      ref2.AddAtXYZ(width/2.02,fRefReadout);
      start2 = ref2;
      start2.AddAtXYZ(-width/15,fRefReadout);
      ttpos2 = start2;
      ttpos2.AddAtXYZ(-width/30,fRefReadout);
    }
    else {
      ref1 = KBVector3(pos);
      ref1.AddAtXYZ(-height/2.02,fRefReadout);
      start1 = ref1;
      start1.AddAtXYZ(height/15,fRefReadout);
      ttpos1 = start1;
      ttpos1.AddAtXYZ(height/30,fRefReadout);

      ref2 = KBVector3(pos);
      ref2.AddAtXYZ(-width/2.02,fRefReadout);
      start2 = ref2;
      start2.AddAtXYZ(+width/15,fRefReadout);
      ttpos2 = start2;
      ttpos2.AddAtXYZ(+width/30,fRefReadout);
    }

    if (fRefReadout == 1) {
      arrow -> DrawArrow(start1.I(),start1.J(),ref1.I(),ref1.J(),0.4/fNumChannels,">");
      text -> DrawText(ttpos1.I(),ttpos1.J(),Form("%d",iChannel));
    } else {
      arrow -> DrawArrow(start2.I(),start2.J(),ref2.I(),ref2.J(),0.4/fNumChannels,">");
      text -> DrawText(ttpos2.I(),ttpos2.J(),Form("%d",iChannel));
    }

    if (iChannel != 0) {
      if (fRefReadout == 1)
        line -> DrawLine(low.I(),low.J(),low.I(),low.J()+height);
      else
        line -> DrawLine(low.I(),low.J(),low.I()+width,low.J());
    }
  }
}

TCanvas *NWDetectorPlane::GetCanvas(Option_t *option)
{
  if (fCanvas == nullptr) {
    auto w = 1100;
    auto h = w*(fHighLimitJ-fLowLimitJ)/(fHighLimitI-fLowLimitI);
    if (h > 720) {
      h = 720;
      w = h*(fHighLimitI-fLowLimitI)/(fHighLimitJ-fLowLimitJ);
    }
    w = 1.1*w;
    fCanvas = new TCanvas(fName,fName,w,h);
    auto tm = 0.10;
    auto bm = 0.10;
    auto lm = 0.13;
    auto rm = 0.12;
    fCanvas->SetMargin(lm,rm,bm,tm);
  }

  return fCanvas;
}

void NWDetectorPlane::IsTopReadout(bool val) { fIsTopReadout = val; }
