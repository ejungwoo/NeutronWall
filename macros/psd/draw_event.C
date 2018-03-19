#include "style.h"
using namespace style;

void draw_event()
{
  auto file = new TFile("/Users/ejungwoo/KEBI/data/run0133.event.root");
  auto tree = (TTree *) file -> Get("data");

  TCut cutwb = "fWallID==0&&fBarID==18";
  TCut cutgm = "";//"sqrt(fRight.fADCTotalSum*fLeft.fADCTotalSum)/sqrt(fRight.fADC*fLeft.fADC)>4";
  TCut cut = cutwb + cutgm;

  //cc(); tree -> Draw("log(fLeft.fADC/fRight.fADC):fLeft.fTDC-fRight.fTDC>>hist(80,-20,20,100,-4,4)", cut, "colz");
  cc(); tree -> Draw("fADCDiff:fTDCDiff>>hist(80,-20,20,100,-4,4)", cut, "colz");
  cc(); tree -> Draw("(fLeft.fTDC+fRight.fTDC)/2>>tdc(240,0,240)", "", "colz");
  cc(); tree -> Draw("sqrt(fRight.fADCTotalSum*fLeft.fADCTotalSum)/sqrt(fRight.fADC*fLeft.fADC)>>ss(100,0,10)", "", "");
  cc(); tree -> Draw("fGeoMean>>gm(100,0,1000)", "", "");
  cc(); tree -> Draw("fTDCDiff>>td(100,-20,20)", cut, "");
}
