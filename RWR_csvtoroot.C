void RWR_csvtoroot(Long64_t Run_Date = 20200826, bool mergetree = true)
{
    system("mkdir -p ./data/");
    TFile *outfile = new TFile(Form("./data/RWR_data_%lld.root", Run_Date), "RECREATE");
    outfile->cd();
    TTree *outTree = new TTree("outTree", "outTree");
    outTree->ReadFile(Form("./data/RWR_data_%lld.csv", Run_Date), "Run_Date/L:event/L:Nroll/I:Ncoll/I:L/F:Ntarget/I:Dinj_truth/F:Dtar_truth/F");
    outTree->Write("", TObject::kOverwrite);
    outfile->Close();

    if (mergetree)
    {
        cout << "[INFO] Start doing hadd to merge data" << endl;
        system("hadd -f ./data/RWR_data_merge.root ./data/RWR_data_202*.root");
    }
}