void make_test_track_trees()
{
    //int numFiles = 1000000;
    //int numEvents = 10000;
    //int numFiles = 10;

    int numPt = 3;
    int numRap = 3;
    int numPhi = 3;

    double ptMin = 0;
    double ptMax = 2.5;
    double rapMin = -1;
    double rapMax = 2.5;
    double phiMin = -TMath::Pi();
    double phiMax = TMath::Pi();

    double ptDiff = (ptMax - ptMin)/numPt;
    double rapDiff = (rapMax - rapMin)/numRap;
    double phiDiff = (phiMax - phiMin)/numPhi;

    cout << "Number of events : " << numPt * numRap * numPhi << endl;

    double particleMass[] = {0.9382720813,1.8756127,2.8089211,2.8083913,3.7273793};
    double pt,px,py,pz,energy,rap,mass,alpha,beta,phi;
    TFile *file;
    TNtuple *tree;

    for (auto system : {108,132})
    {
        double beamAngle = -44.06/1000;// rad, Sn-132
        if (system==108)
            beamAngle = -55.19/1000;// rad, Sn-108

        for (auto pid : {0,1,2,3,4})
        {
            auto mass = particleMass[pid];

            //for (auto iFile=0; iFile<numFiles; ++iFile)
            {
                gSystem -> mkdir(Form("tt_system%d/pid%d",system,pid));
                //file = new TFile(Form("tt_system%d/pid%d/mc_%d_%d_%06d.root",system,pid,system,pid,iFile));
                file = new TFile(Form("tt_system%d/pid%d/mc_%d_%d.root",system,pid,system,pid),"recreate");
                tree = new TNtuple("mc","eta","px:py:pz:y");

                for (auto iPt=0; iPt<numPt; ++iPt)
                {
                    pt = ptMin + (iPt+0.5)*ptDiff;
                    for (auto iRap=0; iRap<numRap; ++iRap)
                    {
                        rap = rapMin + (iRap+0.5)*rapDiff;
                        for (auto iPhi=0; iPhi<numPhi; ++iPhi)
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

                            cout << iPt << " " << iRap << " " << iPhi << " "
                                << pt << " " << rap << " " << phi << " -> "
                                << vec.Px() << " " << vec.Py() << " " << vec.Pz() << " " << vec.Rapidity() << endl;

                            tree -> Fill((Float_t)(vec.Px()),
                                    (Float_t)(vec.Py()),
                                    (Float_t)(vec.Pz()),
                                    (Float_t)(vec.Rapidity()));
                        }
                    }
                }

                file -> Write();
                file -> Close();
            }
        }
    }
}
