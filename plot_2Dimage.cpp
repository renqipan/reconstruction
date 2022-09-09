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

void plot_2Dimage(){
	TString fileNames[]={"test16-4_3mmAir.root","test16-4_3mmAl.root"};
    TString legend[]={"Orignal","Al_Box"};
	TString vars="energy_diff_total";
	TString title="czt16-4";
	TString treeName="detector";
	const int nCol=16;
	const int nRow=4;
	const float detectorZ=1.0;  // width of each detector
	const float detectorX=4.0;
	const float scale_factor=0.5;
	float xlow=0;
	float xup=nCol;
	float ylow=0;
	float yup=nRow;
	int color[]={4,6,1,226,2,kOrange+2,3,kYellow,93};
	const int xbins=xup;
	const int ybins=yup;
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
	
	std::vector<TH2D*> hist; 	
	for(int k=0;k<2;k++){ //loop over files 
		TFile* file=TFile::Open(fileNames[k]);
		TH2D *h1=new TH2D(Form("h%d",k),Form("h%d",k),xbins,xlow,xup,ybins,ylow,yup);
		TTree* tree=(TTree*)file->Get(treeName);// draw from tree
		cout<<fileNames[k]<<" has "<<tree->GetEntries()<<" entries."<<endl;
		c1->cd();
		for (int j=0; j< nRow; j++){
			for(int i=0; i<nCol; i++){
			int nphotons=tree->Draw(vars+"*1000.0",Form("detector_ID==%d",j*nCol+i));
			h1->SetBinContent(i+1,j+1,nphotons);
			h1->SetBinError(i+1,j+1,std::sqrt(nphotons));
			h1->GetXaxis()->SetBinLabel(i+1,Form("%d",i+1));
			h1->GetYaxis()->SetBinLabel(j+1,Form("%d",j+1));
			}
		}
		
		h1->SetLineWidth(2);
		h1->SetStats(kFALSE);
		h1->GetXaxis()->SetTitle("ID in Y axis");
		h1->GetYaxis()->SetTitle("ID in X axis");
		h1->GetZaxis()->SetTitle("number of photons");
		h1->SetTitle("");
		h1->GetXaxis()->CenterTitle();
		h1->GetYaxis()->CenterTitle();
		h1->GetXaxis()->SetTitleSize(0.06);
		h1->GetYaxis()->SetTitleSize(0.06);
		h1->GetZaxis()->SetTitleSize(0.06);
		h1->GetXaxis()->SetTitleOffset(0.9);
		h1->GetYaxis()->SetTitleOffset(0.9);
		h1->GetZaxis()->SetTitleOffset(1.5);
		h1->GetXaxis()->SetLabelSize(0.05);
		h1->GetYaxis()->SetLabelSize(0.05);
		h1->GetZaxis()->SetRangeUser(0.,h1->GetMaximum()*1.5);
		h1->GetXaxis()->SetRangeUser(xlow,xup);
		h1->GetYaxis()->SetRangeUser(ylow,yup);
		c2->cd();
		h1->Draw("Surf2");
    	c2->Print(title+"_"+treeName+"_"+vars+legend[k]+"2D.png");
    	hist.push_back(h1);

	}
	

    auto c3=new TCanvas("c3","",8, 30, 700, 700); //draw on this canvas
	format_canvas(c3);
	c3->cd();

	TH2D *horignal=(TH2D*) hist[0]->Clone("horignal");
	hist[0]->Add(hist[1],-1);
	hist[0]->Divide(horignal);
	TH2D* hratio=(TH2D*) hist[0]->Clone("hratio");

	for(int i=0; i<nCol;i++){
			if(i %2 ==0)
				hratio->GetXaxis()->SetBinLabel(i+1,"");
			else 	
				hratio->GetXaxis()->SetBinLabel(i+1,Form("%.1f",(i+1)*detectorZ*scale_factor));
		}
	for (int j=0; j< nRow; j++){
			if(j %2 ==0)
				hratio->GetYaxis()->SetBinLabel(j+1,"");
			else
				hratio->GetYaxis()->SetBinLabel(j+1,Form("%.1f",(j+1)*detectorX*scale_factor));
	}
	hratio->SetLineWidth(2);
	hratio->SetStats(kFALSE);
	hratio->GetXaxis()->SetTitle("Xscale/mm");
	hratio->GetYaxis()->SetTitle("Yscale/mm");
	hratio->GetZaxis()->SetTitle("Absorption Ratio");
	hratio->SetTitle("");
	hratio->GetXaxis()->CenterTitle();
	hratio->GetYaxis()->CenterTitle();
	hratio->GetXaxis()->SetTitleSize(0.06);
	hratio->GetYaxis()->SetTitleSize(0.06);
	hratio->GetZaxis()->SetTitleSize(0.06);

	hratio->GetXaxis()->SetTitleOffset(0.9);
	hratio->GetYaxis()->SetTitleOffset(0.9);
	hratio->GetZaxis()->SetTitleOffset(1.2);
	hratio->GetXaxis()->SetLabelSize(0.05);
	hratio->GetYaxis()->SetLabelSize(0.05);
	hratio->GetZaxis()->SetLabelSize(0.05);
	hratio->GetZaxis()->SetRangeUser(0.,hratio->GetMaximum()*1.5);
	hratio->Draw("Surf2");
	c3->Print(title+"_"+"image"+"2D.png");


}
