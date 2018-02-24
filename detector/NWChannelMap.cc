#include "NWChannelMap.hh"

#include "TString.h"

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

ClassImp(NWChannelInfo)

NWChannelInfo::NWChannelInfo()
{
  Clear();
}

NWChannelInfo::NWChannelInfo(int channelID, int pairID, int moduleID, int mchannelID, int barID, bool ab, bool lr)
{
  Clear();

  fChannelID  = channelID;
  fPairID     = pairID;
  fModuleID   = moduleID;
  fMChannelID = mchannelID;
  fBarID      = barID;
  fAB         = ab;
  fLR         = lr;
}

void NWChannelInfo::Clear(Option_t *opt)
{
  fChannelID = -1;
  fPairID = -1;
  fModuleID = -1;
  fMChannelID = -1;
  fBarID = -1;
  fAB = false;
  fLR = false;
  fTriggered = false;
}

void NWChannelInfo::Print(Option_t *) const
{
  cout << setw(4) << fChannelID
       << setw(4) << fPairID
       << setw(4) << fModuleID
       << setw(4) << fMChannelID
       << setw(4) << fBarID;

  if (fBarID != -1)
    cout << setw(4) << (fAB ? "A" : "B") << setw(7) << (fLR ? "Left" : "Right");

  cout << endl;
}



ClassImp(NWChannelMap)

void NWChannelMap::SetMapFile(TString fileName)
{
  ifstream fileMap(fileName.Data());

  Int_t ch, pair, mod, mch, bar, ab, lr;
  while (fileMap >> ch >> pair >> mod >> mch >> bar >> ab >> lr) {
    if (ab == -1 || lr == -1) {
      ab = 0;
      lr = 0; 
    }
    auto info = new NWChannelInfo(ch, pair, mod, mch, bar, ab, lr);
    fChannelList.Add(info);
  }
}

void NWChannelMap::Clear(Option_t *opt)
{
  for (auto i = 0; i < 112; ++i) {
    auto info = (NWChannelInfo *) fChannelList[i];
    info->fTriggered = false;
  }

  fPairIDs.clear();
  fBarIDs.clear();
}

void NWChannelMap::Print(Option_t *) const
{
  for (auto i = 0; i < 112; ++i)
    ((NWChannelInfo *) fChannelList[i])->Print();;
}

void NWChannelMap::TriggerChannel(Int_t channelID)
{
  bool found = false;
  Int_t numPairs = fPairIDs.size();

  for (auto i = 0; i < numPairs; ++i)
  {
    auto id = fPairIDs.at(i);
    if (channelID == id) {
      found = true;
      break;
    }
  }

  auto info = (NWChannelInfo *) fChannelList[channelID];

  if (found) {
    fPairIDs.erase(std::remove(fPairIDs.begin(), fPairIDs.end(), channelID), fPairIDs.end());
    fBarIDs.push_back((info->fAB ? 1 : -1)*info->fBarID);
    return;
  }
  else {
    info->fTriggered = true;
    fPairIDs.push_back(info->fPairID);
  }
}

vector<Int_t> *NWChannelMap::GetBarIDs() { return &fBarIDs; }
