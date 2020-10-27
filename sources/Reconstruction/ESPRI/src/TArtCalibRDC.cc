#include "TArtCalibRDC.hh" 
#include "TArtRDC.hh"
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
//#include <TMinuit.h>
//#include "Math/GSLSimAnMinimizer.h"
//#include "Math/GSLMinimizer.h"
#include "Math/Functor.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"


Double_t xx[7];//Global variable for Minuit
Double_t zz[7];//Global variable for Minuit

Double_t myFitFuncXX(const Double_t *xout ) {
  Double_t ans;
  Double_t tmp_z[3]={-2.0,0.0,2.0};//[X'-]X'-X-X
  Double_t res=0.02;//assumed to be 200 um in sigma

  const Double_t x0=xout[0];
  const Double_t th=xout[1];
  const Double_t data1=xx[0];
  const Double_t data2=xx[1];
  const Double_t data3=xx[2];

  // 0:x0, 1:theta0 
  ans = pow((x0+th*tmp_z[0]-data1)/res,2.) + pow((x0+th*tmp_z[1]-data2)/res,2.)
    + pow((x0+th*tmp_z[2]-data3)/res,2.);
  
  //cout<<"CHI:"<<ans<<":"<<x0<<" "<<th<<" "<<data1<<" "<<data2<<" "<<data3<<endl;
  return ans;
}

Double_t myFitFuncXX2(const Double_t *xout ) {
  Double_t ans;
  Double_t tmp_z[3]={-2.0,0.0,2.0};//[X'-]X'-X-X
  Double_t res=0.02;//assumed to be 200 um in sigma

  const Double_t x0=xout[0];
  const Double_t th=xout[1];
  const Double_t dx=xout[2];
  const Double_t data1=xx[0];
  const Double_t data2=xx[1];
  const Double_t data3=xx[2];

  // 0:x0, 1:theta0 
  ans = pow((x0+th*tmp_z[0]-data1+dx)/res,2.) + pow((x0+th*tmp_z[1]-data2+dx)/res,2.)
    + pow((x0+th*tmp_z[2]-data3+dx)/res,2.);
  
  //cout<<"CHI:"<<ans<<":"<<x0<<" "<<th<<" "<<data1<<" "<<data2<<" "<<data3<<endl;
  return ans;
}

//__________________________________________________________
TArtCalibRDC::TArtCalibRDC()
  : TArtReconstruction("ESPRIRDC")
{
  TArtCore::Info(__FILE__,"Creating the RDC calibration objects...");
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

  fRDCArray = new TClonesArray("TArtRDC",10);
  fRDCArray->SetName("ESPRIRDC");
  fRDCArray->SetOwner();
  sman->AddDataContainer(fRDCArray);

}

//__________________________________________________________
TArtCalibRDC::~TArtCalibRDC()  {
  ClearData();

  delete fRDCArray;
}

