#include <fstream>
#include <iostream>

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

void read_hist(){
	TString fileNames[]={"myout_2mmAl05Fe_0829.root"};
    TString legend[]={"Tungsten,100keV"};
	TString vars[]={"energy_pre_Al*1000.0"};
	TString xtitle[]={"photon energy (keV)"};
	TString title[]={"entries"};
	float xlow[]={0.0};
	float xup[]={100.0};
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	 const int bins=xup[0]*5;
	for(int i=0;i<1;i++){ //loop over variables
		auto c1=new TCanvas("c1","",8, 30, 700, 700);//temporary canvas
		auto c2=new TCanvas("c2","",8, 30, 700, 700); //draw on this canvas
		format_canvas(c2);
		c2->cd();
		auto leg=new TLegend(.5,.75,.7,.95);	
		leg->SetFillColor(0);
		leg->SetLineColor(0);
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);
		leg->SetTextFont(42);
		leg->SetTextSize(0.052);
			
		for(int k=0;k<1;k++){ //loop over files 
			TFile* file=TFile::Open(fileNames[k]);
			TH1F *h1=new TH1F("h1","",bins,xlow[i],xup[i]);
			TTree* tree=(TTree*)file->Get("Photons_AlPre");// draw from tree
			c1->cd();
			tree->Draw(vars[i]+">>h1");
			
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
			double integral=h1->Integral();
			h1->Scale(1.0/integral);
			c2->cd();
			if(k<1)
				{h1->Draw("hist");
				}
			else
				{
					h1->Draw("samehist");
			    }
			ofstream write_csv,write_txt;
			TString file_csv="energy_hist.dat";
			TString file_txt="energy_command.mac";
			write_csv.open(file_csv);
			write_txt.open(file_txt);

			TLegendEntry* leg_entry0=leg->AddEntry(h1,legend[k]);
			leg_entry0->SetTextColor(color[k]);
			int nbins=h1->GetNbinsX();
			double x[nbins],y[nbins];
			for(int ii=0;ii<nbins;ii++){
				x[ii]=h1->GetXaxis()->GetBinUpEdge(ii+1);
				x[ii]=x[ii]/1000.0; //store data in MeV unit
				y[ii]=h1->GetBinContent(ii+1);
				//cout<<x[ii]<<" "<<y[ii]<<endl;
				write_csv<<x[ii]<<" "<<y[ii]<<endl;
				write_txt<<"/gps/hist/point"<<" "<<x[ii]<<" "<<y[ii]<<endl;
			}

			write_csv.close();
			cout<<"energy(MeV) spectrum in histogram has been written into: "<<file_csv<<endl;
			write_txt.close();
			cout<<"energy(MeV) spectrum in UI command has been written into: "<<file_txt<<endl;
		}
		c2->cd();
		leg->Draw("same");
	    c2->Print(title[i]+"100_2mmAl.png");
	   //c2->Print(title[i]+".pdf");

	}
	
        
}
