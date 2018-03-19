void choose_w0_b18()
{
  auto file = new TFile("/Users/ejungwoo/KEBI/data/run0133.event.root");
  auto tree = (TTree *) file -> Get("data");


  TCut cutwb = "fWallID==0&&fBarID==18";
  TCut cutgm = "sqrt(fRight.fADCTotalSum*fLeft.fADCTotalSum)/sqrt(fRight.fADC*fLeft.fADC)>4";
  TCut cut = cutwb + cutgm;
  TCut cutg = "(fRight.fTDC+fLeft.fTDC)/2>55 && (fRight.fTDC+fLeft.fTDC)/2<65";
  TCut cutn = "(fRight.fTDC+fLeft.fTDC)/2>100 && (fRight.fTDC+fLeft.fTDC)/2<130";

  auto ofile = new TFile("/Users/ejungwoo/KEBI/data/run0133.choose.root","recreate"); cout << ofile -> GetName() << endl;
  auto otree1 = tree -> CopyTree(cut+cutg);
  otree1 -> Write("a");
  auto otree2 = tree -> CopyTree(cut+cutn);
  otree2 -> Write("b");
  ofile -> Close();
}
