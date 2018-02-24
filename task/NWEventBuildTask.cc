#include <iomanip>
#include "KBRun.hh"
#include "KBChannelBufferS.hh"
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

  //fTreeTCB -> GetEntry(fNumEvents-1); fTriggerIDEnd   = fTCB -> tcb_trigger_number;
  //fTreeTCB -> GetEntry(0);            fTriggerIDStart = fTCB -> tcb_trigger_number;
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
      //kb_info << "Input tree, channel-" << channelID << endl;
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



  fChannelArray = new TClonesArray("KBChannel", 112);
  run -> RegisterBranch("Channel", fChannelArray, true);

  return true;
}

void NWEventBuildTask::Exec(Option_t*)
{
  fChannelArray -> Clear("C");

  bool triggeredEvent = false;
  while (fEntryTCB < fNumEvents)
  {
    fMap -> Clear();
    fTreeTCB -> GetEntry(fEntryTCB);
    ++fEntryTCB;

    Long64_t triggerID = fTCB -> tcb_trigger_number;

    for (auto channelID = 0; channelID <= 103; ++channelID)
    {
      if (channelID > 47 && channelID < 56)
        continue;

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
        // TODO
        triggeredEvent = true;
        fMap -> TriggerChannel(channelID);
        //kb_info << "triggered channel-" << channelID << endl;
      }
    }

    auto barIDs = fMap -> GetBarIDs();
    if (barIDs->size() == 0)
      triggeredEvent = false;
    else {
      kb_info << "triggerID-" << triggerID << ", triggerd by bar: ";
      for (auto id : *barIDs)
        kb_raw << id << " ";
      kb_raw << endl;
    }

    if (triggeredEvent)
      break;
  }
  if (fEntryTCB >= fNumEvents)
    KBRun::GetRun() -> EndOfEvent();



  /*
  for (auto channelID = 0; channelID <= 103; channelID++)
  {
    if (channelID > 47 && channelID < 56)
      continue;

    fFADCCh[channelID] -> tnum == fTriggerID;

    while (fFADCCh[channelID] -> tnum < fTriggerID) {
      countFADC[channelID]++;
      if (countFADC[channelID] >= fTreeCh[channelID] -> GetEntries()) 
        break;

      fTreeCh[channelID] -> GetEntry(countFADC[channelID]);
    }

    if (fFADCCh[channelID] -> tnum == fTriggerID)
    {
      bool bcrossed = false; 
      if (SPCHMap[channelID] >= 0) // Special Channel 
      {
        SPCh[numSpecial] = channelID;
        for (auto tb = 0; tb < kNumTbs; tb++)
        {
          SPWAVE[numSpecial][tb]	= fFADCCh[channelID] -> ADC[tb];

          if(tb>0 && SPWAVE[numSpecial][tb]>=1400 && SPWAVE[numSpecial][tb-1] < 1400 && !bcrossed)
          {
            fSpecialTime[numSpecial] = tb*2 - 2*(SPWAVE[numSpecial][tb]-1400)/(SPWAVE[numSpecial][tb]-SPWAVE[numSpecial][tb-1]);
            bcrossed = true;	      
          }
        }
        ++numSpecial;
      }
      else
      {
        if( channelID < 0 || channelID >= 112 ){ continue; }
        Ch[nWAVE] = channelID;	  
        CH_ttime[nWAVE] = fFADCCh[channelID] -> ttime;
        for( int ip = 0; ip < kNumTbs; ip++){
          WAVE[nWAVE][ip]	= fFADCCh[channelID] -> ADC[ip];
        }
        nWAVE++;
        Int_t RLID = NWRL[channelID]-1;   // 0 : R, 1 :L
        Int_t ABID = NWABMap[channelID]-1;// 0 : A, 1 :B 
        Int_t BARID= BarIDMap[channelID]; // From 0 to 24;
        if( ABID == 0 ){

          TMPAID[BARID]=BARID;
          TMPANHIT[BARID]++;
          TMPAModSum [BARID][RLID] =fFADCCh[channelID] -> ADCSum;
          TMPAModPart[BARID][RLID] =fFADCCh[channelID] -> ADCPart;
          TMPAModTime[BARID][RLID] =fFADCCh[channelID] -> ADCTime;
          TMPAModPeak[BARID][RLID] =fFADCCh[channelID] -> ADCPeak;
          TMPAModPed [BARID][RLID] =fFADCCh[channelID] -> ADCPed; 	

        }else if( ABID == 1 ){

          TMPBID[BARID]=BARID;
          TMPBNHIT[BARID]++;
          TMPBModSum [BARID][RLID] =fFADCCh[channelID] -> ADCSum;
          TMPBModPart[BARID][RLID] =fFADCCh[channelID] -> ADCPart;
          TMPBModTime[BARID][RLID] =fFADCCh[channelID] -> ADCTime;
          TMPBModPeak[BARID][RLID] =fFADCCh[channelID] -> ADCPeak;
          TMPBModPed [BARID][RLID] =fFADCCh[channelID] -> ADCPed; 	

        }	  
      }
    }
    else {
      breakFromEvent = true;
      break;
    }
  }
  */
  //auto eventID = KBRun::GetRun() -> GetCurrentEventID();

  //fDataTree -> GetEntry(eventID);
}
