#include "TArtCalibBDC.hh" 
#include "TArtBDC.hh"
#include "TArtESPRIParameters.hh"
#include "TArtUserParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "TArtTDCHit.hh"

#include <TMatrixD.h>
#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include "Math/Functor.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"

//__________________________________________________________
TArtCalibBDC::TArtCalibBDC()
  : TArtReconstruction("ESPRIBDC")
{
  TArtCore::Info(__FILE__,"Creating the BDC calibration objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }
  setup2 = (TArtUserParameters*)sman->FindParameters("UserParameters");
  if(!setup2){
    TArtCore::Info(__FILE__,"Can not find parameter handler: UserParameters");
    throw;
  }

  fBDCArray = new TClonesArray("TArtBDC",10);
  fBDCArray->SetName("ESPRIBDC");
  fBDCArray->SetOwner();
  sman->AddDataContainer(fBDCArray);

}

//__________________________________________________________
TArtCalibBDC::~TArtCalibBDC()  {
  ClearData();

  delete fBDCArray;
}

//__________________________________________________________
void TArtCalibBDC::ClearData()   {
  fBDCArray->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibBDC::ReconstructData()  {

  Int_t nw[16];//multiplicity
  Int_t iw[20][16];//channel ID at each
  Int_t tw[20][16];// TDC, iplane,ch,num up to 20 hit/plane
  Int_t tww[20][16];//only X1
  Double_t zz[8];//

  Int_t tdc[4];
  Double_t drft1,drft2,drft3,drft4,drft[4];
  Double_t xx[4],dxx[4];
  Double_t ang,init_x,xtrue,atrue,xerr,aerr;

  TArtCore::Debug(__FILE__,"ReconstructData");

  TClonesArray *tdctrk =(TClonesArray *)sman->FindDataContainer("ESPRITDC");
  if(!tdctrk) return;

  //const Double_t test = setup2->FindDoublePara("dprm0");
  const Double_t bstc1 = setup2->FindDoublePara("bstc1")*0.25;
  const Double_t bstc2 = setup2->FindDoublePara("bstc2")*0.25;
  const Double_t bstc3 = setup2->FindDoublePara("bstc3")*0.25;
  const Double_t bstc4 = setup2->FindDoublePara("bstc4")*0.25;
  const Double_t bstc5 = setup2->FindDoublePara("bstc5")*0.25;

  Double_t tch;
  Int_t nrt_tdc = tdctrk->GetEntriesFast();

  for(Int_t i=0;i<16;i++){
    nw[i]=0;
    for(Int_t j=0;j<20;j++){
      iw[j][i]=-1; tw[j][i]=-9999;
      if(i<2) tww[j][i]=-9999;
    }
  }

  Int_t nentry=0;
  for(Int_t i=0;i<nrt_tdc;i++){
    TArtTDCHit *hit = (TArtTDCHit*)tdctrk->At(i);
    Int_t id_plane = (Double_t)hit->GetPlaneID();
    
    if(id_plane>0&&id_plane<=16){//BDC
      Int_t layer = (Double_t)hit->GetLayer();
      Int_t ch  = (Double_t)hit->GetWireID();
      Int_t tdc1 = (Double_t)hit->GetTDC();
      Int_t tdc2 = (Double_t)hit->GetTrailTDC();
      if(id_plane<16) zz[layer-1]=(Double_t)hit->GetWireZPosition();
      //TArtCore::Info(__FILE__,"Number of BDC: %d,%d,%d:%d,%d",i,id_plane-1,ch,tdc1,tdc2);
      if(nw[id_plane-1]<20){
	nentry++;//Total Hit for BDC
	iw[nw[id_plane-1]][id_plane-1]=ch;
	tww[nw[id_plane-1]][id_plane-1]=tdc2;
	tw[nw[id_plane-1]++][id_plane-1]=tdc1;
      }
    }
  }

  /*
  TArtCore::Info(__FILE__,"ResultUX, %d,%d,%d,%d",nw[0],nw[1],nw[2],nw[3]);
  TArtCore::Info(__FILE__,"ResultUY, %d,%d,%d,%d",nw[4],nw[5],nw[6],nw[7]);
  TArtCore::Info(__FILE__,"ResultDX, %d,%d,%d,%d",nw[8],nw[9],nw[10],nw[11]);
  TArtCore::Info(__FILE__,"ResultDY, %d,%d,%d,%d",nw[12],nw[13],nw[14],nw[15]);
  TArtCore::Info(__FILE__,"Total Hit, %d",nentry);
  */
  /*
  cout<<nw[0]<<nw[1]<<nw[2]<<nw[3]<<nw[4]<<nw[5]<<nw[6]<<nw[7]
      <<nw[8]<<nw[9]<<nw[10]<<nw[11]<<nw[12]<<nw[13]<<nw[14]<<nw[15]<<endl;
  */
  /* first event */
  Int_t narray=0;
  Double_t chi, chimax=99999., xout[2];
  for(Int_t ud=0;ud<2;ud++){//BDC1or2 
    new ((*fBDCArray)[narray]) TArtBDC();
    TArtBDC *bdc = (TArtBDC*)fBDCArray->At(narray);
    narray++;
    for(Int_t xy=0;xy<2;xy++){//BDC*XorY
	xtrue=99999.;	atrue=99999.;
      if(nw[8*ud+4*xy]>0&&nw[8*ud+4*xy+1]>0&&nw[8*ud+4*xy+2]>0&&nw[8*ud+4*xy+3]>0&&
	 //if(nw[8*ud+4*xy]>0&&nw[8*ud+4*xy+1]>0&&nw[8*ud+4*xy+2]>0&&nw[8*ud+4*xy+3]>=0&&
	 nw[8*ud+4*xy]<10&&nw[8*ud+4*xy+1]<10&&nw[8*ud+4*xy+2]<10&&nw[8*ud+4*xy+3]<10){
	chimax=99999.;
	for(Int_t l1=0;l1<nw[8*ud+4*xy+0];l1++){
	  for(Int_t l2=0;l2<nw[8*ud+4*xy+1];l2++){
	    for(Int_t l3=0;l3<nw[8*ud+4*xy+2];l3++){
	      for(Int_t l4=0;l4<nw[8*ud+4*xy+3];l4++){
		tch=tw[l1][8*ud+4*xy+0];
		drft1=bstc1*sqrt(tch)+bstc2*tch+bstc3*tch*tch
		  +bstc4*tch*tch*tch+bstc5*tch*tch*tch*tch;
		tch=tw[l2][8*ud+4*xy+1];
		drft2=bstc1*sqrt(tch)+bstc2*tch+bstc3*tch*tch
		  +bstc4*tch*tch*tch+bstc5*tch*tch*tch*tch;
		tch=tw[l3][8*ud+4*xy+2];
		drft3=bstc1*sqrt(tch)+bstc2*tch+bstc3*tch*tch
		  +bstc4*tch*tch*tch+bstc5*tch*tch*tch*tch;
		tch=tw[l4][8*ud+4*xy+3];
		drft4=bstc1*sqrt(tch)+bstc2*tch+bstc3*tch*tch
		  +bstc4*tch*tch*tch+bstc5*tch*tch*tch*tch;
		/*
		drft1=0.0007*(tw[l1][8*ud+4*xy+0]);
		drft2=0.0007*(tw[l2][8*ud+4*xy+1]);
		drft3=0.0007*(tw[l3][8*ud+4*xy+2]);
		drft4=0.0007*(tw[l4][8*ud+4*xy+3]);
		*/
		//cout<<endl<<"WIRE:"<<
		//iw[l1][8*ud+4*xy+0]<<":"<<iw[l2][8*ud+4*xy+1]<<":"<<
		//iw[l3][8*ud+4*xy+2]<<":"<<iw[l4][8*ud+4*xy+3]<<endl;
		//cout<<"DRFT:"<<drft1<<":"<<drft2<<":"<<drft3<<":"<<drft4<<endl;
		for(Int_t i0=0;i0<16;i0++){//LR loop
		  //cout<<2*(i0>>0&0x1)-1<<":"<<2*(i0>>1&0x1)-1<<":"<<2*(i0>>2&0x1)-1<<":"<<2*(i0>>3&0x1)-1<<endl;
		  if(xy==0){
		    //changed
		    xx[0]=0.5*(iw[l1][8*ud+4*xy+0]-0.5)+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=0.5*(iw[l2][8*ud+4*xy+1]    )+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=0.5*(iw[l3][8*ud+4*xy+2]-0.5)+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=0.5*(iw[l4][8*ud+4*xy+3]    )+((i0>>3&0x1)*2-1)*drft4;
		  }else{
		    xx[0]=0.5*(iw[l1][8*ud+4*xy+0]-0.5)+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=0.5*(iw[l2][8*ud+4*xy+1]    )+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=0.5*(iw[l3][8*ud+4*xy+2]-0.5)+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=0.5*(iw[l4][8*ud+4*xy+3]    )+((i0>>3&0x1)*2-1)*drft4;
		  }
		  //cout<<ud<<xy<<" "<<iw[l1][8*ud+4*xy+0]-0.5<<" "<<iw[l2][8*ud+4*xy+1]
		  //<<" "<<iw[l3][8*ud+4*xy+2]-0.5<<" "<<iw[l4][8*ud+4*xy+3]<<endl;
		  //cout<<ud<<xy<<" "<<xx[0]<<" "<<xx[1]<<" "<<xx[2]<<" "<<xx[3]<<endl;
		  TMatrixD xvec(2,1); xvec.Zero();
		  TMatrixD xmat(2,2); xmat.Zero();
		  for(Int_t ii=0;ii<4;ii++){
		    xmat(0,0) ++ ;
		    xmat(0,1) += zz[ii+4*xy] ;
		    xmat(1,0) = xmat(0,1);
		    xmat(1,1) += zz[ii+4*xy]*zz[ii+4*xy] ;
		    xvec(0,0) += xx[ii];
		    xvec(1,0) += zz[ii+4*xy]*xx[ii];
		  }
		  Double_t det = xmat(0,0) * xmat(1,1) - xmat(0,1) * xmat(1,0);
		  if(fabs(det)>1e-5){
		    TMatrixD rxvec = xmat.Invert() * xvec;
		    xout[0] = rxvec(0,0);
		    xout[1] = rxvec(1,0);
		    chi = 0;
		    for(Int_t ii=0;ii<4;ii++)
		      chi += pow(xout[0]+xout[1]*zz[ii+4*xy] - xx[ii],2);
		    if(chi<chimax&&chi>1e-10){
		      //cout<<"TEST:"<<i0<<":"<<chi<<":"<<xout[1]*180/3.14<<endl;
		      chimax=chi;
		      xtrue=xout[0];
		      atrue=xout[1];
		      for(Int_t j=0;j<4;j++)
			dxx[j]=xtrue+atrue*zz[j+4*xy]-xx[j];//residual
		      drft[0]=((i0>>0&0x1)*2-1)*drft1;
		      drft[1]=((i0>>1&0x1)*2-1)*drft2;
		      drft[2]=((i0>>2&0x1)*2-1)*drft3;
		      drft[3]=((i0>>3&0x1)*2-1)*drft4;
		      tdc[0]=tw[l1][8*ud+4*xy+0];
		      tdc[1]=tw[l2][8*ud+4*xy+1];
		      tdc[2]=tw[l3][8*ud+4*xy+2];
		      tdc[3]=tw[l4][8*ud+4*xy+3];
		      //cout<<drft[0]<<" "<<drft[1]<<" "<<drft[2]<<" "<<drft[3]<<endl;
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
      //cout<<ud<<xy<<" "<<xtrue<<"(cm) "<<atrue*180/3.14<<"(deg) "<<chimax<<endl; 
      //cout<<chimax<<":"<<atrue*180/3.14<<":"<<ud<<":"<<xy<<endl;
      //bdc->SetLayer(ud+1);
      bdc->SetCHI2(chimax);
      for(Int_t i=0;i<4;i++){
	bdc->SetRes(4*xy+i,dxx[i]);
	bdc->SetDrf(4*xy+i,drft[i]);
	bdc->SetTDC(4*xy+i,tdc[i]);
      }
      if(xy==0){
	bdc->SetBDCX(xtrue);
	bdc->SetBDCA(atrue);
	bdc->SetRes1(dxx[0]);
	bdc->SetRes2(dxx[1]);
	bdc->SetRes3(dxx[2]);
	bdc->SetRes4(dxx[3]);
	bdc->SetDrf1(drft[0]);
	bdc->SetDrf2(drft[1]);
	bdc->SetDrf3(drft[2]);
	bdc->SetDrf4(drft[3]);
	//cout<<dxx[0]<<":"<<drft[0]<<endl;
      }else{
	bdc->SetBDCY(xtrue);
	bdc->SetBDCB(atrue);
	bdc->SetRes5(dxx[0]);
	bdc->SetRes6(dxx[1]);
	bdc->SetRes7(dxx[2]);
	bdc->SetRes8(dxx[3]);
	bdc->SetDrf5(drft[0]);
	bdc->SetDrf6(drft[1]);
	bdc->SetDrf7(drft[2]);
	bdc->SetDrf8(drft[3]);
      }
      //LR loop
    }//xy
    bdc->SetLayer(ud+1);
    bdc->SetHit1(nw[8*ud+0]);
    bdc->SetHit2(nw[8*ud+1]);
    bdc->SetHit3(nw[8*ud+2]);
    bdc->SetHit4(nw[8*ud+3]);
    bdc->SetHit5(nw[8*ud+4]);
    bdc->SetHit6(nw[8*ud+5]);
    bdc->SetHit7(nw[8*ud+6]);
    bdc->SetHit8(nw[8*ud+7]);
    for(Int_t i=0;i<8;i++)
      bdc->SetHit(i,nw[8*ud+i]);
    //cout<<"TEST:"<<fBDCArray->GetEntries()<<endl;
  }//ud
  //cout<<"ENDevent"<<endl<<endl;
  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtBDC * TArtCalibBDC::GetBDC(Int_t i) {
  return (TArtBDC *)fBDCArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibBDC::GetNumBDC() {
  return fBDCArray->GetEntries();
}

