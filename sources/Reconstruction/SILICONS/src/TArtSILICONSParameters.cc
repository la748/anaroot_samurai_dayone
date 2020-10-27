#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "TArtSILICONSParameters.hh"
#include "TArtSILICONSPara.hh"

#include <TROOT.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>

TArtSILICONSParameters* TArtSILICONSParameters::fSILICONSParameters = 0;

//__________________________________________________________
TArtSILICONSParameters::TArtSILICONSParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating silicons setup...");

  listOfPara = new TList();

  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtSILICONSParameters::~TArtSILICONSParameters() 
{
  itr = pmap.begin();
  std::vector<TArtSILICONSPara*> pvec;
  while(itr != itrEnd){
    std::vector<TArtSILICONSPara*>::iterator it_pvec = pvec.begin();
    if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
      pvec.push_back(itr->second);
      delete itr->second;
      itr->second = 0;
    }
    ++itr;
  }
  delete listOfPara;

  fSILICONSParameters = 0;
}

//__________________________________________________________
TArtSILICONSParameters* TArtSILICONSParameters::Instance(const char* name, const char* title)
{
  if(!fSILICONSParameters) fSILICONSParameters = 
			   new TArtSILICONSParameters(name, title);
  return fSILICONSParameters;
}    

//__________________________________________________________
void TArtSILICONSParameters::Delete()
{
  delete fSILICONSParameters;
  fSILICONSParameters = 0;
}

//__________________________________________________________
Bool_t TArtSILICONSParameters::LoadParameter(char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser *domParser = new TDOMParser();
  domParser->SetValidate(false);
  Int_t parsecode = domParser->ParseFile(xmlfile);
  if (parsecode < 0) {
    cerr << domParser->GetParseCodeMessage(parsecode) << endl;
    return false;
  }
  TXMLNode * node = domParser->GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  itrEnd = pmap.end();
  delete domParser;
  return true;
}

//__________________________________________________________
void TArtSILICONSParameters::ParseParaList(TXMLNode *node) {
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "SILICONS") == 0)
        listOfPara->Add(ParseSILICONSPara(node->GetChildren()));
    }
  }
  return;
}

//__________________________________________________________
TArtSILICONSPara *TArtSILICONSParameters::ParseSILICONSPara(TXMLNode *node) {

  

  TString name;
  UShort_t  id = 0;
  UShort_t  fpl = 0;
  UShort_t  layer = 0;
  UShort_t  strip = 0;
  TArtSILICONSPara::DetDirection direction ;
  Double_t  x = 0, y=0, z=0;
  Double_t  tcal = 0;
  Double_t  toff = 0;
  Double_t  qcal = 0;
  Double_t  qped = 0;
  Double_t  angle = 0, angle_degree=0;
  Int_t tdc_geo, tdc_ch, adc_geo, adc_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0) 
	layer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "strip") == 0) 
	strip = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "angle") == 0) {
	angle_degree = atof(node->GetText());
	angle = TMath::DegToRad()*angle_degree;
      }
      if (strcmp(node->GetNodeName(), "direction") == 0) { // kept for backward compatibility
	if (strcmp(node->GetText(), "H") == 0) direction = direction.Horiz;
	if (strcmp(node->GetText(), "V") == 0) direction = direction.Vert;
      }
      if (strcmp(node->GetNodeName(), "x") == 0)
	x = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "y") == 0)
	y = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "z") == 0)
	z = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "toff") == 0)
	toff = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tcal") == 0)
	tcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped") == 0)
	qped = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal") == 0)
	qcal = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "adc_geo") == 0) 
	adc_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "adc_ch") == 0) 
	adc_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_geo") == 0) 
	tdc_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_ch") == 0) 
	tdc_ch = atoi(node->GetText());
    }
  }
  
  Double_t detpos[3] = {x,y,z}; 
  TArtCore::Info(__FILE__,"Reading database for %s SILICONS",name.Data());

  TArtSILICONSPara *para = new TArtSILICONSPara(id,name, fpl, layer, 
						strip, direction, angle, detpos, 
						tcal,toff, qcal, qped);
  //  para->SetName((name+"Para").Data());
  para->SetMap(tdc_geo, tdc_ch, adc_geo, adc_ch);

  pmap.insert(std::pair<TArtRIDFMap, TArtSILICONSPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  pmap.insert(std::pair<TArtRIDFMap, TArtSILICONSPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtSILICONSParameters::PrintListOfPara(){
  TIter next(listOfPara);
  while (TArtSILICONSPara *p = (TArtSILICONSPara *)next()) std::cout << *p;
}
TArtSILICONSPara * TArtSILICONSParameters::GetSILICONSPara(TArtRIDFMap *rmap){

  itr = pmap.find(*rmap);
  if(itr != itrEnd){
    return (TArtSILICONSPara*)itr->second;
  }
  return NULL;

}