//__________________________________________________________
void TArtCalibRDC::ClearData()   {
  fRDCArray->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibRDC::ReconstructData()  {

  Int_t nw[14];//multiplicity
  Int_t iw[20][14];//channel ID at each
  Int_t tw[20][14];// TDC, iplane,ch,num up to 20 hit/plane
  Int_t tww[20][2];//only X1
  //Double_t zz[7];

  Double_t drft1,drft2,drft3,drft4,drft[4];
  //Double_t xx[7],dxx[7];
  Double_t dxx[7];
  Double_t ang,init_x,xtrue,atrue,xerr,aerr;

  //TArtCore::Debug(__FILE__,"ReconstructData");

  TClonesArray *tdctrk =(TClonesArray *)sman->FindDataContainer("ESPRITDC");
  if(!tdctrk) return;

  /*
  const Double_t rstc1 = setup2->FindDoublePara("rstc1");
  const Double_t rstc2 = setup2->FindDoublePara("rstc2");
  const Double_t rstc3 = setup2->FindDoublePara("rstc3");
  const Double_t rstc4 = setup2->FindDoublePara("rstc4");
  const Double_t rstc5 = setup2->FindDoublePara("rstc5");
  */
  //Double_t rstc[4][2];
  const Double_t rstc1 = setup2->FindDoublePara("rstc1")*0.7;
  const Double_t rstc2 = setup2->FindDoublePara("rstc2")*0.7;
  const Double_t rstc3 = setup2->FindDoublePara("rstc3")*0.7;
  const Double_t rstc4 = setup2->FindDoublePara("rstc4")*0.7;
  const Double_t rstc5 = setup2->FindDoublePara("rstc5")*0.7;
  const Double_t rstc6 = setup2->FindDoublePara("rstc6")*0.7;
  const Double_t rstc7 = setup2->FindDoublePara("rstc7")*0.7;
  const Double_t rstc8 = setup2->FindDoublePara("rstc8")*0.7;
  const Double_t rstc9 = setup2->FindDoublePara("rstc9")*0.7;
  const Double_t rstc10 = setup2->FindDoublePara("rstc10")*0.7;

  Double_t tch;
  Int_t nrt_tdc = tdctrk->GetEntriesFast();

  ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
  //TMinuit *min(4);
  //ROOT::Math::GSLMinimizer min(ROOT::Math::kConjugateFR);//Fletcher-Reeves conjugate
  //ROOT::Math::GSLSimAnMinimizer min;//simulated annealing from GSL

  min->SetMaxFunctionCalls(100000);
  min->SetMaxIterations(100000);
  min->SetTolerance(0.01);
  //min->SetTolerance(1);

  //ROOT::Math::Functor f(&myFitFuncXX,5);//3 measurement 2 parametor
  //ROOT::Math::Functor f(&myFitFuncXX,2);//3 measurement 2 parametor
  ROOT::Math::Functor f(&myFitFuncXX2,2);//3 measurement 3 parametor
  min->SetFunction(f);
  min->SetPrintLevel(-1);//no output
  Int_t ierr=0;
  //min->mnexcm("SET NOWarnings",0,0,ierr);

  for(Int_t i=0;i<14;i++){
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
    
    if(id_plane>16&&id_plane<=30){//RDC
      Int_t layer = (Double_t)hit->GetLayer();
      Int_t ch  = (Double_t)hit->GetWireID();
      Int_t tdc1 = (Double_t)hit->GetTDC();
      Int_t tdc2 = (Double_t)hit->GetTrailTDC();
      if(layer>1)
	zz[layer-2]=(Double_t)hit->GetWireZPosition();

      //TArtCore::Info(__FILE__,"Number of RDC: %d,%d,%d:%d,%d",i,id_plane-17,ch,tdc1,tdc2);
      if(nw[id_plane-17]<20){
	nentry++;//Total Hit for RDC
	iw[nw[id_plane-17]][id_plane-17]=ch;
	if(tdc2>0&&layer==1) tww[nw[id_plane-17]][(id_plane-17)/7]=tdc2;
	tw[nw[id_plane-17]++][id_plane-17]=tdc1;
	//if(tdc1>2000)	tw[nw[id_plane-17]++][id_plane-17]=-9999;

      }
    }
  }
  /*
  TArtCore::Info(__FILE__,"ResultUX, %d,%d,%d,%d",nw[0],nw[1],nw[2],nw[3]);
  TArtCore::Info(__FILE__,"ResultUY, %d,%d,%d",nw[4],nw[5],nw[6]);
  TArtCore::Info(__FILE__,"ResultDX, %d,%d,%d,%d",nw[7],nw[8],nw[9],nw[10]);
  TArtCore::Info(__FILE__,"ResultDY, %d,%d,%d",nw[11],nw[12],nw[13]);
  TArtCore::Info(__FILE__,"Total Hit, %d",nentry);
  */
  /* first event */
  Int_t narray=0;
  //Double_t chi, chimax;
  Double_t chi, chimax=99999., xout[2];
  for(Int_t ud=0;ud<2;ud++){
    new ((*fRDCArray)[narray]) TArtRDC();
    TArtRDC *rdc = (TArtRDC*)fRDCArray->At(narray);//only 1 array/event;
    narray++;
    for(Int_t xy=0;xy<2;xy++){
      //if(nw[7*ud+4*xy]>0&&nw[7*ud+3*xy+1]>0&&nw[7*ud+3*xy+2]>0){
      if(nw[7*ud+3*xy+1]>0&&nw[7*ud+3*xy+2]>0&&nw[7*ud+3*xy+3]>0&&
	 nw[7*ud+3*xy+1]<10&&nw[7*ud+3*xy+2]<10&&nw[7*ud+3*xy+3]<10){
	chimax=99999;
	for(Int_t l1=0;l1<nw[7*ud+3*xy+1];l1++){
	  for(Int_t l2=0;l2<nw[7*ud+3*xy+2];l2++){
	    for(Int_t l3=0;l3<nw[7*ud+3*xy+3];l3++){
	      if(xy==0){
		tch=-tw[l1][7*ud+3*xy+1];
		drft1=rstc1*sqrt(fabs(tch))+rstc2*tch+rstc3*tch*tch
		  +rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
		tch=-tw[l2][7*ud+3*xy+2];
		drft2=rstc1*sqrt(fabs(tch))+rstc2*tch+rstc3*tch*tch
		  +rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
		tch=-tw[l3][7*ud+3*xy+3];
		drft3=rstc1*sqrt(fabs(tch))+rstc2*tch+rstc3*tch*tch
		  +rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
		tch=-tw[0][7*ud+3*xy];
		drft4=rstc1*sqrt(fabs(tch))+rstc2*tch+rstc3*tch*tch
		  +rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
	      }else{
		tch=-tw[l1][7*ud+3*xy+1];
		drft1=rstc6*sqrt(fabs(tch))+rstc7*tch+rstc8*tch*tch
		  +rstc9*tch*tch*tch+rstc10*tch*tch*tch*tch;
		tch=-tw[l2][7*ud+3*xy+2];
		drft2=rstc6*sqrt(fabs(tch))+rstc7*tch+rstc8*tch*tch
		  +rstc9*tch*tch*tch+rstc10*tch*tch*tch*tch;
		tch=-tw[l3][7*ud+3*xy+3];
		drft3=rstc6*sqrt(fabs(tch))+rstc7*tch+rstc8*tch*tch
		  +rstc9*tch*tch*tch+rstc10*tch*tch*tch*tch;
		tch=-tw[0][7*ud+3*xy];
		drft4=rstc6*sqrt(fabs(tch))+rstc7*tch+rstc8*tch*tch
		  +rstc9*tch*tch*tch+rstc10*tch*tch*tch*tch;
	      }
	      /*
	      drft1=rstc1*sqrt(tch)+rstc2*tch+rstc3*tch*tch
		+rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
	      tch=-tw[l2][7*ud+3*xy+2];
	      drft2=rstc1*sqrt(tch)+rstc2*tch+rstc3*tch*tch
		+rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
	      tch=-tw[l3][7*ud+3*xy+3];
	      drft3=rstc1*sqrt(tch)+rstc2*tch+rstc3*tch*tch
		+rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
	      tch=-tw[0][7*ud+3*xy];
	      drft4=rstc1*sqrt(tch)+rstc2*tch+rstc3*tch*tch
		+rstc4*tch*tch*tch+rstc5*tch*tch*tch*tch;
	      */
	      /*
	      drft1=0.004*(-tw[l1][7*ud+3*xy+1]);
	      drft2=0.004*(-tw[l2][7*ud+3*xy+2]);
	      drft3=0.004*(-tw[l3][7*ud+3*xy+3]);
	      drft4=0.004*(-tw[0][7*ud+3*xy]);
	      */
	      //cout<<endl<<"WIRE:"<<iw[l1][7*ud+3*xy+1]<<":"<<
	      //iw[l2][7*ud+3*xy+2]<<":"<<iw[l3][7*ud+3*xy+3]<<endl;
	      //cout<<"DRFT:"<<ud<<xy<<":"<<drft1<<":"<<drft2<<":"<<drft3<<endl;
	      for(Int_t i0=0;i0<8;i0++){//LR loop
		//cout<<2*(i0>>0&0x1)-1<<":"<<2*(i0>>1&0x1)-1<<":"<<2*(i0>>2&0x1)-1<<endl;
		if(xy==0){
		  xx[0]=1.4*(iw[l1][7*ud+3*xy+1]-0.5)+((i0>>0&0x1)*2-1)*drft1;
		  xx[1]=1.4*(iw[l2][7*ud+3*xy+2]    )+((i0>>1&0x1)*2-1)*drft2;
		  xx[2]=1.4*(iw[l3][7*ud+3*xy+3]    )+((i0>>2&0x1)*2-1)*drft3;
		  //xx[3]=1.4*(iw[0][7*ud+3*xy+0]-0.5 )+((i0>>0&0x1)*2-1)*drft4;//same
		}else{
		  xx[0]=1.4*(iw[l1][7*ud+3*xy+1]    )+((i0>>0&0x1)*2-1)*drft1;
		  xx[1]=1.4*(iw[l2][7*ud+3*xy+2]    )+((i0>>1&0x1)*2-1)*drft2;
		  xx[2]=1.4*(iw[l3][7*ud+3*xy+3]-0.5)+((i0>>2&0x1)*2-1)*drft3;
		}
		//cout<<"XX:"<<xx[0]<<":"<<xx[1]<<":"<<xx[2]<<endl;
		//cout<<"ZZ:"<<zz[0]<<":"<<zz[1]<<":"<<zz[2]<<endl;
		/*
		init_x=(xx[0]+xx[1]+xx[2])/3.;

		min->SetVariable(0,"x0",init_x,0.01);//cm
		min->SetVariable(1,"th",0.01,0.01);//radian
		//min->SetVariable(2,"data1",xx[1],0.0);//Measurement
		//min->SetVariable(3,"data2",xx[2],0.0);//Measurement
		//min->SetVariable(4,"data3",xx[3],0.0);//Measurement

		min->Minimize();//minimized
		const Double_t *xout = min->X();
		//cout<<xout[0]<<":"<<xout[1]*180./3.14<<":"<<xout[2]
		//<<":CHI:"<<myFitFuncXX(xout)<<endl;
		chi=myFitFuncXX(xout);
		*/
		TMatrixD xvec(2,1); xvec.Zero();
		TMatrixD xmat(2,2); xmat.Zero();
		for(Int_t ii=0;ii<3;ii++){
		  xmat(0,0) ++ ;
		  xmat(0,1) += zz[ii+3*xy] ;
		  xmat(1,0) = xmat(0,1);
		  xmat(1,1) += zz[ii+3*xy]*zz[ii+3*xy] ;
		  xvec(0,0) += xx[ii];
		  xvec(1,0) += zz[ii+3*xy]*xx[ii];
		}
		Double_t det = xmat(0,0) * xmat(1,1) - xmat(0,1) * xmat(1,0);
		if(fabs(det)>1e-5){
		  TMatrixD rxvec = xmat.Invert() * xvec;
		  xout[0] = rxvec(0,0);
		  xout[1] = rxvec(1,0);
		  chi = 0;
		  for(Int_t ii=0;ii<3;ii++)
		    chi += pow(xout[0]+xout[1]*zz[ii+3*xy] - xx[ii],2);
		  //cout<<"chi:"<<chi<<endl;
		  if(chi<chimax&&chi>1.e-10){
		    //cout<<"GOOD:"<<chi<<endl;
		    chimax=chi;
		    xtrue=xout[0];
		    atrue=xout[1];
		    //dtrue=xout[2];
		    //for(Int_t j=0;j<4-xy;j++){
		    for(Int_t j=0;j<3;j++)
		      dxx[j]=xtrue+atrue*2.0*(j-1)-xx[j];//residual
		    drft[0]=((i0>>0&0x1)*2-1)*drft1;
		    drft[1]=((i0>>1&0x1)*2-1)*drft2;
		    drft[2]=((i0>>2&0x1)*2-1)*drft3;
		    //cout<<drft[0]<<" "<<drft[1]<<" "<<drft[2]<<endl;
		  }
		}
	      }
	    }
	  }
	}
	//cout<<ud<<xy<<" "<<xtrue<<"(cm) "<<atrue*180/3.14<<"(deg) "<<chimax<<endl; 
	//TArtCore::Info(__FILE__,"%d%d %lf (cm) %lf (deg) %e",ud,xy,xtrue,atrue*180./3.14,chimax);
	//rdc->SetLayer(ud+1);
	rdc->SetCHI2(chimax);
	if(xy==0){
	  rdc->SetRDCX(xtrue);
	  rdc->SetRDCA(atrue);
	  //cout<<dxx[0]<<" "<<dxx[1]<<" "<<dxx[2]<<endl;
	  //cout<<drft[0]<<" "<<drft[1]<<" "<<drft[2]<<endl;
	  rdc->SetRes1(dxx[0]);
	  rdc->SetRes2(dxx[1]);
	  rdc->SetRes3(dxx[2]);
	  rdc->SetDrf1(drft[0]);
	  rdc->SetDrf2(drft[1]);
	  rdc->SetDrf3(drft[2]);
	}else{
	  rdc->SetRDCY(xtrue);
	  rdc->SetRDCB(atrue);
	  rdc->SetRes4(dxx[0]);
	  rdc->SetRes5(dxx[1]);
	  rdc->SetRes6(dxx[2]);
	  rdc->SetDrf4(drft[0]);
	  rdc->SetDrf5(drft[1]);
	  rdc->SetDrf6(drft[2]);
	}
      }//End of Good Track
    }//xy
    rdc->SetLayer(ud+1);
    rdc->SetHit1(nw[7*ud+1]);
    rdc->SetHit2(nw[7*ud+2]);
    rdc->SetHit3(nw[7*ud+3]);
    rdc->SetHit4(nw[7*ud+4]);
    rdc->SetHit5(nw[7*ud+5]);
    rdc->SetHit6(nw[7*ud+6]);
  }//ud

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtRDC * TArtCalibRDC::GetRDC(Int_t i) {
  return (TArtRDC *)fRDCArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibRDC::GetNumRDC() {
  return fRDCArray->GetEntries();
}

/*
//__________________________________________________________
void TArtCalibRDC::myFitFuncXUV(Int_t& nDim, Double_t* gout, Double_t& result, Double_t par[], Int_t flg) {
  Double_t ans,xx[6], tmp_z[6]={0.0,4.8,24.4,29.2,48.8,53.6};//V'-V-U'-U-X'-X  
  // 0:x0, 1:y0, 2:a0, 3:b0
  Double_t ang = 15./180.*3.14159;
  ans=0;
  ans=pow(cos(ang)*(par[0]+par[2]*tmp_z[0])-sin(ang)*(par[1]+par[3]*tmp_z[0])
	  -xx[0],2.)
    //V'-V                                                                          
    +pow(cos(ang)*(par[0]+par[2]*tmp_z[1])-sin(ang)*(par[1]+par[3]*tmp_z[1])
	 -xx[1],2.)
    
    +pow(cos(ang)*(par[0]+par[2]*tmp_z[2])+sin(ang)*(par[1]+par[3]*tmp_z[2])
	 -xx[2],2.)
    //U'-U                                                                          
    +pow(cos(ang)*(par[0]+par[2]*tmp_z[3])+sin(ang)*(par[1]+par[3]*tmp_z[3])
	 -xx[3],2.)
    
    +pow(par[0]+par[2]*tmp_z[4]-xx[4],2.)
    //X'-X                                                                          
    +pow(par[0]+par[2]*tmp_z[5]-xx[5],2.);

  result = ans;

}
*/

