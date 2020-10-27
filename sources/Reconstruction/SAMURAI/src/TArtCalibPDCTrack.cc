#include "TArtCalibDCTrack.hh" 
#include "TArtCalibPDCTrack.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibPDCTrack::TArtCalibPDCTrack(const TString indata,
				     const TString outdata) 
  : TArtReconstruction(outdata)
{
  TArtCore::Info(__FILE__,"Creating the PDC detector track objects...");
  // initialize here all necessary variable (wire distances for example)

  indataname = indata ;
  outdataname = outdata ;

  sman = TArtStoreManager::Instance();

  // create the array of tracks that will be rebuild (assume 2 in average) 
  fDCTrackArray = new TClonesArray("TArtDCTrack",2);
  fDCTrackArray->SetName(outdataname.Data());
  fDCTrackArray->SetOwner();
  // add it to the "database"

  sman->AddDataContainer(fDCTrackArray);

  // PDC1
  layerdirs.push_back(u);
  layerdirs.push_back(x);
  layerdirs.push_back(v);

  // PDC2
  layerdirs.push_back(u);
  layerdirs.push_back(x);
  layerdirs.push_back(v);


  // init
  nlayer_u = 0 ;
  nlayer_x = 0 ;
  nlayer_v = 0 ;
  nlayer_y = 0 ; // for compatibility and flexibility

  // map : can be better writen (from layer_dir for example !)  
  
  layer_map.push_back(nlayer_u++); 
  layer_map.push_back(nlayer_x++); 
  layer_map.push_back(nlayer_v++); 
  layer_map.push_back(nlayer_u++); 
  layer_map.push_back(nlayer_x++); 
  layer_map.push_back(nlayer_v++); 
  //

  nlayer = nlayer_u + nlayer_x + nlayer_v ;

  hits_buffer   = new std::vector <TArtDCHit *> [nlayer];
  hits_buffer_x = new std::vector <TArtDCHit *> [nlayer_x];
  hits_buffer_u = new std::vector <TArtDCHit *> [nlayer_u];
  hits_buffer_v = new std::vector <TArtDCHit *> [nlayer_v];
  hits_buffer_y = new std::vector <TArtDCHit *> [nlayer_y]; // for compatibility and flexibility

}

//__________________________________________________________
TArtCalibPDCTrack::~TArtCalibPDCTrack()  {
  // delete, clear all array, varaibles, etc,... (free memory)
  ClearData();
  delete [] hits_buffer_x;
  delete [] hits_buffer_u;
  delete [] hits_buffer_v;
  delete fDCTrackArray;
}

