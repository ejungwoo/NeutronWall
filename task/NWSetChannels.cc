#include <iomanip>
#include "KBRun.hh"
#include "NWChannel.hh"
#include "NWSetChannels.hh"

ClassImp(NWSetChannels)

NWSetChannels::NWSetChannels(const char *treeName)
:KBTask("NWSetChannels",""), fTreeName(treeName)
{
  CreateTree();
}

bool NWSetChannels::Init()
{
  InitTree();

  fNW = (NWDetector *) KBRun::GetRun() -> GetDetector();

  fChannelArray = new TClonesArray("NWChannel", 100);

  bool saveThisBranch = true;
  KBRun::GetRun() -> RegisterBranch("Channel", fChannelArray, saveThisBranch);

  return true;
}

void NWSetChannels::Exec(Option_t*)
{
  fChannelArray -> Clear("C");

  fDataTree -> GetEntry(KBRun::GetRun()->GetCurrentEventID());
  vector<Int_t> oppositeChannelIDs;

  cout << "  [" << this -> GetName() << "] Number of channels: " << fNumChannels << endl;
  for (auto iChannel = 0; iChannel < fNumChannels; ++iChannel)
  {
    auto channelID1 = fChannelIDs[iChannel]; // this channel
    for (auto channelID : oppositeChannelIDs)
    {
      if (channelID == channelID1) {
        // TODO something
      } else {
        auto channelID2 = channelID1;  // opposite channel
        if (channelID2 > 48) channelID2 = channelID1 - 56;
        if (channelID2 < 24) channelID2 = channelID1 + 24;
        else                 channelID2 = channelID1 - 24;
        oppositeChannelIDs.push_back(channelID2);

        if (fVerbosity > 1)
          cout << std::setw(4) << channelID1 << "(" << std::setw(3) << channelID2 << ")";
      }
    }

    auto channel = (NWChannel *) fChannelArray -> ConstructedAt(iChannel);
    channel -> SetID(channelID1);
    channel -> Set(240);
    for (auto tb = 0; tb < fNumTbs; ++tb)
      channel -> SetAt(fRawADC[iChannel][tb],tb);
  }
}

void NWSetChannels::SetVerbosity(Int_t verbose) { fVerbosity = verbose; }
void NWSetChannels::CreateTree() { fDataTree = new TChain(fTreeName); }
void NWSetChannels::AddData(TString fileName) { fDataTree -> Add(fileName); }

void NWSetChannels::InitTree()
{
  fDataTree -> SetBranchAddress("nFADC",&fNumChannels);
  fDataTree -> SetBranchAddress("Channel",&fChannelIDs);
  fDataTree -> SetBranchAddress("FADC",&fRawADC);

  KBRun::GetRun() -> SetEntries(fDataTree -> GetEntries());
}
