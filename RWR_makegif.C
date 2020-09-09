#include "tdrstyle.C"

Float_t AxisTitleSize = 0.06;
Float_t AxisLabelSize = 0.05;
Float_t LeftMargin = 0.13;
Float_t RightMargin = 0.05;
Float_t TopMargin = 0.08;
Float_t BottomMargin = 0.1;
Float_t TopTextOffset = 0.2;
Float_t TopTextSize = 0.8;

void RWR_makegif(const char *infile = "./data/RWR_data_merge.root", const Int_t kUPDATE = 1)
{
    system("mkdir -p ./plots");

    setTDRStyle();
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    gBenchmark->Start("RWR_animation");

    // Load 1dayHEPhysicist logo
    TImage *img = TImage::Open("./OnedayHEPhysicist-logo.pdf");
    img->SetConstRatio(kTRUE);
    if (!img)
    {
        printf("Could not create an image... exit\n");
        return;
    }

    TH1F *hM_Ncoll = new TH1F("hM_Ncoll", "hM_Ncoll", 11, -0.5, 10.5);
    TH1F *hM_Ncoll_err = new TH1F("hM_Ncoll_err", "hM_Ncoll_err", 11, -0.5, 10.5);
    hM_Ncoll->Sumw2();
    hM_Ncoll->SetLineColor(TColor::GetColor("#006E8A"));
    hM_Ncoll->SetFillColor(TColor::GetColor("#006E8A"));
    hM_Ncoll_err->Sumw2();
    hM_Ncoll_err->SetLineColor(TColor::GetColor("#00142E"));
    hM_Ncoll_err->SetLineWidth(3);
    hM_Ncoll_err->SetMarkerStyle(20);
    hM_Ncoll_err->SetMarkerSize(0.1);

    // Open file
    TFile *f = new TFile(infile, "READ");
    TTree *tree = (TTree *)f->Get("outTree");
    Long64_t Run_Date;
    Int_t Ncoll;
    tree->SetBranchAddress("Run_Date", &Run_Date);
    tree->SetBranchAddress("Ncoll", &Ncoll);

    gSystem->Unlink("./plots/RWR_animation.gif"); // delete old file

    TCanvas *c = new TCanvas("c", "", 700, 500);
    gPad->SetRightMargin(RightMargin);
    gPad->SetTopMargin(TopMargin);
    gPad->SetLeftMargin(LeftMargin);
    c->cd();

    TLatex Tl_Run;

    for (Long64_t ev = 0; ev < tree->GetEntriesFast(); ev++)
    {
        tree->GetEntry(ev);

        hM_Ncoll->Fill(Ncoll);
        hM_Ncoll_err->Fill(Ncoll);

        // make animation of filling the histogram
        if ((ev % kUPDATE) == 0)
        {
            auto frame = c->DrawFrame(-0.5, 0, 10.5, (hM_Ncoll->GetMaximum() + hM_Ncoll->GetBinError(hM_Ncoll->GetMaximumBin())) * 1.25);
            frame->GetXaxis()->SetTitle("Number of hits per 10 rolls");
            frame->GetYaxis()->SetTitle("Frequency");
            frame->GetXaxis()->SetTitleOffset(1.05);
            frame->GetYaxis()->SetTitleOffset(1.05);
            frame->GetXaxis()->SetNdivisions(11, 2, 0, kTRUE);
            frame->GetXaxis()->SetTitleSize(AxisTitleSize);
            frame->GetXaxis()->SetLabelSize(AxisLabelSize);
            frame->GetYaxis()->SetTitleSize(AxisTitleSize);
            frame->GetYaxis()->SetLabelSize(AxisLabelSize);
            Tl_Run.SetTextAlign(31);
            Tl_Run.SetTextSize(TopTextSize * TopMargin);
            Tl_Run.SetTextColor(1);
            char _year[10], _month[10], _day[10];
            std::string str_Run = Form("%lld", Run_Date);
            std::size_t len_year = str_Run.copy(_year, 4, 0);
            _year[len_year] = '\0';
            std::size_t len_month = str_Run.copy(_month, 2, 4);
            _month[len_month] = '\0';
            std::size_t len_day = str_Run.copy(_day, 2, 6);
            _day[len_day] = '\0';
            Tl_Run.DrawLatexNDC(1 - gPad->GetRightMargin(), 1 - gPad->GetTopMargin() + TopTextOffset * gPad->GetTopMargin(), Form("Date: %s.%s.%s", _year, _month, _day));
            hM_Ncoll->Draw("HISTSAME");
            hM_Ncoll_err->Draw("PE1X0SAME");

            c->RedrawAxis();
            c->Update();
        }
        c->Modified();
        c->Update();
        if (gROOT->IsBatch())
        {
            c->Print("./plots/RWR_animation.gif+");
            printf("ev = %lld \n", ev);
        }
        else
        {
            if (gSystem->ProcessEvents())
                break;
        }
    }

    hM_Ncoll->Draw("sameaxis"); // to redraw axis hidden by the fill area
    hM_Ncoll_err->Draw("sameaxis");

    c->Modified();

    // Fit a Gaussian function to the histogram (do not draw now)
    TF1 *f1 = new TF1("f1", "gaus", -0.5, 10.5);
    f1->SetParNames("N", "Mean", "Sigma");
    hM_Ncoll->Fit("f1", "REM0");
    f1->SetLineWidth(5);
    f1->SetLineColor(TColor::GetColor("#FEA319"));
    // Extract fit parameters, to be used later
    Float_t Constant_fitr = f1->GetParameter("N");
    Float_t mean_fitr = f1->GetParameter("Mean");
    Float_t meanerr_fitr = f1->GetParError(1);
    Float_t sigma_fitr = f1->GetParameter("Sigma");
    Float_t sigmaerr_fitr = f1->GetParError(2);

    // make animation of fit
    const Float_t kUpdate_func = 0.1;
    for (int i = 0; i < (Int_t)(10.5 - (-0.5) / kUpdate_func); i++)
    {
        f1->SetRange(-0.5, -0.5 + i);
        f1->Draw("same");
        c->Update();
        c->Modified();
        c->Update();
        c->Print("./plots/RWR_animation.gif+");
    }

    // Make decoration: make arrows to mark mean and sigma values from the fit
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
    // Make delay
    for (int i = 0; i < 50; i++)
    {
        if (i >= 10)
        {
            Float_t d = 0.25;
            TPad *l = new TPad("l", "l", 1.03 * LeftMargin, 0.95 * (1 - TopMargin) - d * (img->GetHeight() / img->GetWidth()), (1.03 * LeftMargin) + d * (img->GetWidth() / img->GetHeight()), 0.95 * (1 - TopMargin));
            l->SetFillColor(0);
            l->SetFillStyle(0);
            l->SetFrameFillColor(0);
            l->SetFrameFillStyle(0);
            l->SetFrameLineWidth(0);
            l->Draw();
            if (i == 10)
                l->cd();
            img->Draw();
        }

        c->Update();
        c->Modified();
        c->Update();
        c->Print("./plots/RWR_animation.gif+");
    }

    if (gROOT->IsBatch())
        c->Print("./plots/RWR_animation.gif++");

    f->Close();

    gBenchmark->Show("RWR_animation");
}