//__________________________________________________________
void TArtCalibPDCTrack::ClearData()   {

  for(Int_t i=0;i<nlayer_x;i++) hits_buffer_x[i].clear();
  for(Int_t i=0;i<nlayer_u;i++) hits_buffer_u[i].clear();
  for(Int_t i=0;i<nlayer_v;i++) hits_buffer_v[i].clear();

  fDCTrackArray->Clear("C");
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibPDCTrack::ReconstructData() { // call after the raw data are loaded
  TArtCore::Debug(__FILE__,"ReconstructData");
  // allocate each hit in hit_buffer
  SetHitsBuffer();

  // here comes the reconstruction code

  // --------------------------------------------


  Int_t ntr=0 ;
  Double_t X =0, Y=0, Z=0, U=0, V=0;
  Int_t nhit = 0, nhitx=0, nhitu=0, nhitv=0;
  Double_t weightx =0, weightv =0, weightu =0 ;

  // main algorithm that found track(s)
  // -----------
  for(int l=0;l<nlayer_x;l++){
    for (std::vector<TArtDCHit *>::iterator it = hits_buffer_x[l].begin() ; it != hits_buffer_x[l].end(); ++it){
      TArtDCHit * hit = *it;
      //      std::cout << hit->GetTDC()  << " " << hit->GetTrailTDC()
      //		<< " " << hit-> GetWirePosition()
      //		<< std::endl;
      if(hit->GetTDC()>0 && hit->GetTrailTDC()>0){
	weightx += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC());
	X += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC())*hit-> GetWirePosition() ;
	nhit++;
	nhitx++;
      }
    }
  }
  for(int l=0;l<nlayer_u;l++){
    for (std::vector<TArtDCHit *>::iterator it = hits_buffer_u[l].begin() ; it != hits_buffer_u[l].end(); ++it){
      TArtDCHit * hit = *it;
      //      std::cout << hit->GetTDC()  << " " << hit->GetTrailTDC()
      //		<< " " << hit-> GetWirePosition()
      //		<< std::endl;
      if(hit->GetTDC()>0 && hit->GetTrailTDC()>0){
	weightu += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC());
	U += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC())*hit-> GetWirePosition()/TMath::Sqrt(2) ;
	// X += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC())*hit-> GetWirePosition()/TMath::Sqrt(2) ; // could be added
	nhit++;
	nhitu++;	
	// nhitx++;
      }
    }
  }
  
  for(int l=0;l<nlayer_v;l++){
    for (std::vector<TArtDCHit *>::iterator it = hits_buffer_v[l].begin() ; it != hits_buffer_v[l].end(); ++it){
      TArtDCHit * hit = *it;
      //      std::cout << hit->GetTDC()  << " " << hit->GetTrailTDC()
      //		<< " " << hit-> GetWirePosition()
      //		<< std::endl;
      if(hit->GetTDC()>0 && hit->GetTrailTDC()>0){
	weightv += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC());
	V += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC())*hit-> GetWirePosition()/TMath::Sqrt(2) ;
	// X += TMath::Abs(hit->GetTDC() - hit->GetTrailTDC())*hit-> GetWirePosition()/TMath::Sqrt(2) ; // could be added   
	nhit++;
	nhitv++;
	// nhitx++;
      }
    }
  }

  if(nhitx>0 && weightx>0) {
    X /= weightx ;
  } else {
    X = NAN ;
  }

  if(nhitu>0 && weightu>0) {
    U /= weightu ;
  } else {
    U = NAN ;
  }

  if(nhitv>0 && weightv>0) {
    V /= weightv ;
  } else {
    V = NAN ;
  }

  Y = (U-V)*TMath::Sqrt(2)/2.;
  //  if(!TMath::IsNaN(X)) std::cout << X <<  " " << U << " " << V << " " << Y << std::endl;


  // TODO : 
  // * separate de PDC to obtain angles (<-easy) 

  //  std::cout << X << std::endl;
  // 
  // loop over hits_buffer_x, hits_buffer_u, hits_buffer_v
  // calculate barycentre and project on y
  //
  // --------------
  // then when the track characteristics are found, create a track
  
  // and put it in the main table and increment
  // this part is general : simple algorithm will only found one track
  // more complex ones can be developed for multitrack 

  new ((*fDCTrackArray)[ntr]) TArtDCTrack();


  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetChi2(1); // value =1 ?
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetNDF(1); // 
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetNumHitLayer(nhit);// value between 1 and 6 !
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetPosition(X,0); // X
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetPosition(Y,1); // Y
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetAngle(3.14159,0); // X
  ((TArtDCTrack*)fDCTrackArray->At(ntr))->SetAngle(3.14159,1); // Y
  
  
  ntr++ ;  
 
  // note instead of barycentre, one can try 
  // Lorentzian or squared secant hyperbolics (Sechs)
  // K. Lau, J. Pyrlik, Nucl. Instr. and Meth. A 366 (1995) 298.

  fReconstructed = true;
  return;
}

//__________________________________________________________
void TArtCalibPDCTrack::SetHitsBuffer() {

  // load the hits for PDC, from the "database"
  TClonesArray * hit_array = (TClonesArray *)sman->FindDataContainer(indataname.Data());
  Int_t num_hit = hit_array->GetEntries();

  // allocate each hit in hit_buffer
  for(Int_t i=0;i<num_hit;i++){
    TArtDCHit * hit = (TArtDCHit *)hit_array->At(i);
    Int_t layer = hit->GetLayer();

    hits_buffer[layer].push_back(hit);
    if(layerdirs[layer] == x){
      if(layer_map[layer]>=nlayer_x)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-x %d",layer_map[layer],nlayer_x);
      hits_buffer_x[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == y){
      if(layer_map[layer]>=nlayer_y)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-y %d",layer_map[layer],nlayer_y);
      hits_buffer_y[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == u){
      if(layer_map[layer]>=nlayer_u)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-u %d",layer_map[layer],nlayer_u);
      hits_buffer_u[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == v){
      if(layer_map[layer]>=nlayer_v)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-v %d",layer_map[layer],nlayer_v);
      hits_buffer_v[layer_map[layer]].push_back(hit);
    }
    else{TArtCore::Info(__FILE__,"Can not find corresponding wire-direction");
    }
  } 

  return;

}

