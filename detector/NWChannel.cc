#include "NWChannel.hh"
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(NWChannel)

void NWChannel::Clear(Option_t *opt)
{
  KBChannelBufferS::Clear(opt);

  fModuleID = -1;
  fMChannelID = -1;
  fPedestal = -9999;
  fTDC = -9999;
  fADC = -9999;
  fADCTotalSum = -9999;
  fADCPartSum = -9999;

  fIsFitted = false;
  fPosition = -9999;
  fAlpha = -9999;
  fChiSquare = -9999;
}

void NWChannel::Copy(TObject &obj) const
{
  KBChannelBufferS::Copy(obj);

  auto ch = (NWChannel &) obj;
  ch.SetModuleID(fModuleID);
  ch.SetMChannelID(fMChannelID);
  ch.SetPedestal(fPedestal);
  ch.SetTDC(fTDC);
  ch.SetADC(fADC);
  ch.SetADCTotalSum(fADCTotalSum);
  ch.SetADCPartSum(fADCPartSum);

  ch.SetIsFitted(fIsFitted);
  ch.SetPosition(fPosition);
  ch.SetAlpha(fAlpha);
  ch.SetChiSquare(fChiSquare);
}

void NWChannel::Print(Option_t *) const
{
  cout << "[NWChannel] " << setw(3) << fID << ": " << setw(3) << fModuleID << "|" << setw(3) << fMChannelID
       << ", PTASP: " << fPedestal << " | " << fTDC << "," << fADC << " | " << fADCTotalSum << "," << fADCPartSum << endl;
}
