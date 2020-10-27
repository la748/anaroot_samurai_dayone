#include "TArtCalibBLD.hh" 
#include "TArtBLD.hh"
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
TArtCalibBLD::TArtCalibBLD()
  : TArtReconstruction("ESPRIBLD")
{
  TArtCore::Info(__FILE__,"Creating the BLD calibration objects...");
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

  fBLDArray = new TClonesArray("TArtBLD",10);
  fBLDArray->SetName("ESPRIBLD");
  fBLDArray->SetOwner();
  sman->AddDataContainer(fBLDArray);

}

//__________________________________________________________
TArtCalibBLD::~TArtCalibBLD()  {
  ClearData();

  delete fBLDArray;
}

//__________________________________________________________
void TArtCalibBLD::ClearData()   {
  fBLDArray->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibBLD::ReconstructData()  {

  Int_t nw[36];//multiplicity
  Int_t iw[20][36];//channel ID at each
  Int_t tw[20][36];// TDC, iplane,ch,num up to 20 hit/plane
  Int_t tww[20][36];//only X1
  Double_t zz[36];//
  Double_t wang[36];//

  Int_t tdc[4];
  Double_t drft1,drft2,drft3,drft4,drft[4];
  Double_t xx[4],dxx[4];
  Double_t init_x,xtrue,atrue,xerr,aerr;
  Double_t init_y,ytrue,btrue,yerr,berr;
  Double_t pi,r2d,d2r;

  pi=4.*atan(1.);
  r2d=180./pi;
  d2r=pi/180.;

  TArtCore::Debug(__FILE__,"ReconstructData");
  //TArtCore::Info(__FILE__,"ReconstructData");

  TClonesArray *tdctrk =(TClonesArray *)sman->FindDataContainer("ESPRITDC");
  if(!tdctrk) return;

  const Double_t wspace[6]={0.3,0.5,0.9,0.9,0.5,0.5};
  const Double_t doffset[12]={8.75,8.75,9.00,0.0,12.5,0.0,12.5,0.0,10.75,6.75,10.75,6.75};
  //const Double_t test = setup2->FindDoublePara("dprm0");
  //const Double_t bstc1 = setup2->FindDoublePara("bstc1")*0.25;
  Double_t bstc1[6], bstc2[6], bstc3[6], bstc4[6], bstc5[6];
  for(Int_t i=0;i<6;i++){
    bstc1[i]=0.; bstc2[i]=0.; bstc3[i]=0.; bstc4[i]=0.; bstc5[i]=0.;
    /*
    Double_t length;
    length = wspace[i]/2.;
    bstc1[i] = setup2->FindDoublePara("bstc1")*length;
    bstc2[i] = setup2->FindDoublePara("bstc2")*length;
    bstc3[i] = setup2->FindDoublePara("bstc3")*length;
    bstc4[i] = setup2->FindDoublePara("bstc4")*length;
    bstc5[i] = setup2->FindDoublePara("bstc5")*length;
    */
  }
  bstc1[0] = setup2->FindDoublePara("b31stc1")*0.15;
  bstc2[0] = setup2->FindDoublePara("b31stc2")*0.15;
  bstc3[0] = setup2->FindDoublePara("b31stc3")*0.15;
  bstc4[0] = setup2->FindDoublePara("b31stc4")*0.15;
  bstc5[0] = setup2->FindDoublePara("b31stc5")*0.15;
  bstc1[1] = setup2->FindDoublePara("b32stc1")*0.25;
  bstc2[1] = setup2->FindDoublePara("b32stc2")*0.25;
  bstc3[1] = setup2->FindDoublePara("b32stc3")*0.25;
  bstc4[1] = setup2->FindDoublePara("b32stc4")*0.25;
  bstc5[1] = setup2->FindDoublePara("b32stc5")*0.25;
  bstc1[2] = setup2->FindDoublePara("b51stc1")*0.45;
  bstc2[2] = setup2->FindDoublePara("b51stc2")*0.45;
  bstc3[2] = setup2->FindDoublePara("b51stc3")*0.45;
  bstc4[2] = setup2->FindDoublePara("b51stc4")*0.45;
  bstc5[2] = setup2->FindDoublePara("b51stc5")*0.45;
  bstc1[3] = bstc1[2];
  bstc2[3] = bstc2[2];
  bstc3[3] = bstc3[2];
  bstc4[3] = bstc4[2];
  bstc5[3] = bstc5[2];
  bstc1[4] = setup2->FindDoublePara("b71stc1")*0.25;
  bstc2[4] = setup2->FindDoublePara("b71stc2")*0.25;
  bstc3[4] = setup2->FindDoublePara("b71stc3")*0.25;
  bstc4[4] = setup2->FindDoublePara("b71stc4")*0.25;
  bstc5[4] = setup2->FindDoublePara("b71stc5")*0.25;
  bstc1[5] = bstc1[4];
  bstc2[5] = bstc2[4];
  bstc3[5] = bstc3[4];
  bstc4[5] = bstc4[4];
  bstc5[5] = bstc5[4];

  Double_t tch;
  Int_t nrt_tdc = tdctrk->GetEntriesFast();

  for(Int_t i=0;i<36;i++){
    nw[i]=0;
    for(Int_t j=0;j<20;j++){
      iw[j][i]=-1; tw[j][i]=-9999;
      if(i<2) tww[j][i]=-9999;
    }
  }
  //cout<<":::"<<nrt_tdc<<endl;

  Int_t nentry=0;
  for(Int_t i=0;i<nrt_tdc;i++){
    TArtTDCHit *hit = (TArtTDCHit*)tdctrk->At(i);
    Int_t id_plane = (Double_t)hit->GetPlaneID();
    //cout<<i<<":"<<id_plane<<endl;
    if(id_plane>60&&id_plane<=96){//BLD
      Int_t layer = (Double_t)hit->GetLayer();
      Int_t ch  = (Double_t)hit->GetWireID();
      Int_t tdc1 = (Double_t)hit->GetTDC();
      Int_t tdc2 = (Double_t)hit->GetTrailTDC();
      //if(id_plane<16) zz[layer-1]=(Double_t)hit->GetWireZPosition();
      zz[layer-1]=(Double_t)hit->GetWireZPosition();
      //wang[layer-1]=180.-(Double_t)hit->GetWireAngle();
      wang[layer-1]=(Double_t)hit->GetWireAngle();
      //TArtCore::Info(__FILE__,"Number of BLD: %d,%d,%d:%d,%d",i,id_plane-1,ch,tdc1,tdc2);
      if((id_plane>76&&nw[id_plane-61]<20&&tdc1<2000&&tdc1>-1000)||
	 (id_plane<=76&&nw[id_plane-61]<20&&tdc1<800&&tdc1>-200)){
	nentry++;//Total Hit for BLD
	iw[nw[id_plane-61]][id_plane-61]=ch;
	tww[nw[id_plane-61]][id_plane-61]=tdc2;
	tw[nw[id_plane-61]++][id_plane-61]=tdc1;
      }
    }
  }
  
  /*
  TArtCore::Info(__FILE__,"ResultUX, %d,%d,%d,%d",nw[0],nw[1],nw[2],nw[3]);
  TArtCore::Info(__FILE__,"ResultUY, %d,%d,%d,%d",nw[4],nw[5],nw[6],nw[7]);
  TArtCore::Info(__FILE__,"ResultDX, %d,%d,%d,%d",nw[8],nw[9],nw[10],nw[11]);
  TArtCore::Info(__FILE__,"ResultDY, %d,%d,%d,%d",nw[12],nw[13],nw[14],nw[15]);
  TArtCore::Info(__FILE__,"Total Hit, %d",nentry);

  cout<<nw[0]<<nw[1]<<nw[2]<<nw[3]<<nw[4]<<nw[5]<<nw[6]<<nw[7]<<endl;
  cout<<nw[8]<<nw[9]<<nw[10]<<nw[11]<<endl;
  cout<<nw[12]<<nw[13]<<nw[14]<<nw[15]<<nw[16]<<nw[17]<<nw[18]<<nw[19]<<endl;
  cout<<nw[20]<<nw[21]<<nw[22]<<nw[23]<<nw[24]<<nw[25]<<nw[26]<<nw[27]<<endl;
  cout<<nw[28]<<nw[29]<<nw[30]<<nw[31]<<nw[32]<<nw[33]<<nw[34]<<nw[35]<<endl;
  */
  //return;

  /* first event */

  Int_t narray=0;
  Double_t chi, chimax=99999., xout[4];
  for(Int_t ud=0;ud<6;ud++){//BLD31,32,51,52,71,72
    new ((*fBLDArray)[narray]) TArtBLD();
    TArtBLD *bld = (TArtBLD*)fBLDArray->At(narray);
    narray++;
    for(Int_t xy=0;xy<2;xy++){//BLD*XorY
      if(ud>0&&ud<4&&xy==1) continue;//except 32,51,52
      //cout<<ud<<":"<<xy<<endl;
      xtrue=99999.;	atrue=99999.;
      Int_t player=4*ud+4*xy+(2*ud/(ud+1))*4+(ud/5)*4;
      if(nw[player]>0&&nw[player+1]>0&&nw[player+2]>0&&nw[player+3]>0&&
	 nw[player]<10&&nw[player+1]<10&&nw[player+2]<10&&nw[player+3]<10){
	chimax=99999.;
        for(Int_t l1=0;l1<nw[player+0];l1++){
	  for(Int_t l2=0;l2<nw[player+1];l2++){
	    for(Int_t l3=0;l3<nw[player+2];l3++){
	      for(Int_t l4=0;l4<nw[player+3];l4++){
		tch=tw[l1][player+0];
		drft1=bstc1[ud]*sqrt(abs(tch))+bstc2[ud]*tch+bstc3[ud]*tch*tch
		  +bstc4[ud]*tch*tch*tch+bstc5[ud]*tch*tch*tch*tch;
		tch=tw[l2][player+1];
		drft2=bstc1[ud]*sqrt(abs(tch))+bstc2[ud]*tch+bstc3[ud]*tch*tch
		  +bstc4[ud]*tch*tch*tch+bstc5[ud]*tch*tch*tch*tch;
		tch=tw[l3][player+2];
		drft3=bstc1[ud]*sqrt(abs(tch))+bstc2[ud]*tch+bstc3[ud]*tch*tch
		  +bstc4[ud]*tch*tch*tch+bstc5[ud]*tch*tch*tch*tch;
		tch=tw[l4][player+3];
		drft4=bstc1[ud]*sqrt(abs(tch))+bstc2[ud]*tch+bstc3[ud]*tch*tch
		  +bstc4[ud]*tch*tch*tch+bstc5[ud]*tch*tch*tch*tch;
		/*
		//if(ud==2||ud==3){
		if(ud==4||ud==5){
		  cout<<ud<<" "<<xy<<endl;
		  cout<<"WIRE:"<<iw[l1][player+0]<<":"<<iw[l2][player+1]<<":"<<iw[l3][player+2]<<":"<<iw[l4][player+3]<<endl;
		  cout<<"TDC::"<<tw[l1][player+0]<<":"<<tw[l2][player+1]<<":"<<tw[l3][player+2]<<":"<<tw[l4][player+3]<<endl;
		  cout<<"DRFT:"<<drft1<<":"<<drft2<<":"<<drft3<<":"<<drft4<<endl;
		}
		*/
		for(Int_t i0=0;i0<16;i0++){//LR loop
		  //cout<<2*(i0>>0&0x1)-1<<":"<<2*(i0>>1&0x1)-1<<":"<<2*(i0>>2&0x1)-1<<":"<<2*(i0>>3&0x1)-1<<endl;
		  //cout<<ud<<" "<<xy<<":"<<doffset[2*ud+xy]<<" "<<wspace[ud]<<endl;
		  if(ud==2){//BLD51
		    xx[0]=wspace[ud]*(iw[l1][player+0]+0.5-doffset[2*ud+xy])+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=wspace[ud]*(iw[l2][player+1]    -doffset[2*ud+xy])+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=wspace[ud]*(iw[l3][player+2]    -doffset[2*ud+xy])+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=wspace[ud]*(iw[l4][player+3]    -doffset[2*ud+xy])+((i0>>3&0x1)*2-1)*drft4;
		  }else if(ud==3){//BLD52
		    xx[0]=wspace[ud]*(iw[l1][player+0]+0.5-doffset[2*ud+xy])+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=wspace[ud]*(iw[l2][player+1]    -doffset[2*ud+xy])+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=wspace[ud]*(iw[l3][player+2]+0.5-doffset[2*ud+xy])+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=wspace[ud]*(iw[l4][player+3]-0.5-doffset[2*ud+xy])+((i0>>3&0x1)*2-1)*drft4;
		  }else if(ud>=4&&xy==0){//BLD71X,72X
		    xx[0]=wspace[ud]*(iw[l1][player+0]+0.5-doffset[2*ud+xy])+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=wspace[ud]*(iw[l2][player+1]    -doffset[2*ud+xy])+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=wspace[ud]*(iw[l3][player+2]+0.5-doffset[2*ud+xy])+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=wspace[ud]*(iw[l4][player+3]    -doffset[2*ud+xy])+((i0>>3&0x1)*2-1)*drft4;
		  }else{//BLD31,32
		    xx[0]=wspace[ud]*(iw[l1][player+0]    -doffset[2*ud+xy])+((i0>>0&0x1)*2-1)*drft1;
		    xx[1]=wspace[ud]*(iw[l2][player+1]+0.5-doffset[2*ud+xy])+((i0>>1&0x1)*2-1)*drft2;
		    xx[2]=wspace[ud]*(iw[l3][player+2]    -doffset[2*ud+xy])+((i0>>2&0x1)*2-1)*drft3;
		    xx[3]=wspace[ud]*(iw[l4][player+3]+0.5-doffset[2*ud+xy])+((i0>>3&0x1)*2-1)*drft4;
		  }
		  /*
		  cout<<ud<<xy<<" "<<i0<<" "<<iw[l1][player+0]-0.5<<" "<<iw[l2][player+1]
		      <<" "<<iw[l3][player+2]-0.5<<" "<<iw[l4][player+3]<<endl;
		  */
		  
		  //if(ud==2||ud==3)
		  //cout<<ud<<xy<<" "<<xx[0]<<" "<<xx[1]<<" "<<xx[2]<<" "<<xx[3]<<endl;

		  //TMatrixD xmat0(4,4); xmat0.UnitMatrix();
		  //TMatrixD xvec0(4,1); xvec0.Zero();
		  //TMatrixD xmat0(4,4); xmat0.Zero();
		  Double_t det=1.;
		  TMatrixD xvec1(2,1); xvec1.Zero();
		  TMatrixD xmat1(2,2); xmat1.Zero();
		  TMatrixD xvec2(4,1); xvec2.Zero();
		  TMatrixD xmat2(4,4); xmat2.Zero();
		  if(ud==0||ud>=4){//4x4 F3 & F7
		    for(Int_t ii=0;ii<4;ii++){
		      xmat1(0,0) ++ ;
		      xmat1(0,1) += zz[ii+player] ;
		      xmat1(1,0) = xmat1(0,1);
		      xmat1(1,1) += zz[ii+player]*zz[ii+player] ;
		      xvec1(0,0) += xx[ii];
		      xvec1(1,0) += zz[ii+player]*xx[ii];
		    }
		    det = xmat1(0,0) * xmat1(1,1) - xmat1(0,1) * xmat1(1,0);
		  }else if(ud==2||ud==3){//F5
		    for(Int_t ii=0;ii<4;ii++){
		      //cout<<ud<<":"<<ii<<"::"<<zz[ii+player]<<":"<<wang[ii+player]<<endl;
		      xmat2(0,0) += sin(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r);
		      xmat2(0,1) += sin(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r)*zz[ii+player];
		      xmat2(1,0) = xmat2(0,1);
		      xmat2(1,1) += sin(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r)
			*zz[ii+player]*zz[ii+player];
		      xmat2(0,2) += cos(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r);
		      xmat2(0,3) += cos(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r)*zz[ii+player];
		      xmat2(1,2) = xmat2(0,3);
		      xmat2(1,3) += cos(wang[ii+player]*d2r)*sin(wang[ii+player]*d2r)
			*zz[ii+player]*zz[ii+player];
		      xmat2(2,0) = xmat2(0,2);
		      xmat2(2,1) = xmat2(0,3);
		      xmat2(3,0) = xmat2(1,2);
		      xmat2(3,1) = xmat2(1,3);
		      xmat2(2,2) += cos(wang[ii+player]*d2r)*cos(wang[ii+player]*d2r);
		      xmat2(2,3) += cos(wang[ii+player]*d2r)*cos(wang[ii+player]*d2r)*zz[ii+player];
		      xmat2(3,2) = xmat2(2,3);
		      xmat2(3,3) += cos(wang[ii+player]*d2r)*cos(wang[ii+player]*d2r)
			*zz[ii+player]*zz[ii+player];

		      xvec2(0,0) += sin(wang[ii+player]*d2r)*xx[ii];
		      xvec2(1,0) += sin(wang[ii+player]*d2r)*zz[ii+player]*xx[ii];
		      xvec2(2,0) += cos(wang[ii+player]*d2r)*xx[ii];
		      xvec2(3,0) += cos(wang[ii+player]*d2r)*zz[ii+player]*xx[ii];
		    }
		    /*
		    cout<<"Matrix"<<endl;
		    cout<<xmat2(0,0)<<" "<<xmat2(0,1)<<" "<<xmat2(0,2)<<" "<<xmat2(0,3)<<":"<<xvec2(0,0)<<endl;
		    cout<<xmat2(1,0)<<" "<<xmat2(1,1)<<" "<<xmat2(1,2)<<" "<<xmat2(1,3)<<":"<<xvec2(1,0)<<endl;
		    cout<<xmat2(2,0)<<" "<<xmat2(2,1)<<" "<<xmat2(2,2)<<" "<<xmat2(2,3)<<":"<<xvec2(2,0)<<endl;
		    cout<<xmat2(3,0)<<" "<<xmat2(3,1)<<" "<<xmat2(3,2)<<" "<<xmat2(3,3)<<":"<<xvec2(3,0)<<endl;
		    cout<<endl;
		    TMatrixD tmat = xmat2.Invert();
		    cout<<"Matrix"<<endl;
		    cout<<tmat(0,0)<<" "<<tmat(0,1)<<" "<<tmat(0,2)<<" "<<tmat(0,3)<<"::"<<xvec2(0,0)<<endl;
		    cout<<tmat(1,0)<<" "<<tmat(1,1)<<" "<<tmat(1,2)<<" "<<tmat(1,3)<<"::"<<xvec2(1,0)<<endl;
		    cout<<tmat(2,0)<<" "<<tmat(2,1)<<" "<<tmat(2,2)<<" "<<tmat(2,3)<<"::"<<xvec2(2,0)<<endl;
		    cout<<tmat(3,0)<<" "<<tmat(3,1)<<" "<<tmat(3,2)<<" "<<tmat(3,3)<<"::"<<xvec2(3,0)<<endl;
		    cout<<endl;
		    */
		  }
		  //cout<<"DET:"<<det<<endl;
		  //if(fabs(det)>1e-5){
		    if(ud==0||ud>=4){//BLD31,71,72
		      TMatrixD rxvec = xmat1.Invert() * xvec1;
		      xout[0] = rxvec(0,0);
		      xout[1] = rxvec(1,0);
		    }else if(ud==2||ud==3){//BLD51,52
		      TMatrixD rxvec = xmat2.Invert() * xvec2;
		      xout[0] = rxvec(0,0);//X
		      xout[1] = rxvec(1,0);//A
		      xout[2] = rxvec(2,0);//Y
		      xout[3] = rxvec(3,0);//B
		      //cout<<"RESULT:"<<xout[0]<<" "<<xout[1]<<" "<<xout[2]<<" "<<xout[3]<<endl;
		      //cout<<"MES:"<<xx[0]<<" "<<xx[1]<<" "<<xx[2]<<" "<<xx[3]<<endl;
		    }else{ //BLD32
		      xout[0] = 0.5*(xx[0]+xx[1]);
		      xout[1] = (xx[1]-xx[0])/(zz[player+1]-zz[player]);
		      xout[2] = 0.5*(xx[2]+xx[3]);
		      xout[3] = (xx[3]-xx[2])/(zz[player+3]-zz[player+2]);
		    }
		    chi = 0;
		    if(ud==0||ud==4||ud==5){//xxyyxxyy
		      for(Int_t ii=0;ii<4;ii++)
			chi += pow(xout[0]+xout[1]*zz[ii+player] - xx[ii],2);
		    }else if(ud==2||ud==3){//xxuv
		      for(Int_t ii=0;ii<4;ii++){
			chi += pow((xout[0]+xout[1]*zz[ii+player])*sin(wang[ii+player]*d2r)
				   +(xout[2]+xout[3]*zz[ii+player])*cos(wang[ii+player]*d2r)
				   - xx[ii],2);//u,v
			//cout<<ii<<" "<<i0<<" "<<chi<<endl;
		      }

		    }
		    //BDC31,71,72 also BLD51,52
		    if(ud==2||ud==3){//F5
		      if(fabs(xout[1])<0.15&&fabs(xout[3])<0.15){//around 10 degree
			xtrue=xout[0];
			atrue=xout[1];
			ytrue=xout[2];
			btrue=xout[3];
			for(Int_t j=0;j<4;j++)
			  dxx[j]=(xtrue+atrue*zz[j+player])*sin(wang[j+player]*d2r)
			    +(ytrue+btrue*zz[j+player])*cos(wang[j+player]*d2r)-xx[j];//residual
			drft[0]=((i0>>0&0x1)*2-1)*drft1;
			drft[1]=((i0>>1&0x1)*2-1)*drft2;
			drft[2]=((i0>>2&0x1)*2-1)*drft3;
			drft[3]=((i0>>3&0x1)*2-1)*drft4;
			tdc[0]=tw[l1][player+0];
			tdc[1]=tw[l2][player+1];
			tdc[2]=tw[l3][player+2];
			tdc[3]=tw[l4][player+3];
		      }//better fit at BLD51,52
		    }else if(ud!=1){//BLD31,71,72
		      if(chi<chimax&&chi>1e-30){
			//cout<<ud<<"TEST:"<<i0<<":CHI:"<<chi<<":ANGLE:"<<xout[1]*180/3.14<<endl;
			//cout<<":"<<xx[0]<<":"<<xx[1]<<":"<<xx[2]<<":"<<xx[3]<<endl;
			chimax=chi;
			xtrue=xout[0];
			atrue=xout[1];
			for(Int_t j=0;j<4;j++)
			  dxx[j]=xtrue+atrue*zz[j+4*xy]-xx[j];//residual
			drft[0]=((i0>>0&0x1)*2-1)*drft1;
			drft[1]=((i0>>1&0x1)*2-1)*drft2;
			drft[2]=((i0>>2&0x1)*2-1)*drft3;
			drft[3]=((i0>>3&0x1)*2-1)*drft4;
			tdc[0]=tw[l1][player+0];
			tdc[1]=tw[l2][player+1];
			tdc[2]=tw[l3][player+2];
			tdc[3]=tw[l4][player+3];
			//cout<<drft[0]<<" "<<drft[1]<<" "<<drft[2]<<" "<<drft[3]<<endl;
		      }//better fit at BLD31,71,72
		    }else if(ud==1){
		      //}else if(ud==1&&i0==5&&abs(xx[0]-xx[1])<0.1&&abs(xx[2]-xx[3])<0.1){ //BLD32
		      //if(abs(xx[0]-xx[1])<0.5&&abs(xx[2]-xx[3])<0.5||i0==5){ //BLD32 Not solve LR use 1010
		      //if(i0==5&&abs(xx[0]-xx[1])<0.3){ //BLD32 Not solve LR use 1010
		      if(i0==5){ //BLD32 Not solve LR use 1010
		      //cout<<"NO FIT"<<xx[0]<<":"<<xx[1]<<"::"<<xx[2]<<":"<<xx[3]<<endl;
			chimax=chi;
			xtrue=0.5*(xx[0]+xx[1]);
			atrue=(xx[1]-xx[0])/(zz[player+1]-zz[player]);
			ytrue=0.5*(xx[2]+xx[3]);
			btrue=(xx[3]-xx[2])/(zz[player+3]-zz[player+2]);
			for(Int_t j=0;j<2;j++){
			  dxx[j]  =xtrue+atrue*zz[j+player]-xx[j];//residual
			  dxx[j+2]=ytrue+btrue*zz[j+player]-xx[j+2];//residual
			}
			drft[0]=((i0>>0&0x1)*2-1)*drft1;
			drft[1]=((i0>>1&0x1)*2-1)*drft2;
			drft[2]=((i0>>2&0x1)*2-1)*drft3;
			drft[3]=((i0>>3&0x1)*2-1)*drft4;
			tdc[0]=tw[l1][player+0];
			tdc[1]=tw[l2][player+1];
			tdc[2]=tw[l3][player+2];
			tdc[3]=tw[l4][player+3];
		      }//better fit at BLD32
		    }
		}//end of LR solve
	      }//Hit4 loop
	    }//Hit3 loop
	  }//Hit2 loop
	}//Hit1 loop
      }//Good Hit LOOP
      //cout<<ud<<xy<<" "<<xtrue<<"(cm) "<<atrue*180/3.14<<"(deg) "<<chimax<<endl; 
      //cout<<chimax<<":"<<atrue*180/3.14<<":"<<ud<<":"<<xy<<endl;
      bld->SetLayer(ud+1);
      bld->SetCHI2(chimax);
      for(Int_t i=0;i<4;i++){
	bld->SetRes(4*xy+i,dxx[i]);
	bld->SetDrf(4*xy+i,drft[i]);
	bld->SetTDC(4*xy+i,tdc[i]);
      }
      if(ud==0||ud==4||ud==5){//8 plane with Y
	if(xy==0){
	  bld->SetBLDX(xtrue);
	  bld->SetBLDA(atrue);
	  bld->SetRes1(dxx[0]);
	  bld->SetRes2(dxx[1]);
	  bld->SetRes3(dxx[2]);
	  bld->SetRes4(dxx[3]);
	  bld->SetDrf1(drft[0]);
	  bld->SetDrf2(drft[1]);
	  bld->SetDrf3(drft[2]);
	  bld->SetDrf4(drft[3]);
	  //cout<<dxx[0]<<":"<<drft[0]<<endl;
	}else{
	  bld->SetBLDY(xtrue);
	  bld->SetBLDB(atrue);
	  bld->SetRes5(dxx[0]);
	  bld->SetRes6(dxx[1]);
	  bld->SetRes7(dxx[2]);
	  bld->SetRes8(dxx[3]);
	  bld->SetDrf5(drft[0]);
	  bld->SetDrf6(drft[1]);
	  bld->SetDrf7(drft[2]);
	  bld->SetDrf8(drft[3]);
	}
      }else if(ud==1||ud==2||ud==3){//for plane no xy=1
	if(ud != 1){
	  bld->SetBLDX(-xtrue);//swapped
	  bld->SetBLDA(-atrue);//swapped
	}else{
	  bld->SetBLDX(xtrue);
	  bld->SetBLDA(atrue);
	}
	bld->SetBLDY(ytrue);
	bld->SetBLDB(btrue);
	bld->SetRes1(dxx[0]);
	bld->SetRes2(dxx[1]);
	bld->SetRes3(dxx[2]);
	bld->SetRes4(dxx[3]);
	bld->SetDrf1(drft[0]);
	bld->SetDrf2(drft[1]);
	bld->SetDrf3(drft[2]);
	bld->SetDrf4(drft[3]);
      }
    }//xy loop
    Int_t player=4*ud+(2*ud/(ud+1))*4+(ud/5)*4;
    bld->SetHit1(nw[player+0]);
    bld->SetHit2(nw[player+1]);
    bld->SetHit3(nw[player+2]);
    bld->SetHit4(nw[player+3]);
    if(ud==0||ud>=4){
      bld->SetHit5(nw[player+4]);
      bld->SetHit6(nw[player+5]);
      bld->SetHit7(nw[player+6]);
      bld->SetHit8(nw[player+7]);
    }
    for(Int_t i=0;i<8;i++)
      bld->SetHit(i,nw[player+i]);
    //cout<<"TEST:"<<fBLDArray->GetEntries()<<endl;
  }//ud loop
  //cout<<"ENDevent"<<endl<<endl;
  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtBLD * TArtCalibBLD::GetBLD(Int_t i) {
  return (TArtBLD *)fBLDArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibBLD::GetNumBLD() {
  return fBLDArray->GetEntries();
}

