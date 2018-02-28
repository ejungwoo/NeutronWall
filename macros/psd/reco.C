void reco()
{
  auto run = new KBRun();
  run -> SetRunID(133);
  run -> AddParameterFile("nscl.par");
  run -> SetTag("event");

  auto task = new NWEventBuildTask();
  task -> SetRunName("RUN_133");
  run -> Add(task);

  auto ana = new NWEventAnaTask();
  ana -> SetReuseBranch(true);
  ana -> SetFit(false);
  run -> Add(ana);

  run -> Init();
  run -> RunInRange(0,800000);
}
