// random_shuffle example
#include <iostream>  // std::cout
#include <algorithm> // std::random_shuffle
#include <vector>    // std::vector
#include <ctime>     // std::time
#include <cstdlib>   // std::rand, std::srand

using namespace std;

// random generator function:
int randgen(int i) { return std::rand() % i; }

void RWR_inidata()
{
    system("mkdir -p ./data");

    Long64_t Run_Date = 20200826;
    Long64_t event;
    Int_t Nroll = 10;
    Int_t Ncoll;
    Float_t L = 22.5; //unit: cm
    Int_t Ntarget = 4;
    Float_t Dinj_truth = 1.6; //unit: cm
    Float_t Dtar_truth = 1.6; //unit: cm

    fstream fp;
    fp.open(Form("./data/RWR_data_%lld.csv", Run_Date), ios::out | ios::trunc);
    // fp << "Run_Date, event, Nroll, Ncoll, L, Ntarget, Dinj_truth, Dtar_truth" << endl;

    vector<int> vec_Ncoll{2, 0, 3, 7, 14, 11, 8, 8, 1, 1, 1};
    vector<int> event_Ncoll;
    event_Ncoll.clear();

    Int_t Nev = 0;
    for (size_t i = 0; i < vec_Ncoll.size(); i++)
    {
        Nev += vec_Ncoll[i];
        for (size_t j = 0; j < vec_Ncoll[i]; j++)
        {
            event_Ncoll.push_back(i);
        }
    }

    srand(time(0));
    std::random_shuffle(event_Ncoll.begin(), event_Ncoll.end(), randgen);
    for (size_t i = 0; i < event_Ncoll.size(); i++)
    {
        fp << Run_Date << ", " << i << ", " << Nroll << ", " << event_Ncoll[i] << ", " << L << ", " << Ntarget << ", " << Dinj_truth << ", " << Dtar_truth << endl;
    }
    fp.close();
}