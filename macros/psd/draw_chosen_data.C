#include "style.h"
using namespace style;

void draw_chosen_data()
{
  auto file = new TFile("/Users/ejungwoo/KEBI/data/run0133.w0_b18_fit.root");
  auto tree = (TTree *) file -> Get("data");

  cc(); tree -> Draw("fLeft.fAlpha-fLeft.fPosition", "fWallID==0&&fBarID==18");
  cc(); tree -> Draw("fRight.fAlpha-fRight.fPosition","fWallID==0&&fBarID==18");

  cc(); tree -> Draw("(fLeft.fAlpha-fLeft.fPosition+fRight.fAlpha-fRight.fPosition)/2","fWallID==0&&fBarID==18");

  cc(); tree -> Draw("fLeft.fChiSquare>>histChiL(100,0,200)","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("fRight.fChiSquare>>histChiR(100,0,200)","fWallID==0&&fBarID==18");

  cc(); tree -> Draw("(fLeft.fTDC+fRight.fTDC)/2","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fTDC+fRight.fTDC)/2:(fLeft.fAlpha-fLeft.fPosition+fRight.fAlpha-fRight.fPosition)/2","fWallID==0&&fBarID==18","colz");
  return;

  cc(); tree -> Draw("BarF.fTDCDiff","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fTDC-fRight.fTDC)","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fADC+fRight.fADC)/2","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fPosition-fRight.fPosition)/2","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fPosition-fRight.fPosition)/2","fWallID==0&&fBarID==18");

  cc(); tree -> Draw("(fLeft.fAlpha-fLeft.fPosition+fRight.fAlpha-fRight.fPosition)/2","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fAlpha+fRight.fAlpha)/2","fWallID==0&&fBarID==18");
  cc(); tree -> Draw("(fLeft.fTDC+fRight.fTDC)/2:(fLeft.fAlpha+fRight.fAlpha)/2","fWallID==0&&fBarID==18","colz");
}
