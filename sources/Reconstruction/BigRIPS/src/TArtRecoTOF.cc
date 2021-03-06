#include "TArtRecoTOF.hh" 
#include "TArtPlastic.hh"
#include "TArtPlasticPara.hh"
#include "TArtFocalPlanePara.hh"
#include "TArtTOF.hh"
#include "TArtTOFPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TList.h>
#include <TString.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtRecoTOF::TArtRecoTOF(const TString outdataname)
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the TOF objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fTOFArray = new TClonesArray("TArtTOF",10);
  fTOFArray->SetName("BigRIPSTOF");
  fTOFArray->SetOwner();
  sman->AddDataContainer(fTOFArray);

  
  // define the tof which was defined by user macro
  TList *toflist = setup->GetListOfTOFPara();
  for(int i=0;i<toflist->GetEntries();i++){
    TArtTOFPara *p = (TArtTOFPara *)toflist->At(i);
    DefineNewTOF((char *)((TString *)p->GetUpstreamPlName()->Data()),
		 (char *)((TString *)p->GetDownstreamPlName1()->Data()),
		 (char *)((TString *)p->GetDownstreamPlName2()->Data()),
		 p->GetOffset(),p->GetMiddleFpl()); //added 2nd downstream 24/02/21 and changed GetDownstreamPlName()->GetDownstreamPlName1()
  }

  /*
  // define the tof which was defined by user macro
  TList *toflist = setup->GetListOfTOFPara();
  for(int i=0;i<toflist->GetEntries();i++){
    TArtTOFPara *p = (TArtTOFPara *)toflist->At(i);
    DefineNewTOF((char *)((TString *)p->GetUpstreamPlName()->Data()),
		 (char *)((TString *)p->GetDownstreamPlName()->Data()),
		 p->GetOffset(),p->GetMiddleFpl());
		 }*/
  
}

//__________________________________________________________
TArtRecoTOF::~TArtRecoTOF()  {
  ClearData();
}

//__________________________________________________________
/*TArtTOF * TArtRecoTOF::DefineNewTOF(char *uplname, char *dplname, Double_t offset, Int_t mfpl){

  char name[64];
  Int_t ntof=GetNumTOF();
  new ((*fTOFArray)[ntof]) TArtTOF();
  TArtTOF * tof = (TArtTOF *)fTOFArray->At(ntof);

  sprintf(name,"TOF%sto%s",uplname,dplname);
  tof->SetDetectorName(name);
  tof->SetUpstreamPlaName(uplname);
  tof->SetDownstreamPlaName(dplname);
  tof->SetTimeOffset(offset);

  TString uplstr = uplname;
  TArtPlasticPara * upara = setup->FindPlasticPara(&uplstr);
  if(!upara){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",uplname);
    throw;
  }
  Int_t ufpl = upara->GetFpl();
  Double_t ufpl_zpos = setup->FindFocalPlanePara(ufpl)->GetStdZpos();
  Double_t upara_zoffset = upara->GetZoffset();
  tof->SetUpstreamPlaFpl(ufpl);

  TString dplstr = dplname;
  TArtPlasticPara * dpara = setup->FindPlasticPara(&dplstr);
  if(!dpara){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",dplname);
    throw;
  }

  // middle focal plane information used for F3-F5-F7 Brho analysis
  Double_t mfpl_zpos = 0;
  TArtFocalPlanePara *fpl_para = setup->FindFocalPlanePara(mfpl);
  if(fpl_para) mfpl_zpos = fpl_para->GetStdZpos();

  Int_t dfpl = dpara->GetFpl();
  Double_t dfpl_zpos = setup->FindFocalPlanePara(dfpl)->GetStdZpos();
  Double_t dpara_zoffset = dpara->GetZoffset();
  tof->SetDownstreamPlaFpl(dfpl);

  Double_t toflength = dfpl_zpos+dpara_zoffset - (ufpl_zpos+upara_zoffset);
  Double_t tofulength = mfpl_zpos == 0 ? 0 : mfpl_zpos - (ufpl_zpos+upara_zoffset);
  Double_t tofdlength = mfpl_zpos == 0 ? 0 : dfpl_zpos+dpara_zoffset - mfpl_zpos;
  tof->SetLength(toflength);
  tof->SetUpStreamLength(tofulength);
  tof->SetDownStreamLength(tofdlength);
  TArtCore::Info(__FILE__,"define %s, toflength=%f",name, toflength);

  pla_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  Int_t num_pla = pla_array->GetEntries();

  TArtPlastic * upstream_pla = NULL;
  TArtPlastic * downstream_pla = NULL;
  
  /////////////////////////////////////////////////////////////////////
  // find fpl objects

  for(Int_t j=0;j<num_pla;j++){
    TArtPlastic *tmp_pla = (TArtPlastic *)pla_array->At(j);
    TString planame = *(tmp_pla->GetDetectorName());

    if(planame ==  *(tof->GetUpstreamPlaName())){
      if(NULL == upstream_pla)
	upstream_pla = tmp_pla; 
      else
	TArtCore::Info(__FILE__,"found 2nd u-plastic: %s",tof->GetUpstreamPlaName()->Data());
    }
    if(planame ==  *(tof->GetDownstreamPlaName())){
      if(NULL == downstream_pla)
	downstream_pla = tmp_pla; 
      else
	TArtCore::Info(__FILE__,"found 2nd d-plastic: %s",tof->GetDownstreamPlaName()->Data());
    }
  }

  if(NULL != upstream_pla && NULL != downstream_pla){
    fUpstreamPlaArrayBuffer.push_back(upstream_pla);
    fDownstreamPlaArrayBuffer.push_back(downstream_pla);
  }
  if(NULL == upstream_pla)
    TArtCore::Error(__FILE__,"no upstream pla: %s",tof->GetUpstreamPlaName()->Data(),num_pla);
  if(NULL == downstream_pla)
    TArtCore::Error(__FILE__,"no downstream pla: %s",tof->GetDownstreamPlaName()->Data());

  return tof;

  }*/

