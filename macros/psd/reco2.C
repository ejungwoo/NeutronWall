void reco2()
{
  auto run = new KBRun();
  run -> SetInputFile("/Users/ejungwoo/KEBI/data/run0133.w0_b18.root");
  run -> SetOutputFile("/Users/ejungwoo/KEBI/data/run0133.w0_b18_fit.root");
  run -> AddParameterFile("nscl.par");
  run -> SetTag("psa");

  auto ana = new NWEventAnaTask();
  ana -> SetReuseBranch(false);
  run -> Add(ana);

  run -> Init();
  //run -> Run();
  run -> RunInRange(0,999);
}
