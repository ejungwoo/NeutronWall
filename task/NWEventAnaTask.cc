#include "KBRun.hh"
#include "NWBar.hh"
#include "NWChannel.hh"
#include "NWEventAnaTask.hh"

ClassImp(NWEventAnaTask)

NWEventAnaTask::NWEventAnaTask()
:KBTask("NWEventAnaTask","")
{
}

bool NWEventAnaTask::Init()
{
  fBarArray = KBRun::GetRun() -> GetBranchA("Bar");

  fPSA = new NWPSATask();
  fPSA -> Init();
  fPSA -> SetRank(fRank+1);
  fPSA -> SetFit(fFitFlag);

  if (!fReuseBranchFlag) {
    fBarArrayNew = new TClonesArray("NWBar", 112);
    KBRun::GetRun() -> RegisterBranch("BarF", fBarArray, true);
  }

  return true;
}

void NWEventAnaTask::Exec(Option_t*)
{
  if (!fReuseBranchFlag)
    fBarArrayNew -> Clear("C");



  for (auto iBar = 0; iBar < fBarArray -> GetEntries(); ++iBar)
  {
    auto bar = (NWBar *) fBarArray -> At(iBar);

    NWBar *barOut = bar;
    if (!fReuseBranchFlag)
      barOut = (NWBar *) fBarArrayNew -> ConstructedAt(fBarArrayNew -> GetEntries());

    auto left = bar -> GetLeft();
    auto lPos = abs(-11-bar->GetTDCDiff());
    fPSA -> SetAlphaLL(lPos); // TODO THIS IS ONLY FOR WALL 0 BAR 18
    fPSA -> Exec(left);

    Double_t tLeft;
    if (fFitFlag) {
      tLeft = fPSA -> GetFitPosition();
      left -> SetIsFitted(true);
      left -> SetADC(fPSA -> GetFitAmplitude());
      left -> SetPosition(lPos);
      left -> SetAlpha(fPSA -> GetFitAlpha());
      left -> SetChiSquare(fPSA -> GetFitChiSquare());
    }
    else {
      left -> SetIsFitted(false);
      left -> SetADC(fPSA -> GetMax());
      tLeft = fPSA -> GetPosition();
    }
    left -> SetPedestal(fPSA -> GetPedestal());
    left -> SetTDC(tLeft);



    auto right = bar -> GetRight();
    auto rPos = abs(2-bar->GetTDCDiff());
    fPSA -> SetAlphaLL(rPos); // TODO THIS IS ONLY FOR WALL 0 BAR 18
    fPSA -> Exec(right);

    Double_t tRight;
    if (fFitFlag) {
      tRight = fPSA -> GetFitPosition();
      right -> SetIsFitted(true);
      right -> SetADC(fPSA -> GetFitAmplitude());
      right -> SetPosition(rPos);
      right -> SetAlpha(fPSA -> GetFitAlpha());
      right -> SetChiSquare(fPSA -> GetFitChiSquare());
    }
    else {
      right -> SetIsFitted(false);
      right -> SetADC(fPSA -> GetMax());
      tRight = fPSA -> GetPosition();
    }
    right -> SetPedestal(fPSA -> GetPedestal());
    right -> SetTDC(tRight);



    if (!fReuseBranchFlag) {
      barOut -> SetLeft(left);
      barOut -> SetRight(right);
    }

    barOut -> SetTDCDiff(tLeft - tRight);
  }
}
