#include "NWBar.hh"
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(NWBar)

void NWBar::Clear(Option_t *opt)
{
  fLeft = nullptr;
  fRight = nullptr;

  fWallID = 1;
  fBarID = -1;

  fGeoMean = -999;
  fTDCDiff = -999;
  fADCDiff = -999;

  fGeoMeanTotal = -999;
  fGeoMeanPart = -999;
}

void NWBar::Copy(TObject &obj) const
{
  auto pair = (NWBar &) obj;

  pair.SetLeft(fLeft);
  pair.SetRight(fRight);

  pair.SetWallID(fWallID);
  pair.SetBarID(fBarID);

  pair.SetGeoMean(fGeoMean);
  pair.SetTDCDiff(fTDCDiff);
  pair.SetADCDiff(fTDCDiff);

  pair.SetGeoMeanTotal(fGeoMeanTotal);
  pair.SetGeoMeanPart(fGeoMeanPart);
}

void NWBar::Print(Option_t *) const
{
  cout << "[NWBar] " << setw(3) << fBarID << endl;
  if (fLeft != nullptr)  cout << "            LEFT: " << setw(3) << fLeft -> GetID() << endl;
  if (fRight != nullptr) cout << "           RIGHT: " << setw(3) << fRight -> GetID() << endl;
}