TArtTOF * TArtRecoTOF::DefineNewTOF(char *uplname, char *dplname1, char *dplname2, Double_t offset, Int_t mfpl){

  char name[64];
  Int_t ntof=GetNumTOF();
  new ((*fTOFArray)[ntof]) TArtTOF();
  TArtTOF * tof = (TArtTOF *)fTOFArray->At(ntof);

  sprintf(name,"TOF%sto%s",uplname,dplname1,dplname2);
  tof->SetDetectorName(name);
  tof->SetUpstreamPlaName(uplname);
  tof->SetDownstreamPlaName1(dplname1);
  tof->SetDownstreamPlaName2(dplname2);
  tof->SetTimeOffset(offset);

  TString uplstr = uplname;
  TArtPlasticPara * upara = setup->FindPlasticPara(&uplstr);
  if(!upara){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",uplname);
    throw;
  }
  Int_t ufpl = upara->GetFpl();
  Double_t ufpl_zpos = setup->FindFocalPlanePara(ufpl)->GetStdZpos();
  Double_t upara_zoffset = upara->GetZoffset();
  tof->SetUpstreamPlaFpl(ufpl);

  TString dplstr1 = dplname1;
  TArtPlasticPara * dpara1 = setup->FindPlasticPara(&dplstr1);
  if(!dpara1){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",dplname1);
    throw;
  }

  // middle focal plane information used for F3-F5-F7 Brho analysis
  Double_t mfpl_zpos = 0;
  TArtFocalPlanePara *fpl_para = setup->FindFocalPlanePara(mfpl);
  if(fpl_para) mfpl_zpos = fpl_para->GetStdZpos();

  Int_t dfpl1 = dpara1->GetFpl();
  Double_t dfpl1_zpos = setup->FindFocalPlanePara(dfpl1)->GetStdZpos();
  Double_t dpara1_zoffset = dpara1->GetZoffset();
  tof->SetDownstreamPlaFpl1(dfpl1);

  TString dplstr2 = dplname2;
  TArtPlasticPara * dpara2 = setup->FindPlasticPara(&dplstr2);
  if(!dpara2){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",dplname2);
    throw;
  }

  Int_t dfpl2 = dpara2->GetFpl();
  Double_t dfpl2_zpos = setup->FindFocalPlanePara(dfpl2)->GetStdZpos();
  Double_t dpara2_zoffset = dpara2->GetZoffset();
  tof->SetDownstreamPlaFpl2(dfpl2);

  Double_t toflength = (((dfpl1_zpos+dpara1_zoffset) + (dfpl2_zpos+dpara2_zoffset)) / 2) - (ufpl_zpos+upara_zoffset);
  Double_t tofulength = mfpl_zpos == 0 ? 0 : mfpl_zpos - (ufpl_zpos+upara_zoffset);
  Double_t tofdlength = mfpl_zpos == 0 ? 0 : (((dfpl1_zpos+dpara1_zoffset) + (dfpl2_zpos+dpara2_zoffset)) / 2) - mfpl_zpos;
  tof->SetLength(toflength);
  tof->SetUpStreamLength(tofulength);
  tof->SetDownStreamLength(tofdlength);
  TArtCore::Info(__FILE__,"define %s, toflength=%f",name, toflength);

  pla_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  Int_t num_pla = pla_array->GetEntries();

  TArtPlastic * upstream_pla = NULL;
  TArtPlastic * downstream_pla1 = NULL;
  TArtPlastic * downstream_pla2 = NULL;

  /////////////////////////////////////////////////////////////////////
  // find fpl objects

  for(Int_t j=0;j<num_pla;j++){
    TArtPlastic *tmp_pla = (TArtPlastic *)pla_array->At(j);
    TString planame = *(tmp_pla->GetDetectorName());

    if(planame ==  *(tof->GetUpstreamPlaName())){
      if(NULL == upstream_pla)
	upstream_pla = tmp_pla;
      else
	TArtCore::Info(__FILE__,"found 2nd u-plastic: %s",tof->GetUpstreamPlaName()->Data());
    }
    if(planame ==  *(tof->GetDownstreamPlaName1())){
      if(NULL == downstream_pla1)
	downstream_pla1 = tmp_pla;
      else
	TArtCore::Info(__FILE__,"found 2nd d-plastic: %s",tof->GetDownstreamPlaName1()->Data());
    }
    if(planame ==  *(tof->GetDownstreamPlaName2())){
      if(NULL == downstream_pla2)
	downstream_pla2 = tmp_pla;
      else
	TArtCore::Info(__FILE__,"found 3rd d-plastic: %s",tof->GetDownstreamPlaName2()->Data());
    }
  }

  if(NULL != upstream_pla && NULL != downstream_pla1 && NULL != downstream_pla2){
    fUpstreamPlaArrayBuffer.push_back(upstream_pla);
    fDownstreamPlaArrayBuffer1.push_back(downstream_pla1);
    fDownstreamPlaArrayBuffer2.push_back(downstream_pla2);
  }
  if(NULL == upstream_pla)
    TArtCore::Error(__FILE__,"no upstream pla: %s",tof->GetUpstreamPlaName()->Data(),num_pla);
  if(NULL == downstream_pla1)
    TArtCore::Error(__FILE__,"no downstream1 pla: %s",tof->GetDownstreamPlaName1()->Data());
  if(NULL == downstream_pla2)
    TArtCore::Error(__FILE__,"no downstream2 pla: %s",tof->GetDownstreamPlaName2()->Data());

    return tof;

}

