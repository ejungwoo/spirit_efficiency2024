void make_test_track_trees()
{
    TString tag = "n1333";
    int numEventsPerBin = 1;
    int numPt = 3;
    int numRap = 3;
    int numPhi = 3;

    double ptMin = 0;
    double ptMax = 2.5;
    double rapCMMin = -1;
    double rapCMMax = 2.5;
    double phiMin = -TMath::Pi();
    double phiMax = TMath::Pi();

    double ptDiff = (ptMax - ptMin)/numPt;
    double phiDiff = (phiMax - phiMin)/numPhi;

    cout << "Number of events : " << numPt * numRap * numPhi << endl;

    int pdgArray[] = {2212,1000010020,1000010030,1000020030,1000020040};
    double particleMass[] = {0.9382720813,1.8756127,2.8089211,2.8083913,3.7273793};
    double pt,px,py,pz,energy,rap,mass,alpha,beta,phi;
    int iPt, iRap, iPhi;
    TFile *file;
    TTree *tree;

    for (auto system : {108,132})
    {
        double beamAngle = -44.06/1000;
        double rapMin = (rapCMMin + 1) * 0.383431;
        double rapMax = (rapCMMax + 1) * 0.383431;
        if (system==108) {
            beamAngle = -55.19/1000;
            rapMin = (rapCMMin + 1) * 0.365863;
            rapMax = (rapCMMax + 1) * 0.365863;
        }
        double rapDiff = (rapMax - rapMin)/numRap;
        cout << "system " << system << " : rapidity = " << rapMin << " - " << rapMax << endl;

        for (auto pid : {0,1,2,3,4})
        {
            int pdg = pdgArray[pid];
            auto mass = particleMass[pid];

            gSystem -> mkdir(Form("tt_system%d/pid%d",system,pid));
            TString rootFileName = Form("tt_system%d/pid%d/mc_%d_%d.root",system,pid,system,pid);
            cout << rootFileName << endl;

            file = new TFile(rootFileName,"recreate");
            tree = new TTree("track", tag + " " + system + " " + pid);
            tree -> Branch("ipt",&iPt);
            tree -> Branch("irap",&iRap);
            tree -> Branch("iphi",&iPhi);
            tree -> Branch("pt",&pt);
            tree -> Branch("rap",&rap);
            tree -> Branch("phi",&phi);
            tree -> Branch("alpha",&alpha);
            tree -> Branch("beta",&beta);
            tree -> Branch("energy",&energy);
            tree -> Branch("px",&px);
            tree -> Branch("py",&py);
            tree -> Branch("pz",&pz);

            for (iPt=0; iPt<numPt; ++iPt)
            {
                pt = ptMin + (iPt+0.5)*ptDiff;
                for (iRap=0; iRap<numRap; ++iRap)
                {
                    rap = rapMin + (iRap+0.5)*rapDiff;
                    for (iPhi=0; iPhi<numPhi; ++iPhi)
                    {
                        phi = phiMin + (iPhi+0.5)*phiDiff;
                        px = pt * TMath::Cos(phi);
                        py = pt * TMath::Sin(phi);

                        alpha = TMath::Exp(2*rap);
                        beta = (alpha-1)/(alpha+1);
                        energy = sqrt((px*px+py*py+mass*mass)/(1-beta*beta));
                        pz = beta*energy;

                        TLorentzVector vec;
                        vec.SetPxPyPzE(px,py,pz,energy);
                        vec.RotateY(beamAngle);

                        tree -> Fill();

                        TString fileName = Form("tt_system%d/pid%d/gen_%s__%d_%d__%d_%d_%d.txt",system,pid,tag.Data(),system,pid,iPt,iRap,iPhi);
                        cout << fileName << setw(15) << pt << setw(15) << rap << setw(15) << px << setw(15) << py << setw(15) << pz << endl;

                        ofstream generatorFile(fileName);
                        generatorFile << Form("NEvent        %d", numEventsPerBin) << endl;
                        generatorFile << Form("Momentum3     %.4f %.4f %.4f", px, py, pz) << endl;
                        //generatorFile << Form("VertexFile    VertexLocationEmbedding2024.txt") << endl;
                        generatorFile << Form("VertexFile    VertexLocation.txt") << endl;
                        generatorFile << Form("Particle      %d", pdg) << endl;
                    }
                }
            }

            file -> Write();
            file -> Close();
        }
    }
}
