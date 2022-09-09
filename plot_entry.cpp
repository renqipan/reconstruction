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

void plot_entry(){
	TString fileNames[]={"czt_2mmAl01Te.root","czt_2mmAl02Te.root","czt_2mmAl03Te.root",
						"czt_2mmAl04Te.root","czt_2mmAl05Te.root"};
    TString legend[]={"0.1mm Te","0.2mm Te","0.3mm Te","0.4mm Te","0.5mm Te",};
    TString legned_cut="8-20 keV";
	TString vars[]={"energy_diff_total","energy_secondary_total"};
	TString xtitle[]={"photon energy (keV)","photon energy (keV)"};
	TString title="czt_loopTethinckness";
	TString treeName="detector";
	float xlow[]={0.,0.0};
	float xup[]={100.,100.0};
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	 const int bins=xup[0]*5;
	for(int i=0;i<2;i++){ //loop over variables
		auto c1=new TCanvas("c1","",8, 30, 700, 700);//temporary canvas
		auto c2=new TCanvas("c2","",8, 30, 700, 700); //draw on this canvas
		format_canvas(c2);
		c2->cd();
		auto leg=new TLegend(.5,.65,.7,.9);	
		leg->SetFillColor(0);
		leg->SetLineColor(0);
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);
		leg->SetTextFont(42);
		leg->SetTextSize(0.052);
			
		for(int k=0;k<5;k++){ //loop over files 
			TFile* file=TFile::Open(fileNames[k]);
			TH1D *h1=new TH1D("h1","",bins,xlow[i],xup[i]);
			TTree* tree=(TTree*)file->Get(treeName);// draw from tree
			cout<<"entries: "<<tree->GetEntries()<<endl;
			c1->cd();
			tree->Draw(vars[i]+"*1000.0"+">>h1");
			
			h1->SetLineColor(color[k]);
			h1->SetLineWidth(2);
			h1->SetStats(kFALSE);
			h1->GetXaxis()->SetTitle(xtitle[i]);
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
			h1->GetXaxis()->SetRangeUser(xlow[i],xup[i]);
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
		}
		c2->cd();
		leg->Draw("same");
	    c2->Print(title+"_"+treeName+"_"+vars[i]+".png");

	}
	
        
}
