void extract_run_event_vpos()
{
    auto file = new TFile("data/Vertex2.root");
    auto tree108 = (TTree *) file -> Get("vtxTree_108Sn");
    auto tree132 = (TTree *) file -> Get("vtxTree_132Sn");

    int beam;
    int run;
    int event;
    int runPrev;
    double raveVx;
    double raveVy;
    double raveVz;
    double bdcVx;
    double bdcVy;
    int multTPC;
    bool sigma30;
    bool isGGClose;
    bool isTPCRecoEvent;

    tree108 -> SetBranchAddress("beam",           &beam);
    tree108 -> SetBranchAddress("run",            &run);
    tree108 -> SetBranchAddress("event",          &event);
    tree108 -> SetBranchAddress("raveVx",         &raveVx);
    tree108 -> SetBranchAddress("raveVy",         &raveVy);
    tree108 -> SetBranchAddress("raveVz",         &raveVz);
    tree108 -> SetBranchAddress("bdcVx",          &bdcVx);
    tree108 -> SetBranchAddress("bdcVy",          &bdcVy);
    tree108 -> SetBranchAddress("multTPC",        &multTPC);
    tree108 -> SetBranchAddress("sigma30",        &sigma30);
    tree108 -> SetBranchAddress("isGGClose",      &isGGClose);
    tree108 -> SetBranchAddress("isTPCRecoEvent", &isTPCRecoEvent);

    int multCut = 55;
    tree108 -> Draw(">>lkentrylist",Form("run==2384&&multTPC>%d",multCut),"entrylist");
    auto entryList = (TEntryList*) gDirectory -> Get("lkentrylist");
    auto numEntries = entryList -> GetN();
    cout << tree108 -> GetEntries() << " " << numEntries << endl;
    tree108 -> SetEntryList(entryList);

    TString fileName = Form("config/VertexLocation_run2384_cut%d_n%lld.txt",multCut,numEntries);
    cout << fileName << endl;
    ofstream fileVertexLocation(fileName);

    fileVertexLocation << "#RunNum   EventNum    x(mm)    y(mm)    z(mm)" << endl;
    for (auto iEntry=0; iEntry<numEntries; ++iEntry) {
        auto entryNumber = tree108 -> GetEntryNumber(iEntry);
        tree108 -> LoadTree(entryNumber);
        tree108 -> GetEntry(entryNumber);
        fileVertexLocation << run << " " << event << " " << 0.1*raveVx << " " << 0.1*raveVy << " " << 0.1*raveVz << endl;
    }
}
