void choose_w0_b18()
{
  auto file = new TFile("/Users/ejungwoo/KEBI/data/run0133.event.root");
  auto tree = (TTree *) file -> Get("data");

  TCutG *cutg = new TCutG("mycut",7);
  cutg->SetVarX("fTDCDiff");
  cutg->SetVarY("Bar.fLeft.fADC-Bar.fRight.fADC");
  cutg->SetPoint(0, -9.31232,  40.5263);
  cutg->SetPoint(1, -9.31232,  2.63158);
  cutg->SetPoint(2, -1.36103, -67.8947);
  cutg->SetPoint(3,  1.43266, -61.5789);
  cutg->SetPoint(4, 0.787966, -16.3158);
  cutg->SetPoint(5, -6.87679,  44.7368);
  cutg->SetPoint(6, -9.31232,  40.5263);

  auto ofile = new TFile("/Users/ejungwoo/KEBI/data/run0133.w0_b18.root","recreate");
  cout << ofile -> GetName() << endl;
  auto otree = tree -> CopyTree("(mycut)&& Bar.fWallID==0&& Bar.fBarID==18");
  otree -> Write();
}
