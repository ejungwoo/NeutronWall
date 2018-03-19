void pulse()
{
  auto psa = new NWPSATask();
  psa -> Init();
  auto f1 = psa -> F();

  f1 -> SetParameters(333,11,1);
  auto hist = new TH1D("hist","",100,0,100);
  hist -> SetMaximum(400);
  hist -> Draw();

  for (auto i = 1; i <= 16; i+=2) {
    f1 -> SetParameter(2,i);
    f1 -> DrawCopy("same");
  }
}
