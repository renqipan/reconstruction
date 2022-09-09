void format_canvas(TCanvas* c){
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.13);
		c->SetRightMargin(0.05);
		c->SetTopMargin(0.05);
		c->SetBottomMargin(0.15);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);

	}
void Floor(TH2D* histo){
	for (int i=0;i<histo->GetNbinsX();i++){
		for (int j=0;j<histo->GetNbinsY();j++){
			if(!(histo->GetBinContent(i+1,j+1)>0.001)){
				histo->SetBinContent(i+1,j+1,0.0);
				float xx=histo->GetXaxis()->GetBinCenter(i+1);
				float yy=histo->GetYaxis()->GetBinCenter(j+1);
				//cout<<"warning!!!!! No events in x: "<<xx<<" y: "<<yy<<endl;
			}
		}
	}
}

Double_t Find_max10(TH2D* hist){
	double x[100],y[100];
	double max=hist->GetMaximum();
	int counter=0;
	for (int i=16;i<19;i++){
		double binContent=hist->GetBinContent(i,11);
	//	if(binContent>max/20. && binContent< max){
			y[counter]=binContent;
			x[counter]=hist->GetXaxis()->GetBinCenter(i);
			cout<<"x: "<<x[counter]<<" y: "<<y[counter]<<endl;
			counter++;
		//}
	}
	int nbins = sizeof(x)/sizeof(x[0]);;
	TGraph* graph=new TGraph(nbins,x,y);
	TF1 myFunc("myFunc","[0]+[1]*x+[2]*x**2",6,9);
	//TF1 myFunc("gaussian","[0]*TMath::Gaus(x,[1],[2])",6,9);
	myFunc.SetParameters(1,7.0,-10);
	graph->Fit(&myFunc);
	cout<<"parameter[0]: "<<myFunc.GetParameter(0)<<endl;
	cout<<"parameter[1]: "<<myFunc.GetParameter(1)<<endl;
	cout<<"parameter[2]: "<<myFunc.GetParameter(2)<<endl;
	double xlocation=myFunc.GetX(max/10.0,6,9);
	cout<<"x location: "<<xlocation<<endl;
	cout<<"focus size: "<<xlocation*2.0<<endl;
	return xlocation*2.0;
}

void plot_2Dfocus(){
	TString fileNames="myscan_2mmAl05Fe_posdirec*.root";
    TString legend[]={"Tungsten,100keV"};
	TString treeName="Photons_AlPost";
	double focus_frame=10;
	double xlow=-focus_frame;
	double xup=focus_frame;
	double ylow=-focus_frame;
	double yup=focus_frame;
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	TString cuts[]={"energy_post_Al>0.0","energy_post_Al>0.0&&energy_post_Al<0.02","energy_post_Al>0.02&&energy_post_Al<0.03",
	"energy_post_Al>0.03&& energy_post_Al<0.04","energy_post_Al>0.04&&energy_post_Al<0.05",
	"energy_post_Al>0.05&&energy_post_Al<0.06","energy_post_Al>0.06&&energy_post_Al<0.07","energy_post_Al>0.07"};
	TString addText[]={"0<k<100keV","0<k<20keV","20keV<k<30keV","30keV<k<40keV","40keV<k<50keV",
					"50keV<k<60keV","60keV<k<70keV","70keV<k<100keV"};
	TString cutsName[]={"k0-100","k0-20","k20-30","k30-40","k40-50","k50-60","k60-70","k70-100"};
	TChain* tree=new TChain(treeName);
	tree->Add(fileNames);
	Int_t entries=tree->GetEntries();
	cout<<"this tree has "<<entries<<" entries"<<endl;
	for(int i=0;i<8;i++){ //loop over cuts
		auto c1=new TCanvas("c1","",8, 30, 700, 700);//temporary canvas
		auto c2=new TCanvas("c2","",8, 30, 700, 700); //draw on this canvas
		format_canvas(c2);
		c2->cd();
		auto leg=new TLegend(.47,.82,.73,.97);	
		leg->SetFillColor(0);
		leg->SetLineColor(0);
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);
		leg->SetTextFont(42);
		leg->SetTextSize(0.052);
		TH2D *h1=new TH2D("h2D","",20,xlow,xup,20,ylow,yup);
		c1->cd();
		tree->Draw("(imageZ-3)*1000.0:imageX*1000.0>>h2D",cuts[i]);
		//TH2D *h1=(TH2D*)gROOT->FindObject("h2D")->Clone();
		double integral=h1->Integral();
		h1->Scale(1.0/integral);
		//Floor(h1);
		//double focus_size=Find_max10(h1);
		h1->SetStats(kFALSE);
		h1->GetXaxis()->SetTitle("X axis / um");
		h1->GetYaxis()->SetTitle("Z axis / um");
		h1->GetZaxis()->SetTitle("density");
		h1->SetTitle("");
		h1->GetXaxis()->CenterTitle();
		h1->GetYaxis()->CenterTitle();
		h1->GetZaxis()->CenterTitle();
		h1->GetXaxis()->SetTitleSize(0.06);
		h1->GetYaxis()->SetTitleSize(0.06);
		h1->GetZaxis()->SetTitleSize(0.06);
		h1->GetXaxis()->SetTitleOffset(0.9);
		h1->GetYaxis()->SetTitleOffset(1.5);
		h1->GetXaxis()->SetLabelSize(0.05);
		h1->GetYaxis()->SetLabelSize(0.05);
		h1->GetXaxis()->SetNdivisions(6);
		gPad->SetLogz();
		c2->cd();
		h1->DrawNormalized("surf2");
		TPaveText *pt = new TPaveText(0.3,0.83,0.5,0.93,"NDC");
		  pt->SetFillColor(0);
		  pt->SetTextFont(42);
		  pt->SetTextSize(0.05);
		  pt->SetTextColor(kBlack);
		  pt->SetBorderSize(0);
		  pt->AddText(addText[i]); 
		  pt->Draw();
		c2->Print(treeName+"_"+"focusXZ"+"_"+cutsName[i]+".png");

		delete h1;
		delete c1;
		delete c2;

	}
	
        
}
