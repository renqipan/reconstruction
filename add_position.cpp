//reconstruct the focus of x-ray after it transverse
// a Al filter.
void add_position(){
	TString fileName="mysca2n_2mmAl05Fe_posdirec.root";
	TFile *file=new TFile(fileName,"update");
	TTree *mytree=(TTree*) file->Get("Photons_AlPost");
	Double_t posX,posY,posZ,px,py,pz,energy;
	mytree->SetBranchAddress("post_positionX",&posX);
	mytree->SetBranchAddress("post_positionY",&posY);
	mytree->SetBranchAddress("post_positionZ",&posZ);
	mytree->SetBranchAddress("post_momX",&px);
	mytree->SetBranchAddress("post_momY",&py);
	mytree->SetBranchAddress("post_momZ",&pz);
	mytree->SetBranchAddress("energy_post_Al",&energy);

	Double_t imageX,imageY,imageZ;
	TBranch *branchImageX, *branchImageZ;
	branchImageX=mytree->Branch("imageX",&imageX,"imageX/D");
	branchImageZ=mytree->Branch("imageZ",&imageZ,"imageZ/D");
	Int_t entries= mytree->GetEntries();
	cout<<"total number of events: "<<entries<<endl;
	imageY=0.0;  //define focus plane
	for(Int_t i=0;i<entries;i++){
	    mytree->GetEntry(i);
	    imageX=(imageY - posY)/py*px+posX;
	    imageZ=(imageY - posY)/py*pz+posZ;
    	if(i<20){
	    	std::cout<<"posX: "<<posX<<" posY: "<<posY<<" posZ: "<<posZ<<std::endl;
	    	cout<<"px: "<<px<<" py: "<<py<<" pz: "<<pz<<endl;
	    	cout<<"imageX: "<<imageX<<" imageZ: "<<imageZ<<endl;
    	} 
    	 branchImageX->Fill();
    	 branchImageZ->Fill();
	    
	   

	}
	mytree->Write("",TObject::kOverwrite);
	std::cout<<"the tree "<<mytree->GetName()<<" has "<<mytree->GetEntries()<<" entries"<<std::endl;
    std::cout<<fileName<<" has been updated"<<std::endl;        
    delete mytree;
    file->Close();
}