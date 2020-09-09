#include "tdrstyle.C"
void RWR_plotdata(const char *infilename = "./data/RWR_data_merge.root")
{
    system("mkdir -p ./plots");

    TH1F *hM_Ncoll = new TH1F("hM_Ncoll", "hM_Ncoll", 11, -0.5, 10.5);

    TFile *infile = new TFile(infilename, "READ");
    TTree *tree = (TTree *)infile->Get("outTree");

    Int_t Ncoll;
    tree->SetBranchAddress("Ncoll", &Ncoll);

    for (Long64_t ev = 0; ev < tree->GetEntriesFast(); ev++)
    {
        tree->GetEntry(ev);
        hM_Ncoll->Fill(Ncoll);
    }

    infile->Close();

    // Make simple plot
    Float_t AxisTitleSize = 0.06, AxisLabelSize = 0.05;
    setTDRStyle();
    gStyle->SetOptStat(0);
    gStyle->SetGridStyle(1);
    gStyle->SetGridColor(17);
    hM_Ncoll->Sumw2();
    TCanvas *c = new TCanvas("c", "", 700, 500);
    // c->SetGrid();
    c->cd();
    gPad->SetRightMargin(0.05);
    gPad->SetTopMargin(0.08);
    gPad->SetLeftMargin(0.13);

    hM_Ncoll->GetXaxis()->SetTitleOffset(1.05);
    hM_Ncoll->GetYaxis()->SetTitleOffset(1.05);
    hM_Ncoll->GetXaxis()->SetNdivisions(11, 2, 0, kTRUE);
    hM_Ncoll->GetYaxis()->SetRangeUser(0., (hM_Ncoll->GetMaximum() + hM_Ncoll->GetBinError(hM_Ncoll->GetMaximumBin())) * 1.25);
    hM_Ncoll->GetXaxis()->SetTitleSize(AxisTitleSize);
    hM_Ncoll->GetXaxis()->SetLabelSize(AxisLabelSize);
    hM_Ncoll->GetYaxis()->SetTitleSize(AxisTitleSize);
    hM_Ncoll->GetYaxis()->SetLabelSize(AxisLabelSize);
    hM_Ncoll->GetXaxis()->SetTitle("Number of hits per 10 rolls");
    hM_Ncoll->GetYaxis()->SetTitle("Frequency");
    hM_Ncoll->SetLineColor(TColor::GetColor("#006E8A"));
    hM_Ncoll->SetFillColor(TColor::GetColor("#006E8A"));
    hM_Ncoll->Draw("HIST");

    gPad->RedrawAxis("g");

    TH1F *hM_Ncoll_clone = (TH1F *)hM_Ncoll->Clone("hM_Ncoll_clone");
    hM_Ncoll_clone->SetLineColor(TColor::GetColor("#00142E"));
    hM_Ncoll_clone->SetLineWidth(3);
    hM_Ncoll_clone->SetMarkerStyle(20);
    hM_Ncoll_clone->SetMarkerSize(0.1);
    hM_Ncoll_clone->Draw("PE1X0SAME");

    c->RedrawAxis();

    TF1 *f1 = new TF1("f1", "gaus", -0.5, 10.5);
    f1->SetParNames("N", "Mean", "Sigma");
    hM_Ncoll->Fit("f1", "REM0");
    gStyle->SetOptFit(0);
    f1->SetLineWidth(5);
    f1->SetLineColor(TColor::GetColor("#FEA319"));
    f1->Draw("same");

    Float_t Constant_fitr = f1->GetParameter("N");
    Float_t mean_fitr = f1->GetParameter("Mean");
    Float_t meanerr_fitr = f1->GetParError(1);
    Float_t sigma_fitr = f1->GetParameter("Sigma");
    Float_t sigmaerr_fitr = f1->GetParError(2);

    // TArrow *ar_mean = new TArrow(mean_fitr, 0.5, mean_fitr, 0.9 * c->GetUymax(), 0.05, "<|");
    TArrow *ar_mean = new TArrow(mean_fitr, Constant_fitr + 0.5, mean_fitr, 0.9 * c->GetUymax(), 0.05, "<|");
    ar_mean->SetAngle(40);
    ar_mean->SetLineWidth(4);
    ar_mean->SetFillColor(TColor::GetColor("#F96C41"));
    ar_mean->SetLineColor(TColor::GetColor("#F96C41"));
    ar_mean->Draw();
    TLatex Tl_mean;
    Tl_mean.SetTextAlign(21);
    Tl_mean.SetTextSize(0.05);
    Tl_mean.SetTextColor(TColor::GetColor("#F96C41"));
    Tl_mean.DrawLatex(mean_fitr, 0.91 * c->GetUymax(), Form("#mu = %.2f #pm %.3f", mean_fitr, meanerr_fitr));
    TLine *line_mean = new TLine(mean_fitr, 0.0, mean_fitr, Constant_fitr);
    line_mean->SetLineWidth(3);
    line_mean->SetLineStyle(kDashed);
    line_mean->SetLineColor(TColor::GetColor("#F96C41"));
    line_mean->Draw();

    Float_t y0_sigma = f1->Eval(mean_fitr - sigma_fitr);
    Float_t y1_sigma = f1->Eval(mean_fitr + sigma_fitr);
    TArrow *ar_sigma = new TArrow(mean_fitr - sigma_fitr, y0_sigma, mean_fitr + sigma_fitr, y1_sigma, 0.03, "<|>");
    ar_sigma->SetLineWidth(4);
    ar_sigma->SetFillColor(TColor::GetColor("#D0743C"));
    ar_sigma->SetLineColor(TColor::GetColor("#D0743C"));
    ar_sigma->Draw();
    TLatex Tl_sigma;
    Tl_sigma.SetTextAlign(23);
    Tl_sigma.SetTextSize(0.05);
    Tl_sigma.SetTextColor(TColor::GetColor("#D0743C"));
    Tl_sigma.DrawLatex(mean_fitr, y0_sigma - 1.3, Form("#sigma = %.2f #pm %.3f", sigma_fitr, sigmaerr_fitr));

    c->SaveAs("./plots/RWR_Ncoll_merge.png");
    c->SaveAs("./plots/RWR_Ncoll_merge.pdf");
}