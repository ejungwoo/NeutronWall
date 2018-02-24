#include <iomanip>
#include "KBRun.hh"
#include "NWBar.hh"
#include "NWChannel.hh"
#include "NWEventBuildTask.hh"

ClassImp(NWEventBuildTask)

NWEventBuildTask::NWEventBuildTask()
:KBTask("NWEventBuildTask","")
{
}

bool NWEventBuildTask::Init()
{

  auto run = KBRun::GetRun();
  auto par = run -> GetParameterContainer();

  TString nameMap = par -> GetParString("channelMapFile");
  TString pathTCB = par -> GetParString("TCBCONVDATA");
  TString pathConvData[4];
  for (auto i = 0; i < 4; i++)
    pathConvData[i] = par -> GetParString(Form("CONVDATA_%d",i));



  fMap = new NWChannelMap();
  fMap -> SetMapFile(nameMap);



  TString nameTCB = Form("%s/%s_tcb_conv.root",pathTCB.Data(),fRunName.Data());
  auto fileTCB = new TFile(nameTCB);
  if (fileTCB == nullptr) {
    kb_error << "No TCB Data " << nameTCB << endl;
    return false;
  }
  fTreeTCB = (TTree*) fileTCB -> Get("TCBTree");
  if (fTreeTCB == nullptr) {
    kb_error << "TCB tree is nullptr" << endl;
    return false;
  }

  fTCB = new TCB();
  fTreeTCB -> SetBranchAddress("tcb", &fTCB);

  fNumEvents = fTreeTCB -> GetEntries();
  run -> SetEntries(fNumEvents);
  fEntryTCB = 0;

  //fTCB -> tcb_ttime; //XXX WHAT IS THIS?

  for (auto moduleID = 0; moduleID < 28; ++moduleID)
  {
    auto diskID = (int)(moduleID/7);
    TString nameFile = Form("%s/%s_fadc_%d_conv.root",pathConvData[diskID].Data(),fRunName.Data(),moduleID+1);

    auto file = new TFile(nameFile, "read");
    if (file == nullptr) {
      kb_error << "No Raw Data: " << nameFile << endl;
      return false;
    }
    kb_info << "Input file: " << nameFile << endl;

    for (auto iChannel = 0; iChannel < 4; ++iChannel) {
      TString nameTree = Form("Tree_%d", iChannel);
      auto channelID = moduleID * 4 + iChannel;
      fTreeCh[channelID] = (TTree*) file -> Get(nameTree.Data());

      if (fTreeCh[channelID] == nullptr) {
        kb_error << "No Tree: " << nameTree << endl;
        return false;
      }
    }
  }

  for (auto channelID = 0; channelID <= 103; channelID++)
  {
    if (channelID > 47 && channelID < 56)
      continue;
    fFADCCh[channelID] = new FADC();
    fTreeCh[channelID] -> SetBranchAddress("wave",&(fFADCCh[channelID]));

    fNumEntriesCh[channelID] = fTreeCh[channelID] -> GetEntries();

    fEntryCh[channelID] = 0;
    if (fNumEntriesCh[channelID] > 0)
      fTreeCh[channelID] -> GetEntry(fEntryCh[channelID]);
    else {
      kb_error << "No Entries in channel-" << channelID << endl;
      return false;
    }
    kb_out << "FADC channel-" << left << setw(7) << channelID << fNumEntriesCh[channelID] << endl;
  }



  fBarArray = new TClonesArray("NWBar", 112);
  run -> RegisterBranch("Bar", fBarArray, true);

  return true;
}

void NWEventBuildTask::Exec(Option_t*)
{
  fBarArray -> Clear("C");

  bool triggeredEvent = false;
  while (fEntryTCB < fNumEvents)
  {
    fMap -> Clear();
    fTreeTCB -> GetEntry(fEntryTCB);
    ++fEntryTCB;

    Long64_t triggerID = fTCB -> tcb_trigger_number;

    //Int_t channelIdx[112] = {0};

    for (auto channelID = 0; channelID <= 103; ++channelID)
    {
      if (channelID > 47 && channelID < 56)
        continue;

      //kb_debug << channelID << " " << (fFADCCh[channelID]->mid-1)*4 + (fFADCCh[channelID]->cid-1) << endl;

      bool triggeredCh = false;
      while (1)
      {
        auto triggerIDCh = fFADCCh[channelID] -> tnum;

        if (triggerIDCh < triggerID) {
          fEntryCh[channelID] = fEntryCh[channelID] + 1;
          fTreeCh[channelID] -> GetEntry(fEntryCh[channelID]);
          continue;
        }
        else if (triggerIDCh == triggerID) {
          triggeredCh = true;
          break;
        }
        else if (triggerIDCh > triggerID)
          break;
      }

      if (triggeredCh) {
        triggeredEvent = true;
        fMap -> TriggerChannel(channelID);
        //kb_info << "triggered channel-" << channelID << endl;
      }
    }

    auto barIDs = fMap -> GetBarIDs();
    if (barIDs->size() == 0)
      triggeredEvent = false;
    else // Take Data
    {
      kb_info << "triggerID-" << triggerID << ", triggerd by bar: ";
      for (auto id : *barIDs)
        kb_raw << id << " ";
      kb_raw << endl;

      for (auto id : *barIDs) {
        Int_t leftID, rightID;
        bool ab = true;
        if (id < 0) {
          ab = false;
          id = -id;
        }

        fMap -> FindChannelByBar(ab, id, leftID, rightID);
        ///////////////////////////////////////////////////////////////////
        // COPY Data
        ///////////////////////////////////////////////////////////////////
        auto bar = (NWBar *) fBarArray -> ConstructedAt(fBarArray->GetEntries());
        bar -> SetWall(ab);
        bar -> SetBarID(id);
        //kb_debug << leftID << " " << rightID << endl;

        auto left = new NWChannel();
        left -> Set(240);
        for (auto tb = 0; tb < 240; ++tb)
          left -> SetAt(fFADCCh[leftID] -> ADC[tb], tb);

        auto right = new NWChannel();
        right -> Set(240);
        for (auto tb = 0; tb < 240; ++tb)
          right -> SetAt(fFADCCh[rightID] -> ADC[tb], tb);

        bar -> SetLeft(left);
        bar -> SetRight(right);
        ///////////////////////////////////////////////////////////////////
      }
    }

    if (triggeredEvent)
      break;
  }

  if (fEntryTCB >= fNumEvents)
    KBRun::GetRun() -> EndOfEvent();
}
