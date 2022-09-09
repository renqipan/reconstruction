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

void compare_cos(){
	TString fileNames[]={"myout_2mmAir05Fe.root","myout_2mmAl05Fe.root"};
    TString legend[]={"2mm Air","2mm Al"};
	TString vars[]={"cos_theta_post","cos_phi_post"};
	TString xtitle[]={"cos#theta","cos#phi"};
	TString title="compare_AirAlRange";
	TString treeName="Photons_AlPost";
	TString cuts[]={"energy_post_Al<0.02","energy_post_Al>0.02&&energy_post_Al<0.03",
	"energy_post_Al>0.03&& energy_post_Al<0.04","energy_post_Al>0.04&&energy_post_Al<0.05",
	"energy_post_Al>0.05&&energy_post_Al<0.06","energy_post_Al>0.07"};
	TString addText[]={"k<20 keV","20keV<k<30keV","30keV<k<40keV","40keV<k<50keV",
					"50keV<k<60keV","70keV<k<100keV"};
	TString cutsName[]={"k0-20","k20-30","k30-40","k40-50","k50-60","k70-100"};
	float xlow[]={-1.0,-1.0};
	float xup[]={1.0,1.0};
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	 const int bins=100;
	for(int i=0;i<2;i++){ //loop over variables

		for(int n=0;n<6;n++)//loop over cuts
		{
			auto c1=new TCanvas("c1","",8, 30, 700, 700);//temporary canvas
			auto c2=new TCanvas("c2","",8, 30, 700, 700); //draw on this canvas
			format_canvas(c2);
			c2->cd();
			auto leg=new TLegend(.6,.78,.8,.95);	
			leg->SetFillColor(0);
			leg->SetLineColor(0);
			leg->SetBorderSize(0);
			leg->SetFillStyle(0);
			leg->SetTextFont(42);
			leg->SetTextSize(0.052);
			for(int k=0;k<2;k++){ //loop over files 
			TFile* file=TFile::Open(fileNames[k]);
				TH1F *h1=new TH1F("h1","",bins,xlow[i],xup[i]);
				TTree* tree=(TTree*)file->Get(treeName);// draw from tree
				c1->cd();
				tree->Draw(vars[i]+">>h1",cuts[n]);
				
				h1->SetLineColor(color[k]);
				h1->SetLineWidth(2);
				h1->SetStats(kFALSE);
				h1->GetXaxis()->SetTitle(xtitle[i]);
				h1->GetYaxis()->SetTitle("probability density");
				h1->SetTitle("");
				h1->GetXaxis()->CenterTitle();
				h1->GetYaxis()->CenterTitle();
				h1->GetXaxis()->SetTitleSize(0.06);
				h1->GetYaxis()->SetTitleSize(0.06);
				h1->GetXaxis()->SetTitleOffset(0.9);
				h1->GetYaxis()->SetTitleOffset(1.5);
				h1->GetXaxis()->SetLabelSize(0.05);
				h1->GetYaxis()->SetLabelSize(0.05);
				h1->GetYaxis()->SetRangeUser(0.,h1->GetMaximum()*1.3);
				h1->GetXaxis()->SetRangeUser(xlow[i],xup[i]);
				h1->GetXaxis()->SetNdivisions(5);
				c2->cd();
				if(k<1)
					{h1->DrawNormalized("hist");
					}
				else
					{
						h1->DrawNormalized("samehist");
				    }
				TLegendEntry* leg_entry0=leg->AddEntry(h1,legend[k]);
				leg_entry0->SetTextColor(color[k]);

			}
			c2->cd();
			leg->Draw("same");
			 TPaveText *pt = new TPaveText(0.3,0.8,0.5,0.90,"NDC");
			  pt->SetFillColor(0);
			  pt->SetTextFont(42);
			  pt->SetTextSize(0.05);
			  pt->SetTextColor(kBlack);
			  pt->SetBorderSize(0);
			  pt->AddText(addText[n]); 
			  pt->Draw();
		    c2->Print(title+"_"+treeName+"_"+vars[i]+cutsName[n]+".png");
		    delete c1;
		    delete c2;
		}
	
		

	}
	
        
}
