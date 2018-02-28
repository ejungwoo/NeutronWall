#include "style.h"
using namespace style;

void draw_channel(TString fileName = "./calibrated.root")
{
  gstat(0);
  auto run = KBRun::GetRun();
  run -> SetInputFile(fileName);
  run -> SetOutputFile("temp.out.root");
  run -> Init();
  auto barArray = run -> GetBranchA("Bar");

  auto psa = new NWPSATask;
  psa -> Init();

  auto count = 0;

  for (auto event = 0; event < 1400; ++event)
  //auto event = 423;
  {
    run -> GetEvent(event);

    for (auto ibar = 0; ibar < barArray -> GetEntries(); ++ibar)
    {
      auto bar = (NWBar *) barArray -> At(ibar);
      if (bar -> GetWallID() == 0 && bar -> GetBarID() == 18 && (bar -> GetSubTime() > -7 && bar -> GetSubTime() < -5) 
        && (bar -> GetLeft() -> GetADC() + bar -> GetRight() -> GetADC()) > 200
        //&& (bar->GetLeft()->GetTDC() + bar->GetRight()->GetTDC())/2.<65)
        && (bar->GetLeft()->GetTDC() + bar->GetRight()->GetTDC())/2.>100)
      {
        auto right = bar -> GetRight();
        auto left  = bar -> GetLeft();

        psa -> SetAlphaLL(abs(2 - bar -> GetSubTime()));
        psa -> Exec(right);
        //auto a = psa -> GetdTb();
        //psa -> Draw();
        {
          auto h = psa -> H();
          auto g = psa -> G();
          auto f = psa -> F();
          auto l = psa -> L();

          h -> SetMinimum(-1);
          h -> GetXaxis() -> SetRangeUser(psa -> GetPosMax()-10, psa -> GetPosMax()+80);

          auto cvs = c(h->GetName());
          make(h) -> Draw();
          make(g) -> Draw("samep");
          make(f) -> Draw("samel");
          l -> Draw();
        }
        auto ac = psa -> GetFitChiSquare();

        psa -> SetAlphaLL(abs(-11-bar -> GetSubTime()));
        psa -> Exec(left);
        //auto b = psa -> GetdTb();
        //psa -> Draw();
        {
          auto h = psa -> H();
          auto g = psa -> G();
          auto f = psa -> F();
          auto l = psa -> L();

          h -> SetMinimum(-1);
          h -> GetXaxis() -> SetRangeUser(psa -> GetPosMax()-10, psa -> GetPosMax()+80);

          auto cvs = c(h->GetName());
          make(h) -> Draw();
          make(g) -> Draw("samep");
          make(f) -> Draw("samel");
          l -> Draw();
        }
        auto bc = psa -> GetFitChiSquare();

        //cout << event << " " << bar -> GetSubTime() << endl;

        if (++count>5)
          return;
      }
    }
  }
}
