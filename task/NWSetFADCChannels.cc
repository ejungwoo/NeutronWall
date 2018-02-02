#include <iomanip>
#include "KBRun.hh"
#include "KBChannelBufferS.hh"
#include "NWSetFADCChannels.hh"

ClassImp(NWSetFADCChannels)

NWSetFADCChannels::NWSetFADCChannels()
:KBTask("NWSetFADCChannels","")
{
  CreateTree();
}

NWSetFADCChannels::NWSetFADCChannels(const char *treeName)
:KBTask("NWSetFADCChannels",""), fTreeName(treeName)
{
  CreateTree();
}

bool NWSetFADCChannels::Init()
{
  InitTree();

  fNW = (NWDetector *) KBRun::GetRun() -> GetDetector();

  fChannelArray = new TClonesArray("KBChannelBufferS", 100);

  bool saveThisBranch = true;
  KBRun::GetRun() -> RegisterBranch("Channel", fChannelArray, saveThisBranch);

  return true;
}

void NWSetFADCChannels::Exec(Option_t*)
{
  fChannelArray -> Clear("C");

  auto eventID = KBRun::GetRun()->GetCurrentEventID();

  fDataTree -> GetEntry(eventID);

  vector<Int_t> oppositeChannelIDs;

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

      auto channel = (KBChannelBufferS *) fChannelArray -> ConstructedAt(iChannel);
      channel -> SetID(eventID*1000+channelID1);
      channel -> Set(240);
      for (auto tb = 0; tb < fNumTbs; ++tb)
        channel -> SetAt(fRawADC[iChannel][tb],tb);
    }
  }
}

void NWSetFADCChannels::SetVerbosity(Int_t verbose) { fVerbosity = verbose; }
void NWSetFADCChannels::CreateTree() { fDataTree = new TChain(fTreeName); }
void NWSetFADCChannels::AddData(TString fileName) { fDataTree -> Add(fileName); }

void NWSetFADCChannels::InitTree()
{
  fDataTree -> SetBranchAddress("nFADC",&fNumChannels);
  fDataTree -> SetBranchAddress("Channel",&fChannelIDs);
  fDataTree -> SetBranchAddress("FADC",&fRawADC);
}
