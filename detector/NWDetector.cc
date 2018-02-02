#include "NWDetector.hh"
#include "NWDetectorPlane.hh"
#include "KBChannelBufferD.hh"
#include <iostream>
using namespace std;

NWDetector::NWDetector()
:KBDetector("NWDetector","NW Detector")
{
}

bool NWDetector::Init()
{
  if (BuildGeometry() == false)
    return false;

  return true;
}

bool NWDetector::BuildGeometry()
{
  auto numWalls = fPar -> GetParInt("numWalls");
  for (auto iWall = 0; iWall < numWalls; ++iWall)
  {
  }

  if (fGeoManager == nullptr) {
    new TGeoManager();
    fGeoManager = gGeoManager;
    fGeoManager -> SetVerboseLevel(0);
    fGeoManager -> SetNameTitle("NW Detector", "NW Detector Geometry");
  }

  auto top = new TGeoVolumeAssembly("TOP");
  fGeoManager -> SetTopVolume(top);
  fGeoManager -> SetTopVisible(true);

  auto wall = new TGeoVolumeAssembly("Wall");

  auto nTbs = fPar -> GetParInt("numTimeBuckets");
  for (auto iWall = 0; iWall < numWalls; ++iWall)
  {
    top -> AddNode(wall, iWall);

    auto xDetector  = fPar -> GetParDouble(Form("xDetector%d",  iWall));
    auto yDetector  = fPar -> GetParDouble(Form("yDetector%d",  iWall));
    auto zDetector  = fPar -> GetParDouble(Form("zDetector%d",  iWall));
    auto dxDetector = fPar -> GetParDouble(Form("dxDetector%d", iWall));
    auto dyDetector = fPar -> GetParDouble(Form("dyDetector%d", iWall));
    auto dzDetector = fPar -> GetParDouble(Form("dzDetector%d", iWall));
    auto lxDetector = fPar -> GetParDouble(Form("lxDetector%d", iWall));
    auto lyDetector = fPar -> GetParDouble(Form("lyDetector%d", iWall));
    auto lzDetector = fPar -> GetParDouble(Form("lzDetector%d", iWall));

    auto nameOfMaterial = fPar -> GetParString(Form("nameOfMaterial%d", iWall));
    auto material = new TGeoMedium(nameOfMaterial.Data(), 1, new TGeoMaterial(nameOfMaterial.Data()));

    auto detector = fGeoManager -> MakeBox(Form("Wall%d_Detector", iWall),material,lxDetector/2,lyDetector/2,lzDetector/2);
    detector -> SetVisibility(true);
    detector -> SetLineColor(kBlue-10);
    detector -> SetTransparency(90);

    auto numReadouts = fPar -> GetParInt(Form("numReadouts%d", iWall));
    for (auto iReadout = 0; iReadout < numReadouts; ++iReadout)
    {
      auto plane = new NWDetectorPlane();
      AddPlane(plane);

      plane -> SetParameterContainer(fPar);
      plane -> SetPlaneID(iWall);

      if (iReadout == 0) plane -> IsTopReadout(true);
      else               plane -> IsTopReadout(false);

      auto numDetectors = fPar -> GetParInt(Form("numDetectors%d", iWall));
      for (auto iCopy = 0; iCopy < numDetectors; ++iCopy)
      {
        auto pos1 = TVector3(xDetector + iCopy*dxDetector, yDetector + iCopy*dyDetector, zDetector + iCopy*dzDetector);
        if (iReadout == 0) {
          auto translation = new TGeoTranslation(pos1.X(), pos1.Y(), pos1.Z());
          wall -> AddNode(detector, iCopy, translation);
        }

        auto channelID = iWall*100 + iCopy;
        auto pos2 = TVector3(lxDetector/2,lyDetector/2,lzDetector/2);

        auto channel = new KBChannelBufferD();
        channel -> SetID(channelID);
        channel -> SetPos1(pos1); 
        channel -> SetPos2(pos2); 
        channel -> Set(nTbs);

        plane -> AddChannel(channel);
      }
      plane -> Init();
    }
  }

  fNPlanes = fDetectorPlaneArray -> GetEntries();

  auto materialTarget = new TGeoMedium("matTarget", 1, new TGeoMaterial("matTarget"));
  auto target = fGeoManager -> MakeBox("target",materialTarget,20,20,5);
  target -> SetVisibility(true);
  target -> SetLineColor(kBlack);
  top -> AddNode(target, 1);  

  fGeoManager -> CloseGeometry();

  return true;
}

bool NWDetector::BuildDetectorPlane()
{
  return true;
}
