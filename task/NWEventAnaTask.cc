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

  fPSA -> SetUseShortRange(fUseShortRange);

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

    auto chLeft = bar -> GetLeft();
    auto posLeft = abs(-11-bar->GetTDCDiff());
    //fPSA -> SetAlphaLL(posLeft); // TODO THIS IS ONLY FOR WALL 0 BAR 18
    fPSA -> Exec(chLeft);

    Double_t tLeft;
    Double_t aLeft;
    if (fFitFlag) {
      tLeft = fPSA -> GetFitPosition();
      aLeft = fPSA -> GetFitAmplitude();
      chLeft -> SetIsFitted(true);
      chLeft -> SetPosition(posLeft);
      chLeft -> SetAlpha(fPSA -> GetFitAlpha());
      chLeft -> SetChiSquare(fPSA -> GetFitChiSquare());
    }
    else {
      tLeft = fPSA -> GetPosition();
      aLeft = fPSA -> GetMax();
      chLeft -> SetIsFitted(false);
    }
    chLeft -> SetPedestal(fPSA -> GetPedestal());
    chLeft -> SetTDC(tLeft);
    chLeft -> SetADC(aLeft);

    Double_t tsLeft = fPSA -> GetTotalSum();
    Double_t psLeft = fPSA -> GetPartSum();
    chLeft -> SetADCTotalSum(tsLeft);



    auto chRight = bar -> GetRight();
    auto posRight = abs(2-bar->GetTDCDiff());
    //fPSA -> SetAlphaLL(posRight); // TODO THIS IS ONLY FOR WALL 0 BAR 18
    fPSA -> Exec(chRight);

    Double_t tRight;
    Double_t aRight;
    if (fFitFlag) {
      tRight = fPSA -> GetFitPosition();
      aRight = fPSA -> GetFitAmplitude();
      chRight -> SetIsFitted(true);
      chRight -> SetPosition(posRight);
      chRight -> SetAlpha(fPSA -> GetFitAlpha());
      chRight -> SetChiSquare(fPSA -> GetFitChiSquare());
    }
    else {
      tRight = fPSA -> GetPosition();
      aRight = fPSA -> GetMax();
      chRight -> SetIsFitted(false);
    }
    chRight -> SetPedestal(fPSA -> GetPedestal());
    chRight -> SetTDC(tRight);
    chRight -> SetADC(aRight);

    Double_t tsRight = fPSA -> GetTotalSum();
    Double_t psRight = fPSA -> GetPartSum();
    chRight -> SetADCTotalSum(tsRight);



    if (!fReuseBranchFlag) {
      barOut -> SetLeft(chLeft);
      barOut -> SetRight(chRight);
    }

    barOut -> SetTDCDiff(tLeft - tRight);
    barOut -> SetGeoMean(sqrt(aLeft*aRight));
    barOut -> SetADCDiff(log(aLeft/aRight));

    barOut -> SetGeoMeanTotal(sqrt(tsLeft*tsRight));
    barOut -> SetGeoMeanPart(sqrt(psLeft*psRight));
  }
}
