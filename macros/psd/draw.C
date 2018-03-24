#include "style.h"
using namespace style;

void draw()
{
  zcolor(1);

  auto file = new TFile("/mnt/spirit/analysis/user/leej/KEBI/data/run0785.event.root");
  auto tree = (TTree *) file -> Get("data");

  TCanvas *cvs;

  //tree -> Draw("sqrt(fRight.fADCTotalSum*fLeft.fADCTotalSum)/sqrt(fRight.fADC*fLeft.fADC)>>asdf(100,0,10)");//>4";
  //TCut cut = cutwb + cutgm;
  //TCut cutg = "(fRight.fTDC+fLeft.fTDC)/2>55 && (fRight.fTDC+fLeft.fTDC)/2<65";
  //TCut cutn = "(fRight.fTDC+fLeft.fTDC)/2>100 && (fRight.fTDC+fLeft.fTDC)/2<130";

  TCut cut = "sqrt(fRight.fADCTotalSum*fLeft.fADCTotalSum)/sqrt(fRight.fADC*fLeft.fADC) > 4 && fGeoMeanTotal != fGeoMeanPart";

  //cvs = cc(); tree -> Draw("fBarID+fWallID*24:fGeoMeanTotal>>hist(1000,0,50000,48,1,49)",cut,"colz"); cvs -> SetLogz();
  //cvs = cc(); tree -> Draw("fGeoMeanPart:fGeoMeanTotal>>bar1(200,0,5000,200,0,5000)",cut+"fWallID==0&&fBarID==1","colz"); cvs -> SetLogz(); cvs -> SetGrid();
  //cvs = cc(); tree -> Draw("fGeoMeanPart:fGeoMeanTotal>>bar2(200,0,5000,200,0,5000)",cut+"fWallID==0&&fBarID==2","colz"); cvs -> SetLogz(); cvs -> SetGrid();
  //cvs = cc(); tree -> Draw("fGeoMeanPart:fGeoMeanTotal>>bar3(200,0,5000,200,0,5000)",cut+"fWallID==0&&fBarID==3","colz"); cvs -> SetLogz(); cvs -> SetGrid();
  cvs = cc(); tree -> Draw("fGeoMeanPart:fGeoMeanTotal>>bar24(500,0,5000,500,0,5000)",cut+"fWallID==0&&fBarID==24","colz"); cvs -> SetLogz(); cvs -> SetGrid();
  cvs = cc(); tree -> Draw("fGeoMeanPart/fGeoMeanTotal>>bar24_p(500,0.5,1.5)",cut+"fWallID==0&&fBarID==24","colz"); cvs -> SetGrid();
  return;

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
