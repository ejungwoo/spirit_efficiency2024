void vertex_add_event_number()
{
    auto file = new TFile("/data/RB230064/ejungwoo/Vertex.root");
    auto itree108 = (TTree *) file -> Get("vtxTree_108Sn");
    auto itree132 = (TTree *) file -> Get("vtxTree_132Sn");

    auto ofile = new TFile("data/Vertex2.root","recreate");
    auto otree108 = new TTree("vtxTree_108Sn","");
    auto otree132 = new TTree("vtxTree_132Sn","");

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

    itree108 -> SetBranchAddress("beam",           &beam);
    itree108 -> SetBranchAddress("run",            &run);
    itree108 -> SetBranchAddress("raveVx",         &raveVx);
    itree108 -> SetBranchAddress("raveVy",         &raveVy);
    itree108 -> SetBranchAddress("raveVz",         &raveVz);
    itree108 -> SetBranchAddress("bdcVx",          &bdcVx);
    itree108 -> SetBranchAddress("bdcVy",          &bdcVy);
    itree108 -> SetBranchAddress("multTPC",        &multTPC);
    itree108 -> SetBranchAddress("sigma30",        &sigma30);
    itree108 -> SetBranchAddress("isGGClose",      &isGGClose);
    itree108 -> SetBranchAddress("isTPCRecoEvent", &isTPCRecoEvent);

    otree108 -> Branch("beam",           &beam);
    otree108 -> Branch("run",            &run);
    otree108 -> Branch("event",          &event);
    otree108 -> Branch("raveVx",         &raveVx);
    otree108 -> Branch("raveVy",         &raveVy);
    otree108 -> Branch("raveVz",         &raveVz);
    otree108 -> Branch("bdcVx",          &bdcVx);
    otree108 -> Branch("bdcVy",          &bdcVy);
    otree108 -> Branch("multTPC",        &multTPC);
    otree108 -> Branch("sigma30",        &sigma30);
    otree108 -> Branch("isGGClose",      &isGGClose);
    otree108 -> Branch("isTPCRecoEvent", &isTPCRecoEvent);

    itree132 -> SetBranchAddress("beam",           &beam);
    itree132 -> SetBranchAddress("run",            &run);
    itree132 -> SetBranchAddress("raveVx",         &raveVx);
    itree132 -> SetBranchAddress("raveVy",         &raveVy);
    itree132 -> SetBranchAddress("raveVz",         &raveVz);
    itree132 -> SetBranchAddress("bdcVx",          &bdcVx);
    itree132 -> SetBranchAddress("bdcVy",          &bdcVy);
    itree132 -> SetBranchAddress("multTPC",        &multTPC);
    itree132 -> SetBranchAddress("sigma30",        &sigma30);
    itree132 -> SetBranchAddress("isGGClose",      &isGGClose);
    itree132 -> SetBranchAddress("isTPCRecoEvent", &isTPCRecoEvent);

    otree132 -> Branch("beam",           &beam);
    otree132 -> Branch("run",            &run);
    otree132 -> Branch("event",          &event);
    otree132 -> Branch("raveVx",         &raveVx);
    otree132 -> Branch("raveVy",         &raveVy);
    otree132 -> Branch("raveVz",         &raveVz);
    otree132 -> Branch("bdcVx",          &bdcVx);
    otree132 -> Branch("bdcVy",          &bdcVy);
    otree132 -> Branch("multTPC",        &multTPC);
    otree132 -> Branch("sigma30",        &sigma30);
    otree132 -> Branch("isGGClose",      &isGGClose);
    otree132 -> Branch("isTPCRecoEvent", &isTPCRecoEvent);

    event = 0;
    runPrev = -1;
    auto numEntries = itree108 -> GetEntries();
    cout << 108 << " " << numEntries << endl;
    for (auto iEntry=0; iEntry<numEntries; ++iEntry)
    {
        itree108 -> GetEntry(iEntry);
        if (runPrev!=run) {
            runPrev = run;
            event = 0;
        }
        otree108 -> Fill();
        event++;
    }

    event = 0;
    runPrev = -1;
    numEntries = itree132 -> GetEntries();
    cout << 132 << " " << numEntries << endl;
    for (auto iEntry=0; iEntry<numEntries; ++iEntry)
    {
        itree132 -> GetEntry(iEntry);
        if (runPrev!=run) {
            runPrev = run;
            event = 0;
        }
        otree132 -> Fill();
        event++;
    }

    ofile -> cd();
    otree108 -> Write();
    otree132 -> Write();
}