//__________________________________________________________
void TArtRecoTOF::ClearData()   {
  for(Int_t i=0;i<GetNumTOF();i++)
    ((TArtTOF*)fTOFArray->At(i))->SetTOF(0);
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoTOF::ReconstructData()   { // call after the raw data are loaded

  for(Int_t i=0;i<GetNumTOF();i++){

    TArtTOF *tof = (TArtTOF *)fTOFArray->At(i);

    /////////////////////////////////////////////////////////////////////
    // calculate time of fright

    //tof->SetTOF(downstream_pla->GetTimeSlew() - upstream_pla->GetTimeSlew() + tof->GetTimeOffset());
    //tof->SetTOF(fDownstreamPlaArrayBuffer[i]->GetTimeLSlew() - fUpstreamPlaArrayBuffer[i]->GetTimeLSlew() + tof->GetTimeOffset());
    //tof->SetTOF(fDownstreamPlaArrayBuffer[i]->GetTimeSlewPos() - fUpstreamPlaArrayBuffer[i]->GetTimeSlewPos() + tof->GetTimeOffset()); //commented for trial
    //tof->SetTOF(((fDownstreamPlaArrayBuffer1[i]->GetTimeSlewPos() + fDownstreamPlaArrayBuffer2[i]->GetTimeSlewPos())/2) - fUpstreamPlaArrayBuffer[i]->GetTimeSlewPos() + tof->GetTimeOffset());
    tof->SetTOF(((fDownstreamPlaArrayBuffer1[i]->GetTimeLSlew() + fDownstreamPlaArrayBuffer2[i]->GetTimeLSlew())/2) - fUpstreamPlaArrayBuffer[i]->GetTimeLSlew() + tof->GetTimeOffset());

  } 

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtTOF * TArtRecoTOF::GetTOF(Int_t i) {
  return (TArtTOF *)fTOFArray->At(i);
}
//__________________________________________________________
Int_t TArtRecoTOF::GetNumTOF() {
  return fTOFArray->GetEntries();
}
//__________________________original________________________________
/*TArtTOF * TArtRecoTOF::FindTOF(char * u_planame, char * d_planame){
  for(Int_t i=0;i<GetNumTOF();i++)
    if(((TArtTOF*)fTOFArray->At(i))->GetUpstreamPlaName()->CompareTo(u_planame) == 0 && ((TArtTOF*)fTOFArray->At(i))->GetDownstreamPlaName()->CompareTo(d_planame) == 0)
      return (TArtTOF*)fTOFArray->At(i);
  return NULL;
  }*/
//_________________________new_________________________________
TArtTOF * TArtRecoTOF::FindTOF(char * u_planame, char * d_planame1, char * d_planame2){
  for(Int_t i=0;i<GetNumTOF();i++)
    if(((TArtTOF*)fTOFArray->At(i))->GetUpstreamPlaName()->CompareTo(u_planame) == 0 && ((TArtTOF*)fTOFArray->At(i))->GetDownstreamPlaName1()->CompareTo(d_planame1) == 0 && ((TArtTOF*)fTOFArray->At(i))->GetDownstreamPlaName2()->CompareTo(d_planame2) == 0)
      return (TArtTOF*)fTOFArray->At(i);
  return NULL;
}
