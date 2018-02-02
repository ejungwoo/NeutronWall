#include "KBRun.hh"
#include "KBChannelHit.hh"
#include "NWDigiTask.hh"

ClassImp(NWDigiTask)

NWDigiTask::NWDigiTask()
:KBTask("NWDigiTask","")
{
}

bool NWDigiTask::Init()
{
  fChannelArray = new TClonesArray("KBChannelHit", 100);

  bool saveThisBranch = true;
  KBRun::GetRun() -> RegisterBranch("Channelhit", fChannelArray, saveThisBranch);

  return true;
}

void NWDigiTask::Exec(Option_t*)
{
  fChannelArray -> Clear();

  auto hit = (KBChannelHit *) fChannelArray -> ConstructedAt(0);
  hit -> SetID(0);
  hit -> SetTDC(0);
  hit -> SetADC(123.987);

  hit = (KBChannelHit *) fChannelArray -> ConstructedAt(1);
  hit -> SetID(1);
  hit -> SetTDC(1.1);
  hit -> SetADC(1000);

  hit = (KBChannelHit *) fChannelArray -> ConstructedAt(2);
  hit -> SetID(2);
  hit -> SetTDC(22);
  hit -> SetADC(8);

  cout << "  [" << this -> GetName() << "] Message from this task" << endl;
}
