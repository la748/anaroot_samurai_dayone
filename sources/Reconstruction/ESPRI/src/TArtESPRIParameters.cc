#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"
#include "TArtESPRIParameters.hh"
#include "TArtRNaIPara.hh"
#include "TArtTDCHitPara.hh"
#include "TArtFADCPara.hh"
#include "TArtPlasPara.hh"

#include <TList.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

TArtESPRIParameters* TArtESPRIParameters::fESPRIParameters = 0;

//__________________________________________________________
TArtESPRIParameters::TArtESPRIParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating ESPRI setup...");

  listOfRNaIPara = new TList();
  listOfTDCHitPara = new TList();
  listOfFADCPara = new TList();
  listOfPlasPara = new TList();

  listOfRNaIPara->SetOwner();//only SAMIRAI?
  listOfTDCHitPara->SetOwner();
  listOfFADCPara->SetOwner();
  listOfPlasPara->SetOwner();

  //fStoreManager = TArtStoreManager::Instance();
  //fStoreManager->AddParameters(this);
  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);

}

//__________________________________________________________
TArtESPRIParameters::~TArtESPRIParameters() 
{

  delete listOfRNaIPara;
  delete listOfTDCHitPara;
  delete listOfFADCPara;
  delete listOfPlasPara;

  fESPRIParameters = 0;
}

//__________________________________________________________
TArtESPRIParameters* TArtESPRIParameters::Instance(const char* name, const char* title)
{
  if(!fESPRIParameters) fESPRIParameters = new TArtESPRIParameters(name, title);
  return fESPRIParameters;
}    

//__________________________________________________________
void TArtESPRIParameters::Delete()
{
  delete fESPRIParameters;
  fESPRIParameters = 0;  
}

//__________________________________________________________
Bool_t TArtESPRIParameters::LoadParameter(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser domParser;
  domParser.SetValidate(false);
  Int_t parsecode = domParser.ParseFile(xmlfile);
  if(parsecode < 0){
    std::cerr << domParser.GetParseCodeMessage(parsecode) << std::endl;
    return false;
  }
  TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  return true;
}

//__________________________________________________________
void TArtESPRIParameters::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node
    
    if(strcmp(node->GetNodeName(), "ESPRIRNAI") == 0){
      listOfRNaIPara->Add(ParseRNaIPara(node->GetChildren()));
    }
    else if(strcmp(node->GetNodeName(), "ESPRITDC") == 0){
      listOfTDCHitPara->Add(ParseTDCHitPara(node->GetChildren(),&tdc_pmap));
    }
    else if(strcmp(node->GetNodeName(), "ESPRIFADC") == 0){
      listOfFADCPara->Add(ParseFADCPara(node->GetChildren(),&fadc_pmap));
    }
    else if(strcmp(node->GetNodeName(), "ESPRIPLAS") == 0){
      listOfPlasPara->Add(ParsePlasPara(node->GetChildren(),&plas_pmap));
    }
  }
}

//
// parameter parser for RNaI
//
//__________________________________________________________
TArtRNaIPara *TArtESPRIParameters::ParseRNaIPara(TXMLNode *node)
{

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     layer = -1;
  Double_t  tcal = 0;
  Double_t  toff = 0;
  Double_t  qcal = 0;
  Double_t  qped = 0;
  Double_t  theta = 0;
  Int_t tdc_geo, tdc_ch, adc_geo, adc_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node        
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      else if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "layer") == 0)
        layer = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "tcal") == 0)
        tcal = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "toff") == 0)
        toff = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "qcal") == 0)
        qcal = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "qped") == 0)
        qped = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "tdc_geo") == 0)
        tdc_geo = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "tdc_ch") == 0)
        tdc_ch = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "adc_geo") == 0)
        adc_geo = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "adc_ch") == 0)
        adc_ch = atoi(node->GetText());
      else
	TArtCore::Warning(__FILE__, "Unknown parameter %s", node->GetNodeName());
    }

  }

  //TArtCore::Info(__FILE__,"Reading database for %s RNaI",name.Data());
  TArtRNaIPara * para = new TArtRNaIPara(id, name, fpl, layer, tcal, toff, qcal, qped);
  para->SetMap(tdc_geo, tdc_ch, adc_geo, adc_ch);
  //TArtCore::Info(__FILE__,"%d %d %d %d %s on RNaI",tdc_geo, tdc_ch, adc_geo, adc_ch,name.Data());
  rnai_pmap.insert(std::pair<TArtRIDFMap, TArtRNaIPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  rnai_pmap.insert(std::pair<TArtRIDFMap, TArtRNaIPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));
  
  return para;

}

void TArtESPRIParameters::PrintListOfRNaIPara(){
  TIter next(listOfRNaIPara);
  while (TArtRNaIPara *p = (TArtRNaIPara *)next()) std::cout << *p;
}

const TArtRNaIPara* TArtESPRIParameters::FindRNaIPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtRNaIPara *>::const_iterator itr = rnai_pmap.find(*rmap);
  if(itr != rnai_pmap.end()) return (TArtRNaIPara*)itr->second;
  else return 0;
}

