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
  auto run = KBRun::GetRun();
  auto par = run -> GetParameterContainer();

  fBarArray = run -> GetBranchA("Bar");

  fPSATask = new NWPSATask();
  fPSATask -> SetRank(fRank+1);

  return true;
}

void NWEventAnaTask::Exec(Option_t*)
{
  for (auto iBar = 0; iBar < fBarArray -> GetEntries(); ++iBar)
  {
    auto bar = (NWBar *) fBarArray -> At(iBar);

    auto wallID = bar -> GetWallID();
    auto barID = bar -> GetBarID();

    for (auto channel : {bar -> GetLeft(), bar -> GetRight()}) {
      fPSATask -> Fit(channel);
    }
  }
}
