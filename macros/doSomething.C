void doSomething()
{
  auto run = new KBRun();
  run -> SetRunID(0);
  run -> SetTag("test");
  run -> AddParameterFile("NeutronWall.par");
  run -> AddDetector(new NWDetector());

  auto task = new NWDoSomethingTask();
  run -> Add(task);

  run -> Init();
  run -> Run();
}
