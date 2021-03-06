Int_t fEventID = 0;

void nx(Int_t eventID = -1) {
  if (eventID < 0) KBRun::GetRun() -> RunEve(++fEventID);
  else KBRun::GetRun() -> RunEve(fEventID = eventID);
  cout << "Event " << fEventID << endl;
}

void eve(TString name = "/Users/ejungwoo/KEBI/data/NeutronWall.mc.root")
{
  auto run = new KBRun();
  run -> SetInputFile(name);
  run -> SetTag("eve");
  run -> AddDetector(new NWDetector());
  run -> Init();
  run -> RunEve(fEventID);
}
