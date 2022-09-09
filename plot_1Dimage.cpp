using namespace std;
void format_canvas(TCanvas* c){
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.18);
		c->SetRightMargin(0.05);
		c->SetTopMargin(0.05);
		c->SetBottomMargin(0.15);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);

	}

void plot_1Dimage(){
	TString fileNames[]={"czt16iso_3mmAir.root","czt16iso_3mmAl.root"};
    TString legend[]={"Orignal","Al Box"};
	TString vars="energy_diff_total";
	TString xtitle="scale/mm";
	TString title="czt16_AlBox";
	TString treeName="detector";
	const int ndetector=16;
	const float bin_width=1.0;  // width of each detector
	const float scale_factor=0.5;
	float xlow=0;
	float xup=ndetector;
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	const int bins=xup;
	auto c1=new TCanvas("c1","",8, 30, 700, 700);//temporary canvas
	auto c2=new TCanvas("c2","",8, 30, 700, 700); //draw on this canvas
	format_canvas(c2);
	c2->cd();
	auto leg=new TLegend(.7,.75,.9,.9);	
	leg->SetFillColor(0);
	leg->SetLineColor(0);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.052);
	
	std::vector<TH1D*> hist; 	
	for(int k=0;k<2;k++){ //loop over files 
		TFile* file=TFile::Open(fileNames[k]);
		TH1D *h1=new TH1D(Form("h%d",k),Form("h%d",k),bins,xlow,xup);
		TTree* tree=(TTree*)file->Get(treeName);// draw from tree
		cout<<fileNames[k]<<" has "<<tree->GetEntries()<<" entries."<<endl;
		c1->cd();
		for(int ncut=0; ncut<ndetector;ncut++){
			int nphotons=tree->Draw(vars+"*1000.0",Form("detector_ID==%d",ncut));
			h1->SetBinContent(ncut+1,nphotons);
			h1->SetBinError(ncut+1,std::sqrt(nphotons));
			h1->GetXaxis()->SetBinLabel(ncut+1,Form("%d",ncut+1));
		}
		h1->SetLineColor(color[k]);
		h1->SetLineWidth(2);
		h1->SetStats(kFALSE);
		h1->GetXaxis()->SetTitle("detector ID");
		h1->GetYaxis()->SetTitle("number of photons");
		h1->SetTitle("");
		h1->GetXaxis()->CenterTitle();
		h1->GetYaxis()->CenterTitle();
		h1->GetXaxis()->SetTitleSize(0.06);
		h1->GetYaxis()->SetTitleSize(0.06);
		h1->GetXaxis()->SetTitleOffset(0.9);
		h1->GetYaxis()->SetTitleOffset(1.5);
		h1->GetXaxis()->SetLabelSize(0.05);
		h1->GetYaxis()->SetLabelSize(0.05);
		h1->GetYaxis()->SetRangeUser(0.,h1->GetMaximum()*1.2);
		h1->GetXaxis()->SetRangeUser(xlow,xup);
		//h1->GetXaxis()->SetNdivisions(5);
		c2->cd();
		if(k<1)
			{h1->Draw("hist");
			}
		else
			{
				h1->Draw("samehist");
		    }
		TLegendEntry* leg_entry0=leg->AddEntry(h1,legend[k]);
		leg_entry0->SetTextColor(color[k]);
		hist.push_back(h1);
	}
	c2->cd();
	leg->Draw("same");
    c2->Print(title+"_"+treeName+"_"+vars+".png");

    auto c3=new TCanvas("c3","",8, 30, 700, 700); //draw on this canvas
	format_canvas(c3);
	c3->cd();

	TH1D *horignal=(TH1D*) hist[0]->Clone("horignal");
	hist[0]->Add(hist[1],-1);
	hist[0]->Divide(horignal);
	TH1D* hratio=(TH1D*) hist[0]->Clone("hratio");
	hratio->SetLineColor(kBlue);
	hratio->SetLineWidth(2);
	hratio->SetStats(kFALSE);
	hratio->GetXaxis()->SetTitle("detector ID");
	hratio->GetYaxis()->SetTitle("Absorption Ratio");
	hratio->SetTitle("");
	hratio->GetXaxis()->CenterTitle();
	hratio->GetYaxis()->CenterTitle();
	hratio->GetXaxis()->SetTitleSize(0.06);
	hratio->GetYaxis()->SetTitleSize(0.06);
	hratio->GetXaxis()->SetTitleOffset(0.9);
	hratio->GetYaxis()->SetTitleOffset(1.5);
	hratio->GetXaxis()->SetLabelSize(0.05);
	hratio->GetYaxis()->SetLabelSize(0.05);
	hratio->GetYaxis()->SetRangeUser(0.,hratio->GetMaximum()*1.5);
	hratio->GetXaxis()->SetRangeUser(xlow,xup);
	hratio->Draw("hist");
	c3->Print(title+"_"+"ratio"+".png");

	auto c4=new TCanvas("c4","",8, 30, 700, 700); //draw on this canvas
	format_canvas(c4);
	c4->cd();
	for(int ncut=0; ncut<ndetector;ncut++){
			if(ncut %2 ==0)
				hratio->GetXaxis()->SetBinLabel(ncut+1,"");	
			else 	
				hratio->GetXaxis()->SetBinLabel(ncut+1,Form("%.1f",(ncut+1)*bin_width*scale_factor));
		}
	hratio->GetXaxis()->SetTitle(xtitle);
	hratio->GetXaxis()->LabelsOption("hd");
	hratio->Draw("hist");
	c4->Print(title+"_"+"1Dimage"+".png");
}