//
// parameter parser for Plas
//
//__________________________________________________________
TArtPlasPara *TArtESPRIParameters::ParsePlasPara(TXMLNode *node, std::map<TArtRIDFMap, TArtPlasPara *> * map)
{

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     layer = -1;
  Int_t     id_plane = -1;
  Int_t     channel = -1;
  Double_t  qcal = 0;
  Double_t  qped = 0;
  Int_t det, geo, ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node        
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      else if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "layer") == 0)
        layer = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "id_plane") == 0)
        id_plane = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "channel") == 0)
        channel = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "qcal") == 0)
        qcal = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "qped") == 0)
        qped = (Double_t)atof(node->GetText());
      else if (strcmp(node->GetNodeName(), "det") == 0)
        det = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "geo") == 0)
        geo = atoi(node->GetText());
      else if (strcmp(node->GetNodeName(), "ch") == 0)
        ch = atoi(node->GetText());
      else
	TArtCore::Warning(__FILE__, "Unknown parameter %s", node->GetNodeName());
    }

  }

  TArtPlasPara * para = new TArtPlasPara(id, name, fpl, layer, id_plane, channel, qcal, qped);
  para->SetMap(det, geo, ch);
  
  map->insert(std::pair<TArtRIDFMap, TArtPlasPara *>(*((TArtRIDFMap *)para->GetPlasMap()), para));
  
  return para;

}

void TArtESPRIParameters::PrintListOfPlasPara(){
  TIter next(listOfPlasPara);
  while (TArtPlasPara *p = (TArtPlasPara *)next()) std::cout << *p;
}

const TArtPlasPara* TArtESPRIParameters::FindPlasPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtPlasPara *>::const_iterator itr = plas_pmap.find(*rmap);
  if(itr != plas_pmap.end()) return (TArtPlasPara*)itr->second;
  else return 0;
}

//
// parameter parser for ESPRI-DC
//
//__________________________________________________________

TArtTDCHitPara *TArtESPRIParameters::ParseTDCHitPara(TXMLNode *node, std::map<TArtRIDFMap, TArtTDCHitPara *> * map)
{
  Int_t     id=-1;
  TString   name;
  Int_t     fpl=-1;

  Int_t layer=-1;
  Int_t id_plane=-1;
  Int_t wireid=-9999;
  Double_t wireang=-9999; // wire position in a layer
  Double_t wirez=-9999; // wire z position
  Int_t toffset=-9999; // wire z position

  // calibration value for TDC -> timing
  Double_t  tzero_offset=0;

  Int_t det, geo, ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0)
        layer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "id_plane") == 0)
        id_plane = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "wireid") == 0)
        wireid = (Int_t)atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "wireang") == 0)
        wireang = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "wirez") == 0)
        wirez = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "toffset") == 0)
        toffset = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "det") == 0)
        det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "geo") == 0)
        geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch") == 0)
        ch = atoi(node->GetText());

    }
  }

  //  TArtCore::Info(__FILE__,"Reading database for %s DC Hit",name.Data());
  //TArtTDCHitPara * para = new TArtTDCHitPara(id, name, fpl, layer, id_plane, wireid, wireang, wirez, tzero_offset);
  TArtTDCHitPara * para = new TArtTDCHitPara(id, name, fpl, layer, id_plane, wireid, wireang, wirez, toffset);
  para->SetMap(det, geo, ch);

  map->insert(std::pair<TArtRIDFMap, TArtTDCHitPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtESPRIParameters::PrintListOfTDCHitPara(){
  TIter next(listOfTDCHitPara);
  while (TArtTDCHitPara *p = (TArtTDCHitPara *)next()) std::cout << *p;
}

const TArtTDCHitPara* TArtESPRIParameters::FindTDCHitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtTDCHitPara *>::const_iterator itr = tdc_pmap.find(*rmap);
  if(itr != tdc_pmap.end()) return (TArtTDCHitPara*)itr->second;
  else return 0;
}

//_____________________________________________________________________

//
// parameter parser for ESPRI-FADC
//
//__________________________________________________________

TArtFADCPara *TArtESPRIParameters::ParseFADCPara(TXMLNode *node, std::map<TArtRIDFMap, TArtFADCPara *> * map)
{
  Int_t     id=-1;
  TString   name;
  Int_t     fpl=-1;

  Int_t det, geo, ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());

      if (strcmp(node->GetNodeName(), "det") == 0)
        det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "geo") == 0)
        geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch") == 0)
        ch = atoi(node->GetText());

    }
  }

  TArtFADCPara * para = new TArtFADCPara(id, name, fpl);
  para->SetMap(det, geo, ch);

  map->insert(std::pair<TArtRIDFMap, TArtFADCPara *>(*((TArtRIDFMap *)para->GetFADCMap()), para));

  return para;

}

void TArtESPRIParameters::PrintListOfFADCPara(){
  TIter next(listOfFADCPara);
  while (TArtFADCPara *p = (TArtFADCPara *)next()) std::cout << *p;
}

const TArtFADCPara* TArtESPRIParameters::FindFADCPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtFADCPara *>::const_iterator itr = fadc_pmap.find(*rmap);
  if(itr != fadc_pmap.end()) return (TArtFADCPara*)itr->second;
  else return 0;
}

//_____________________________________________________________________

