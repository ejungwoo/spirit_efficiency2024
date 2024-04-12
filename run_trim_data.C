void run_trim_data
(
  int fRunNo = 2384,
  const char *fAnaName = "efficiency2024",
  TString in = "data/run2384_s0.conc.exp.root",
  TString fPathOut = "./data"
)
{
  TString spiritroot = TString(gSystem -> Getenv("VMCWORKDIR"))+"/";
  TString pathToParameters = spiritroot + "parameters/";
  TString versionOut;
  std::ifstream(spiritroot+"VERSION.compiled") >> versionOut;

  TString systemDB = "systemDB.csv";
  TString runDB = "runDB.csv";
  TString fSystemDB = pathToParameters + systemDB;
  TString fRunDB = spiritroot + "parameters/" + runDB;

  auto paramSetter = new STParameters(fRunNo, fSystemDB, fRunDB);
  //auto fParameterFile = paramSetter -> GetParameterFile();
  auto systemAll = paramSetter -> GetSystemID();

  int fSystem1 = int(systemAll/1000);

  TString beam = Form("/data/RB230064/jhpark/common/BeamData/beam_run%d.ridf.root", fRunNo);
  TString beamCut = "/data/RB230064/jhpark/common/beamGate.root";
  TString beamCutName = Form("sigma30_%dSn", fSystem1);

  TString pathToOutSys;
  TString pathToLogSys;

  pathToOutSys = fPathOut + "/";
  pathToLogSys = fPathOut + "/log/";
  //if (fPathOut.EndsWith(Form("Sn%d",fSystem1))) {
  //  pathToOutSys = fPathOut + "/";
  //  pathToLogSys = fPathOut + "/log/";
  //}
  //else if (fPathOut.EndsWith(Form("Sn%d/",fSystem1))) {
  //  pathToOutSys = fPathOut + "";
  //  pathToLogSys = fPathOut + "log/";
  //}
  //else if (fPathOut.EndsWith("/")) {
  //  pathToOutSys = Form("%s/Sn%d/",fPathOut.Data(),fSystem1);
  //  pathToLogSys = Form("%s/Sn%d/log/",fPathOut.Data(),fSystem1);
  //}
  //else if (fPathOut.EndsWith("/")) {
  //  pathToOutSys = Form("%s/Sn%d/",fPathOut.Data(),fSystem1);
  //  pathToLogSys = Form("%s/Sn%d/log/",fPathOut.Data(),fSystem1);
  //}
  //else if (fPathOut.IsNull()) {
  //  pathToOutSys = Form("%s/%s/Sn%d/",spiritroot.Data(),fAnaName,fSystem1);
  //  pathToLogSys = Form("%s/%s/Sn%d/log/",spiritroot.Data(),fAnaName,fSystem1);
  //}
  //else {
  //  pathToOutSys = Form("/%s/Sn%d/",fAnaName,fSystem1);
  //  pathToLogSys = Form("/%s/Sn%d/log/",fAnaName,fSystem1);
  //}
  gSystem -> Exec(TString("mkdir -p ")+pathToOutSys);

  TString par = pathToParameters + "/ST.parameters.par";
  TString geo = spiritroot+"geometry/geomSpiRIT.man.root"; 
  TString out = pathToOutSys+"run"+fRunNo+".trimmed.root";
  TString log = pathToLogSys+"run"+fRunNo+".trimmed.log";

  FairLogger *logger = FairLogger::GetLogger();
  logger -> SetLogToScreen(true);

  FairParAsciiFileIo* parReader = new FairParAsciiFileIo();
  parReader -> open(par);

  TChain chain("spirit");
  // same file is added twice because the conc files may have either of those tree name
  chain.Add(in+"/cbmsim");
  chain.Add(in+"/spirit");
 
  FairRunAna* run = new FairRunAna();
  run -> SetGeomFile(geo);
  run -> SetOutputFile(out);
  run -> GetRuntimeDb() -> setSecondInput(parReader);

  auto reader = new STConcReaderTask();
  reader -> SetChain(&chain);

  //auto eventFilter = new STFilterEventTask();
  //int multMin = 39, multMax = 100;
  //switch(systemAll)
  //{
  //  case 124112: 
  //    eventFilter -> SetBeamFor124Star(pathToParameters+"/isotopesCutG124.root");
  //    //eventFilter -> SetBeamCut(beamCut, beamCutName); 
  //    eventFilter -> SetVertexCut(-17.84, -11.69);
  //    //eventFilter -> SetVertexBDCCut(0.0974, 0.848*3, 0.689, 3*0.803);
  //    eventFilter -> SetVertexBDCCut(0.0, 0.848*3, 0.0, 3*0.803);
  //    eventFilter -> SetMultiplicityCut(multMin, multMax, 20);
  //    break;
  //  case 132124: 
  //    eventFilter -> SetBeamCut(beamCut, beamCutName); 
  //    eventFilter -> SetVertexCut(-18.480, -11.165);
  //    //eventFilter -> SetVertexBDCCut(2.69e-1, 3*0.988, 3.71e-1, 3*0.7532);
  //    eventFilter -> SetVertexBDCCut(0, 3*0.988, 0, 3*0.7532);
  //    eventFilter -> SetMultiplicityCut(multMin, multMax, 20);
  //    break;
  //  case 112124: 
  //    eventFilter -> SetBeamCut(beamCut, beamCutName); 
  //    eventFilter -> SetVertexCut(-16.944, -11.727);
  //    //eventFilter -> SetVertexBDCCut(-1.55e-1, 3*0.832, -3.18, 3*0.997);
  //    eventFilter -> SetVertexBDCCut(0.0, 3*0.832, 0.0, 3*0.997);
  //    eventFilter -> SetMultiplicityCut(multMin, multMax, 20);
  //    break;
  //  case 108112: 
  //    eventFilter -> SetBeamCut(beamCut, beamCutName);
  //    eventFilter -> SetVertexCut(-18.480, -11.165);
  //    //eventFilter -> SetVertexBDCCut(-9.25e-3, 3*0.933, -2.80478, 3*0.8424);
  //    eventFilter -> SetVertexBDCCut(0.0, 3*0.933, 0.0, 3*0.8424);
  //    eventFilter -> SetMultiplicityCut(multMin, multMax, 20);
  //    break;
  //}

  //eventFilter -> SetVertexXYCut(-15, 15, -225, -185);
  //eventFilter -> SetRejectBadEvents();

  //auto bdcInfo = new STAddBDCInfoTask();
  //bdcInfo -> SetRunNo(fRunNo);
  //bdcInfo -> SetBeamFile(beam);

  run -> AddTask(reader);
  //run -> AddTask(eventFilter);
  //run -> AddTask(bdcInfo);

  run -> Init();
  run -> Run(0, chain.GetEntries());

  cout /*<< "Log   : " */ << log << endl;
  cout /*<< "Input : " */ << in << endl;
  cout /*<< "Output: " */ << out << endl;

  gApplication -> Terminate();
}
