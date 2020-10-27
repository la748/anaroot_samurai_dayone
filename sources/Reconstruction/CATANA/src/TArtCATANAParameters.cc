#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "TArtCATANAParameters.hh"
#include "TArtCATANACsIPara.hh"

#include <TROOT.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>

TArtCATANAParameters* TArtCATANAParameters::fCATANAParameters = 0;

//__________________________________________________________
  TArtCATANAParameters::TArtCATANAParameters(const char* name, const char* title)
: TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating catana setup...");

  listOfCsIPara = new TList();
  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtCATANAParameters::~TArtCATANAParameters() 
{
  itr = pmap.begin();
  std::vector<TArtCATANACsIPara*> pvec;
  while(itr != itrEnd){
    std::vector<TArtCATANACsIPara*>::iterator it_pvec = pvec.begin();
    if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
      pvec.push_back(itr->second);
      delete itr->second;
      itr->second = 0;
    }
    ++itr;
  }
  delete listOfCsIPara;

  // Delete the list of para

  fCATANAParameters = 0;
}

//__________________________________________________________
TArtCATANAParameters* TArtCATANAParameters::Instance(const char* name, const char* title)
{
  if(!fCATANAParameters) fCATANAParameters = 
    new TArtCATANAParameters(name, title);
  return fCATANAParameters;
}    

//__________________________________________________________
void TArtCATANAParameters::Delete()
{
  delete fCATANAParameters;
  fCATANAParameters = 0;
}

//__________________________________________________________
Bool_t TArtCATANAParameters::LoadParameter(char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser *domParser = new TDOMParser();
  domParser->SetValidate(false);
  Int_t parsecode = domParser->ParseFile(xmlfile);
  if (parsecode < 0) {
    std::cerr << domParser->GetParseCodeMessage(parsecode) << std::endl;
    return false;
  }
  TXMLNode * node = domParser->GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  itrEnd = pmap.end();
  delete domParser;
  return true;
}

//__________________________________________________________
void TArtCATANAParameters::ParseParaList(TXMLNode *node) {
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "CATANA") == 0)
        listOfCsIPara->Add(ParseCATANACsIPara(node->GetChildren()));
    }
  }

  return;
}

//__________________________________________________________
TArtCATANACsIPara *TArtCATANAParameters::ParseCATANACsIPara(TXMLNode *node) {

  TString name;
  Int_t     id = -1;
  Int_t     ch = -1;
  Int_t     fpl = -1;
  Int_t     frame = -1;
  Int_t     layer = -1;
  Double_t  x = 0, y=0, z=0;
  Double_t  tcal = 0;
  Double_t  toff = 0;
  Double_t  qcal = 0;
  Double_t  qped = 0;
  Int_t tdc_geo, tdc_ch, adc_geo, adc_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "Id") == 0) 
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch") == 0)
        ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "frame") == 0) 
        frame = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0) 
        layer = atoi(node->GetText());
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
  //  TArtCore::Info(__FILE__,"Reading database for %s CATANA",name.Data());
  //  TArtCATANANaIPara * para = new TArtCATANANaIPara(id, name, fpl, layer, tcal, toff, qcal, qped, theta);
  TArtCATANACsIPara *para = new TArtCATANACsIPara(id,ch,name, fpl, frame,layer, detpos, 
      tcal,toff, qcal, qped);
  //  para->SetName((name+"Para").Data());
  para->SetMap(tdc_geo, tdc_ch, adc_geo, adc_ch);

  pmap.insert(std::pair<TArtRIDFMap, TArtCATANACsIPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  pmap.insert(std::pair<TArtRIDFMap, TArtCATANACsIPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

//__________________________________________________________
void TArtCATANAParameters::PrintListOfCsIPara(){
  TIter next(listOfCsIPara);
  while (TArtCATANACsIPara *p = (TArtCATANACsIPara *)next()) std::cout << *p;
}
TArtCATANACsIPara * TArtCATANAParameters::GetCATANACsIPara(TArtRIDFMap *rmap){

  itr = pmap.find(*rmap);
  if(itr != itrEnd){
    return (TArtCATANACsIPara*)itr->second;
  }
  return NULL;

}
//__________________________________________________________
