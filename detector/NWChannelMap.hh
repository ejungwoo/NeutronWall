#ifndef NWCHANNELMAP_HH
#define NWCHANNELMAP_HH

#include "TObject.h"
#include "TObjArray.h"
#include <vector>
using namespace std;

class NWChannelInfo : public TObject
{
  public:
    NWChannelInfo();
    NWChannelInfo(int channelID, int pairID, int moduleID, int mchannelID, int barID, bool ab, bool lr);

    virtual ~NWChannelInfo() {}

    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const; 

    int fChannelID;  ///< channel ID (0-112)
    int fPairID;     ///< channel ID of opposite channel in bar
    int fModuleID;   ///< module  ID (1-28)
    int fMChannelID; ///< channel ID in module (1-4)

    int fBarID;      ///< bar ID (0-28)
    bool fAB;        ///< true: Wall-A, false: Wall-B
    bool fLR;        ///< true: left,   false: right

    int fTriggered;  ///< trigger flag

  ClassDef(NWChannelInfo, 1)
};

class NWChannelMap : public TObject 
{
  public:
    NWChannelMap() {}
    virtual ~NWChannelMap() {}

    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const; 

    void SetMapFile(TString fileName);
    void TriggerChannel(Int_t channelID);

    vector<Int_t> *GetBarIDs();

    void FindChannelByBar(bool ab, Int_t barID, Int_t &leftID, Int_t &rightID);

  private:
    TObjArray fChannelList;
    vector<Int_t> fPairIDs;
    vector<Int_t> fBarIDs;

  ClassDef(NWChannelMap, 1)
};

#endif
