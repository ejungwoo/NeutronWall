#include "style.h"
using namespace style;

void draw_alpha()
{
  gstat(0);

  TCut cutwb = "fWallID==0&&fBarID==18";
  TCut cut = cutwb;

  auto filea = new TFile("/Users/ejungwoo/KEBI/data/run0133.a.root");
  auto treea = (TTree *) filea -> Get("data");
  treea -> SetTitle("gamma");
  /*
  auto filecopy = new TFile("small_alpha.root","recreate");
  auto treecopy = treea -> CopyTree("fAlpha<1");
  treecopy -> Write();
  return;
  */
  auto fileb = new TFile("/Users/ejungwoo/KEBI/data/run0133.b.root");
  auto treeb = (TTree *) fileb -> Get("data");
  treeb -> SetTitle("not_gamma");


  auto psa = new NWPSATask();
  psa -> Init();
  psa -> SetFit(true);
  psa -> SetUseShortRange(true);
  auto draw_channel = [psa, treea, treeb]()
  {
    for (auto tree : {treea, treeb})
    {
      TClonesArray *array = nullptr;
      tree -> SetBranchAddress("BarF",&array);
      auto cvs = new TCanvas(tree->GetTitle(),"",1200,680);
      cvs -> Divide(2,2);
      auto count = 0;

      bool draw_more = true;
      for (auto event = 0; event < tree -> GetEntries() && draw_more; ++event)
      {
        tree -> GetEntry(event);

        for (auto ibar = 0; ibar < array -> GetEntries() && draw_more; ++ibar)
        {
          auto bar = (NWBar *) array -> At(ibar);

          if (bar -> GetWallID() == 0 && bar -> GetBarID() == 18)
          {
            vector<NWChannel*> channels;
            channels.push_back(bar -> GetLeft());
            channels.push_back(bar -> GetRight());

            for (auto channel : channels)
            {
              psa -> Exec(((TArrayS*)channel));
              auto h = psa -> H();
              auto g = psa -> G();
              auto f = psa -> F();
              auto l = psa -> L("f");

              h -> SetMinimum(-1);
              h -> SetMaximum(psa -> GetFitAmplitude()*1.1);
              h -> GetXaxis() -> SetRangeUser(psa -> GetPosMax()-10, psa -> GetPosMax()+80);

              cvs -> cd(++count);// -> SetLogy();
              make(h) -> Draw();
              make(g) -> Draw("samep");
              make(f) -> Draw("samel");
              l -> Draw();

              if (count>4) {
                draw_more = false;
                break;
              }
            }
          }
        }
      }
    }
  };

  auto draw2 = [treea,treeb,cut](TString name, Int_t n, Double_t x0, Double_t x1, Int_t m, Double_t y0, Double_t y1)
  {
    auto name0 = name;
    name0.ReplaceAll("/","ov");
    name0.ReplaceAll("(","L");
    name0.ReplaceAll(")","R");
    name0.ReplaceAll(":","OV");
    auto a = new TH2D(name0+"a",name,n,x0,x1,m,y0,y1);
    auto b = new TH2D(name0+"b",name,n,x0,x1,m,y0,y1);

    treea -> Project(name0+"a",name,cut);
    treeb -> Project(name0+"b",name,cut);

    cc(name0+"a"); a -> Draw("colz");
    cc(name0+"b"); b -> Draw("colz");
  };

  auto draw = [treea,treeb,cut](TString name, Int_t n, Double_t x0, Double_t x1)
  {
    auto name0 = name;
    name0.ReplaceAll("/","ov");
    name0.ReplaceAll("(","L");
    name0.ReplaceAll(")","R");
    name0.ReplaceAll(":","OV");
    auto a = new TH1D(name0+"a",name,n,x0,x1);
    auto b = new TH1D(name0+"b",name,n,x0,x1);
         b -> SetLineColor(kRed);
    treea -> Project(name0+"a",name,cut);
    treeb -> Project(name0+"b",name,cut);
    cc(name0);
    if (a->GetBinContent(a->GetMaximumBin())>b->GetBinContent(b->GetMaximumBin())) {
      a -> Draw();
      b -> Draw("same");
    } else {
      b -> Draw();
      a -> Draw("same");
    }
  };

  draw("fAlpha",40,0,16);
  draw("fChiSquare",40,0,100);
  draw("fAlpha/fADC",40,0,0.1);

  draw("(fLeft.fTDC+fRight.fTDC)/2",240,0,240);
  draw2("log(fLeft.fADC/fRight.fADC):fLeft.fTDC-fRight.fTDC",80,-20,20,100,-4,4);

  draw_channel();
}
