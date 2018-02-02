void geo()
{
  auto nw = new NWDetector();
  nw -> AddParameterFile("NeutronWall.par");
  nw -> Init();
  for (auto i = 0; i < nw -> GetNPlanes(); ++i) {
    auto plane = nw -> GetDetectorPlane(i);
    plane -> GetCanvas();
    auto hist = plane -> GetHist();
    for (auto bin = 0; bin < 1000; ++bin)
      hist -> SetBinContent(bin+1,bin);
    hist -> Draw("colz");
    plane -> DrawFrame();
  }
  gStyle -> SetPalette(kBird);
}
