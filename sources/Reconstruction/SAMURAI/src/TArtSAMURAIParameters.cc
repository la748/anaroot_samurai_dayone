
#include "TArtSAMURAIParameters.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtHODPlaPara.hh"
#include "TArtRPTOFPlaPara.hh"
#include "TArtTEDCsIPara.hh"
#include "TArtDCHitPara.hh"

#include "TArtNEBULAPlaPara.hh"
#include "TArtNEBULAHPCPara.hh"
#include "TArtNeuLANDPlaPara.hh"

#include "TArtHIMEPlaPara.hh"

#include "TArtNINJAPlaPara.hh"

#include <TList.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

TArtSAMURAIParameters* TArtSAMURAIParameters::fSAMURAIParameters = 0;

//__________________________________________________________
  TArtSAMURAIParameters::TArtSAMURAIParameters(const char* name, const char* title)
: TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating SAMURAI setup...");
  listOfHODPlaPara = new TList();
  listOfRPTOFPlaPara = new TList();
  listOfTEDCsIPara = new TList();
  listOfBPCHitPara = new TList();
  listOfBDC1HitPara = new TList();
  listOfBDC2HitPara = new TList();
  listOfFDC0HitPara = new TList();
  listOfFDC1HitPara = new TList();
  listOfFDC2HitPara = new TList();
  listOfPDCHitPara = new TList();
  listOfRPDCHitPara = new TList();
  listOfS1DC1HitPara = new TList();
  listOfS1DC2HitPara = new TList();

  listOfHODPlaPara->SetOwner();
  listOfRPTOFPlaPara->SetOwner();
  listOfTEDCsIPara->SetOwner();
  listOfBPCHitPara->SetOwner(); 
  listOfBDC1HitPara->SetOwner();
  listOfBDC2HitPara->SetOwner();
  listOfFDC0HitPara->SetOwner();
  listOfPDCHitPara->SetOwner();
  listOfFDC1HitPara->SetOwner();
  listOfFDC2HitPara->SetOwner();
  listOfRPDCHitPara->SetOwner();
  listOfS1DC1HitPara->SetOwner();
  listOfS1DC2HitPara->SetOwner();

  fNEBULAPlaUParaMap = new TArtNEBULAPlaParaMap;
  fNEBULAPlaDParaMap = new TArtNEBULAPlaParaMap;
  fNEBULAHPCParaMap  = new TArtNEBULAHPCParaMap;
  fNeuLANDPlaParaMap = new TArtNeuLANDPlaParaMap;
  fTacquilaParaMap   = new TArtTacquilaParaMap;
  fHIMEPlaUParaMap = new TArtHIMEPlaParaMap;
  fHIMEPlaDParaMap = new TArtHIMEPlaParaMap;
  fNeuLANDVETOPlaUParaMap = new TArtNEBULAPlaParaMap;
  fNeuLANDVETOPlaDParaMap = new TArtNEBULAPlaParaMap;

  fNINJAPla0ParaMap = new TArtNINJAPlaParaMap;
  fNINJAPla1ParaMap = new TArtNINJAPlaParaMap;

  fStoreManager = TArtStoreManager::Instance();
  fStoreManager->AddParameters(this);

}

//__________________________________________________________
TArtSAMURAIParameters::~TArtSAMURAIParameters() 
{
  {
    TArtNEBULAPlaParaMap::iterator itr = fNEBULAPlaUParaMap->begin();
    std::vector<const TArtNEBULAPlaPara*> pvec;
    while(itr != fNEBULAPlaUParaMap->end()){
      std::vector<const TArtNEBULAPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtNEBULAPlaParaMap::iterator itr = fNEBULAPlaDParaMap->begin();
    std::vector<const TArtNEBULAPlaPara*> pvec;
    while(itr != fNEBULAPlaDParaMap->end()){
      std::vector<const TArtNEBULAPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtNEBULAHPCParaMap::iterator itr = fNEBULAHPCParaMap->begin();
    std::vector<const TArtNEBULAHPCPara*> pvec;
    while(itr != fNEBULAHPCParaMap->end()){
      std::vector<const TArtNEBULAHPCPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtNeuLANDPlaParaMap::iterator itr = fNeuLANDPlaParaMap->begin();
    std::vector<const TArtNeuLANDPlaPara*> pvec;
    while(itr != fNeuLANDPlaParaMap->end()){
      std::vector<const TArtNeuLANDPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtTacquilaParaMap::iterator itr = fTacquilaParaMap->begin();
    std::vector<const TArtTacquilaPara*> pvec;
    while(itr != fTacquilaParaMap->end()){
      std::vector<const TArtTacquilaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtNEBULAPlaParaMap::iterator itr = fNeuLANDVETOPlaDParaMap->begin();
    std::vector<const TArtNEBULAPlaPara*> pvec;
    while(itr != fNeuLANDVETOPlaDParaMap->end()){
      std::vector<const TArtNEBULAPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtHIMEPlaParaMap::iterator itr = fHIMEPlaUParaMap->begin();
    std::vector<const TArtHIMEPlaPara*> pvec;
    while(itr != fHIMEPlaUParaMap->end()){
      std::vector<const TArtHIMEPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }
  {
    TArtHIMEPlaParaMap::iterator itr = fHIMEPlaDParaMap->begin();
    std::vector<const TArtHIMEPlaPara*> pvec;
    while(itr != fHIMEPlaDParaMap->end()){
      std::vector<const TArtHIMEPlaPara*>::iterator it_pvec = pvec.begin();
      if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
        pvec.push_back(itr->second);
        delete itr->second;
        itr->second = 0;
      }
      ++itr;
    }
  }


  delete fNEBULAPlaUParaMap;
  delete fNEBULAPlaDParaMap;
  delete fNEBULAHPCParaMap;
  delete fNeuLANDPlaParaMap;
  delete fTacquilaParaMap;
  delete fNeuLANDVETOPlaUParaMap;
  delete fNeuLANDVETOPlaDParaMap;
  delete fHIMEPlaUParaMap;
  delete fHIMEPlaDParaMap;
  delete fNINJAPla0ParaMap;
  delete fNINJAPla1ParaMap;

  delete listOfHODPlaPara;
  delete listOfRPTOFPlaPara;
  delete listOfTEDCsIPara;
  delete listOfBPCHitPara;
  delete listOfBDC1HitPara;
  delete listOfBDC2HitPara;
  delete listOfPDCHitPara;
  delete listOfFDC0HitPara;
  delete listOfFDC1HitPara;
  delete listOfFDC2HitPara;
  delete listOfRPDCHitPara;
  delete listOfS1DC1HitPara;
  delete listOfS1DC2HitPara;

  fSAMURAIParameters = 0;
}

//__________________________________________________________
TArtSAMURAIParameters* TArtSAMURAIParameters::Instance(const char* name, const char* title)
{
  if(!fSAMURAIParameters) fSAMURAIParameters = new TArtSAMURAIParameters(name, title);
  return fSAMURAIParameters;
}    

//__________________________________________________________
void TArtSAMURAIParameters::Delete()
{
  delete fSAMURAIParameters;
  fSAMURAIParameters = 0;  
}

//__________________________________________________________
Bool_t TArtSAMURAIParameters::LoadParameter(const char *xmlfile)
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
Bool_t TArtSAMURAIParameters::LoadNeuLANDTCal(const char *csvfile)
{
  TArtCore::Info(__FILE__,"Load NeuLAND tcal from %s", csvfile);
  std::ifstream file(csvfile);
  if (!file) {
    std::cerr << csvfile << ": Could not open." << std::endl;
    return false;
  }
#define ARRAY_SIZE 100
  int channel_array[ARRAY_SIZE];
  Double_t ns_array[ARRAY_SIZE];
  size_t size = 0;
  int prev_t_type = 0, prev_bar_id = 0;
#define ERROR_PREFIX csvfile << ':' << line_no << ": "
  for (int line_no = 1;; ++line_no) {
    char line[256];
    bool is_start = false;
    int plane, bar, t_type, channel;
    double ns;
    bool finalize = false;
    file.getline(line, sizeof line);
    if (!file) {
      finalize = true;
    } else {
      if (1 == line_no) {
        continue;
      }
#define STRTOK_R(start) do {\
  token = strtok_r(start, ", ", &p);\
  if (NULL == token) {\
    std::cerr << ERROR_PREFIX << "Unexpected EOL." << std::endl;\
    return false;\
  }\
} while (0)
char *p, *token;
STRTOK_R(line);
if (0 == strcmp(token, "START_TCAL")) {
  is_start = true;
} else if (0 != strcmp(token, "NEULAND_TCAL")) {
  std::cerr << ERROR_PREFIX << "Expected START_TCAL or NEULAND_TCAL, "
    "got \"" << token << "\"." << std::endl;
  return false;
}
STRTOK_R(p);
plane = strtol(token, NULL, 10);
STRTOK_R(p);
bar = strtol(token, NULL, 10);
if (is_start && (0 != plane || 0 != bar)) {
  std::cerr << ERROR_PREFIX << "START_TCAL should have 0 plane and bar."
    << std::endl;
  return false;
}
STRTOK_R(p);
t_type = strtol(token, NULL, 10);
STRTOK_R(p);
channel = strtol(token, NULL, 10);
STRTOK_R(p);
ns = strtod(token, NULL);
token = strtok_r(p, ", ", &p);
if (NULL != token) {
  std::cerr << ERROR_PREFIX << "Garbage after line." << std::endl;
  return false;
}
}
if ((prev_t_type && prev_t_type != t_type) || finalize) {
  std::map<int,TArtNeuLANDPlaPara*>::const_iterator it =
    fIDNeuLANDPlaParaMap.find(prev_bar_id);
  if (fIDNeuLANDPlaParaMap.end() == it) {
    std::cerr << ERROR_PREFIX << "Could not find NeuLAND para for bar "
      << prev_bar_id << ", must be in XML parameters file." <<
      std::endl;
    return false;
  }
  TArtNeuLANDPlaPara *para = it->second;
  para->SetTDCTCal(prev_t_type, channel_array, ns_array, size);
  if (finalize) {
    break;
  }
  size = 0;
}
prev_t_type = t_type;
prev_bar_id = is_start ? 401 : (plane - 1) * 50 + bar;
channel_array[size] = channel;
ns_array[size] = ns;
++size;
}
return true;
}

//__________________________________________________________
void TArtSAMURAIParameters::ParseParaList(TXMLNode *node)
{ 
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "SAMURAIHOD") == 0){
      listOfHODPlaPara->Add(ParseHODPlaPara(node->GetChildren()));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIRPTOF") == 0){
      listOfRPTOFPlaPara->Add(ParseRPTOFPlaPara(node->GetChildren()));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAITED") == 0){
      listOfTEDCsIPara->Add(ParseTEDCsIPara(node->GetChildren()));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIBPC") == 0){
      listOfBPCHitPara->Add(ParseDCHitPara(node->GetChildren(),&bpc_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIBDC1") == 0){
      listOfBDC1HitPara->Add(ParseDCHitPara(node->GetChildren(),&bdc1_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIBDC2") == 0){
      listOfBDC2HitPara->Add(ParseDCHitPara(node->GetChildren(),&bdc2_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIFDC0") == 0){
      listOfFDC0HitPara->Add(ParseDCHitPara(node->GetChildren(),&fdc0_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIPDC") == 0){
      listOfPDCHitPara->Add(ParseDCHitPara(node->GetChildren(),&pdc_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIFDC1") == 0){
      listOfFDC1HitPara->Add(ParseDCHitPara(node->GetChildren(),&fdc1_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIFDC2") == 0){
      listOfFDC2HitPara->Add(ParseDCHitPara(node->GetChildren(),&fdc2_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIRPDC") == 0){
      listOfRPDCHitPara->Add(ParseDCHitPara(node->GetChildren(),&rpdc_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIS1DC1") == 0){
      listOfS1DC1HitPara->Add(ParseDCHitPara(node->GetChildren(),&s1dc1_pmap));
    }
    else if(strcmp(node->GetNodeName(), "SAMURAIS1DC2") == 0){
      listOfS1DC2HitPara->Add(ParseDCHitPara(node->GetChildren(),&s1dc2_pmap));
    }
    else if(strcmp(node->GetNodeName(), "NEBULA") == 0){
      ParseNEBULAPlaPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "NEBULAHPC") == 0){
      ParseNEBULAHPCPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "NEULAND") == 0){
      ParseNeuLANDPlaPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "TACQUILA") == 0){
      ParseTacquilaPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "NEULANDVETO") == 0){
      ParseNeuLANDVETOPlaPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "HIME") == 0){
      ParseHIMEPlaPara(node->GetChildren());
    }
    else if(strcmp(node->GetNodeName(), "NINJA") == 0){
      ParseNINJAPlaPara(node->GetChildren());
    }
  }
}


//
// Parser for NEBULA Plastic
//
//__________________________________________________________
TArtNEBULAPlaPara *TArtSAMURAIParameters::ParseNEBULAPlaPara(TXMLNode *node)
{
  TArtNEBULAPlaPara* para = new TArtNEBULAPlaPara;
  Int_t tu_geo = -1, tu_ch = -1, qu_geo = -1, qu_ch = -1;
  Int_t td_geo = -1, td_ch = -1, qd_geo = -1, qd_ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      para->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "Layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->SetLayer(layer);
    }else if(strcmp(node->GetNodeName(), "SubLayer") == 0){
      Int_t sublayer = atoi(node->GetText());
      para->SetSubLayer(sublayer);
    }else if(strcmp(node->GetNodeName(), "PosX") == 0){
      Double_t posx = atof(node->GetText());
      para->SetDetPos(posx, 0);
    }else if(strcmp(node->GetNodeName(), "PosY") == 0){
      Double_t posy = atof(node->GetText());
      para->SetDetPos(posy, 1);
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      para->SetDetPos(posz, 2);
    }else if(strcmp(node->GetNodeName(), "TUCal") == 0){
      Double_t tucal = atof(node->GetText());
      para->SetTUCal(tucal);
    }else if(strcmp(node->GetNodeName(), "TUOff") == 0){
      Double_t tuoff = atof(node->GetText());
      para->SetTUOff(tuoff);
    }else if(strcmp(node->GetNodeName(), "TUSlw") == 0){
      Double_t tuslw = atof(node->GetText());
      para->SetTUSlw(tuslw);
    }else if(strcmp(node->GetNodeName(), "QUCal") == 0){
      Double_t qucal = atof(node->GetText());
      para->SetQUCal(qucal);
    }else if(strcmp(node->GetNodeName(), "QUPed") == 0){
      Double_t quped = atof(node->GetText());
      para->SetQUPed(quped);
    }else if(strcmp(node->GetNodeName(), "TDCal") == 0){
      Double_t tdcal = atof(node->GetText());
      para->SetTDCal(tdcal);
    }else if(strcmp(node->GetNodeName(), "TDOff") == 0){
      Double_t tdoff = atof(node->GetText());
      para->SetTDOff(tdoff);
    }else if(strcmp(node->GetNodeName(), "TDSlw") == 0){
      Double_t tdslw = atof(node->GetText());
      para->SetTDSlw(tdslw);
    }else if(strcmp(node->GetNodeName(), "QDCal") == 0){
      Double_t qdcal = atof(node->GetText());
      para->SetQDCal(qdcal);
    }else if(strcmp(node->GetNodeName(), "QDPed") == 0){
      Double_t qdped = atof(node->GetText());
      para->SetQDPed(qdped);
    }else if(strcmp(node->GetNodeName(), "DTCal") == 0){
      Double_t dtcal = atof(node->GetText());
      para->SetDTCal(dtcal);
    }else if(strcmp(node->GetNodeName(), "DTOff") == 0){
      Double_t dtoff = atof(node->GetText());
      para->SetDTOff(dtoff);
    }else if(strcmp(node->GetNodeName(), "TAveOff") == 0){
      Double_t taveoff = atof(node->GetText());
      para->SetTAveOff(taveoff);
    }else if(strcmp(node->GetNodeName(), "Ignore") == 0){
      Int_t ignore = atoi(node->GetText());
      para->SetIgnore(ignore);
    }else if(strcmp(node->GetNodeName(), "tu_geo") == 0){
      tu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "tu_ch") == 0){
      tu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_geo") == 0){
      qu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_ch") == 0){
      qu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_geo") == 0){
      td_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_ch") == 0){
      td_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_geo") == 0){
      qd_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_ch") == 0){
      qd_ch = atoi(node->GetText());
    }
  }

  Int_t detector_q = 0;
  Int_t detector_t = 0;

  if(para->GetLayer() == 1){
    detector_q = NEBULA1Q;
    detector_t = NEBULA1T;
  }else if(para->GetLayer() == 2){
    detector_q = NEBULA2Q;
    detector_t = NEBULA2T;
  }else if(para->GetLayer() == 3){
    detector_q = NEBULA3Q;
    detector_t = NEBULA3T;
  }else if(para->GetLayer() == 4){
    detector_q = NEBULA4Q;
    detector_t = NEBULA4T;
  }

  para->SetMapU(detector_q, qu_geo, qu_ch);
  fNEBULAPlaUParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapU(), para));
  para->SetMapU(detector_t, tu_geo, tu_ch);
  fNEBULAPlaUParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapU(), para));
  para->SetMapD(detector_q, qd_geo, qd_ch);
  fNEBULAPlaDParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapD(), para));
  para->SetMapD(detector_t, td_geo, td_ch);
  fNEBULAPlaDParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapD(), para));

  fIDNEBULAPlaParaMap.insert(std::pair<int,TArtNEBULAPlaPara*>(para->GetID(), para));

  return para;
}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNEBULAPlaUPara(const TArtRIDFMap &rmap) const
{
  TArtNEBULAPlaParaMap::const_iterator itr = fNEBULAPlaUParaMap->find(rmap);
  if(itr != fNEBULAPlaUParaMap->end()) return itr->second;
  else return 0;
}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNEBULAPlaDPara(const TArtRIDFMap &rmap) const
{
  TArtNEBULAPlaParaMap::const_iterator itr = fNEBULAPlaDParaMap->find(rmap);
  if(itr != fNEBULAPlaDParaMap->end()) return itr->second;
  else return 0;
}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNEBULAPlaPara(Int_t id) const
{
  std::map<int,TArtNEBULAPlaPara*>::const_iterator it = fIDNEBULAPlaParaMap.find(id);
  if(it != fIDNEBULAPlaParaMap.end()) return it->second;
  else return 0;
}

//
// parameter parser for NEBULA HPC
//
//
//__________________________________________________________
TArtNEBULAHPCPara *TArtSAMURAIParameters::ParseNEBULAHPCPara(TXMLNode *node)
{
  TArtNEBULAHPCPara* para = new TArtNEBULAHPCPara;
  Int_t tdc_geo = -1, tdc_ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      para->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "Layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->SetLayer(layer);
    }else if(strcmp(node->GetNodeName(), "SubLayer") == 0){
      Int_t sublayer = atoi(node->GetText());
      para->SetSubLayer(sublayer);
    }else if(strcmp(node->GetNodeName(), "PosX") == 0){
      Double_t posx = atof(node->GetText());
      para->SetDetPos(posx, 0);
    }else if(strcmp(node->GetNodeName(), "PosY") == 0){
      Double_t posy = atof(node->GetText());
      para->SetDetPos(posy, 1);
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      para->SetDetPos(posz, 2);
    }else if(strcmp(node->GetNodeName(), "TCal") == 0){
      Double_t tcal = atof(node->GetText());
      para->SetTCal(tcal);
    }else if(strcmp(node->GetNodeName(), "TOff") == 0){
      Double_t toff = atof(node->GetText());
      para->SetTOff(toff);
    }else if(strcmp(node->GetNodeName(), "tdc_geo") == 0){
      tdc_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "tdc_ch") == 0){
      tdc_ch = atoi(node->GetText());
    }
  }

  para->SetMap(tdc_geo, tdc_ch);
  fNEBULAHPCParaMap->insert(TArtNEBULAHPCParaPair(para->GetMap(), para));

  fIDNEBULAHPCParaMap.insert(std::pair<int,TArtNEBULAHPCPara*>(para->GetID(), para));

  return para;
}

const TArtNEBULAHPCPara* TArtSAMURAIParameters::FindNEBULAHPCPara(const TArtRIDFMap &rmap) const
{
  TArtNEBULAHPCParaMap::const_iterator itr = fNEBULAHPCParaMap->find(rmap);
  if(itr != fNEBULAHPCParaMap->end()) return itr->second;
  else return 0;
}

const TArtNEBULAHPCPara* TArtSAMURAIParameters::FindNEBULAHPCPara(Int_t id) const
{
  std::map<int,TArtNEBULAHPCPara*>::const_iterator it = fIDNEBULAHPCParaMap.find(id);
  if(it != fIDNEBULAHPCParaMap.end()) return it->second;
  else return 0;
}

void TArtSAMURAIParameters::PrintListOfNEBULAPlaPara() const {
  TArtNEBULAPlaParaMap::iterator it = fNEBULAPlaUParaMap->begin();
  while(it !=  fNEBULAPlaUParaMap->end()){
    std::cout << *(it->second) << std::endl;
    ++it;
  }
}

// HIME
//
// Parser for HIME Plastic
//
//__________________________________________________________
TArtHIMEPlaPara *TArtSAMURAIParameters::ParseHIMEPlaPara(TXMLNode *node)
{
  TArtHIMEPlaPara* para = new TArtHIMEPlaPara;
  Int_t tu_geo = -1, tu_ch = -1, qu_geo = -1, qu_ch = -1;
  Int_t td_geo = -1, td_ch = -1, qd_geo = -1, qd_ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      para->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "Layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->SetLayer(layer);
    }else if(strcmp(node->GetNodeName(), "SubLayer") == 0){
      Int_t sublayer = atoi(node->GetText());
      para->SetSubLayer(sublayer);
    }else if(strcmp(node->GetNodeName(), "direction") == 0){
      if(strcmp(node->GetText(),"H")){
        para->SetDetDirect(para->Horiz);
      } else {
        para->SetDetDirect(para->Vert);
      }
    }else if(strcmp(node->GetNodeName(), "PosX") == 0){
      Double_t posx = atof(node->GetText());
      para->SetDetPos(posx, 0);
    }else if(strcmp(node->GetNodeName(), "PosY") == 0){
      Double_t posy = atof(node->GetText());
      para->SetDetPos(posy, 1);
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      para->SetDetPos(posz, 2);
    }else if(strcmp(node->GetNodeName(), "TUCal") == 0){
      Double_t tucal = atof(node->GetText());
      para->SetTUCal(tucal);
    }else if(strcmp(node->GetNodeName(), "TUOff") == 0){
      Double_t tuoff = atof(node->GetText());
      para->SetTUOff(tuoff);
    }else if(strcmp(node->GetNodeName(), "TUSlw") == 0){
      Double_t tuslw = atof(node->GetText());
      para->SetTUSlw(tuslw);
    }else if(strcmp(node->GetNodeName(), "QUCal") == 0){
      Double_t qucal = atof(node->GetText());
      para->SetQUCal(qucal);
    }else if(strcmp(node->GetNodeName(), "QUPed") == 0){
      Double_t quped= atof(node->GetText());
      para->SetQUPed(quped);
    }else if(strcmp(node->GetNodeName(), "TDCal") == 0){
      Double_t tdcal = atof(node->GetText());
      para->SetTDCal(tdcal);
    }else if(strcmp(node->GetNodeName(), "TDOff") == 0){
      Double_t tdoff = atof(node->GetText());
      para->SetTDOff(tdoff);
    }else if(strcmp(node->GetNodeName(), "TDSlw") == 0){
      Double_t tdslw = atof(node->GetText());
      para->SetTDSlw(tdslw);
    }else if(strcmp(node->GetNodeName(), "QDCal") == 0){
      Double_t qdcal = atof(node->GetText());
      para->SetQDCal(qdcal);
    }else if(strcmp(node->GetNodeName(), "QDPed") == 0){
      Double_t qdped= atof(node->GetText());
      para->SetQDPed(qdped);
    }else if(strcmp(node->GetNodeName(), "DTCal") == 0){
      Double_t dtcal = atof(node->GetText());
      para->SetDTCal(dtcal);
    }else if(strcmp(node->GetNodeName(), "DTOff") == 0){
      Double_t dtoff = atof(node->GetText());
      para->SetDTOff(dtoff);
    }else if(strcmp(node->GetNodeName(), "TAveOff") == 0){
      Double_t taveoff = atof(node->GetText());
      para->SetTAveOff(taveoff);
    }else if(strcmp(node->GetNodeName(), "Ignore") == 0){
      Int_t ignore = atoi(node->GetText());
      para->SetIgnore(ignore);
    }else if(strcmp(node->GetNodeName(), "tu_geo") == 0){
      tu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "tu_ch") == 0){
      tu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_geo") == 0){
      qu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_ch") == 0){
      qu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_geo") == 0){
      td_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_ch") == 0){
      td_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_geo") == 0){
      qd_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_ch") == 0){
      qd_ch = atoi(node->GetText());
    }
  }

  Int_t detector_q = HIMEQ;
  Int_t detector_t = HIMET;

  // Int_t detector_q = 0;
  // Int_t detector_t = 0;


  // if(para->GetLayer() == 1){
  //   detector_q = HIME1Q;
  //   detector_t = HIME1T;
  // }else if(para->GetLayer() == 2){
  //   detector_q = HIME2Q;
  //   detector_t = HIME2T;
  // }else if(para->GetLayer() == 3){
  //   detector_q = HIME3Q;
  //   detector_t = HIME3T;
  // }else if(para->GetLayer() == 4){
  //   detector_q = HIME4Q;
  //   detector_t = HIME4T;
  // }

  para->SetMapU(detector_q, qu_geo, qu_ch);
  fHIMEPlaUParaMap->insert(TArtHIMEPlaParaPair(para->GetMapU(), para));
  para->SetMapU(detector_t, tu_geo, tu_ch);
  fHIMEPlaUParaMap->insert(TArtHIMEPlaParaPair(para->GetMapU(), para));
  para->SetMapD(detector_q, qd_geo, qd_ch);
  fHIMEPlaDParaMap->insert(TArtHIMEPlaParaPair(para->GetMapD(), para));
  para->SetMapD(detector_t, td_geo, td_ch);
  fHIMEPlaDParaMap->insert(TArtHIMEPlaParaPair(para->GetMapD(), para));

  fIDHIMEPlaParaMap.insert(std::pair<int,TArtHIMEPlaPara*>(para->GetID(), para));

  return para;
}

const TArtHIMEPlaPara* TArtSAMURAIParameters::FindHIMEPlaUPara(const TArtRIDFMap &rmap) const
{
  TArtHIMEPlaParaMap::const_iterator itr = fHIMEPlaUParaMap->find(rmap);
  if(itr != fHIMEPlaUParaMap->end()) return itr->second;
  else return 0;
}

const TArtHIMEPlaPara* TArtSAMURAIParameters::FindHIMEPlaDPara(const TArtRIDFMap &rmap) const
{
  TArtHIMEPlaParaMap::const_iterator itr = fHIMEPlaDParaMap->find(rmap);
  if(itr != fHIMEPlaDParaMap->end()) return itr->second;
  else return 0;
}

const TArtHIMEPlaPara* TArtSAMURAIParameters::FindHIMEPlaPara(Int_t id) const
{
  std::map<int,TArtHIMEPlaPara*>::const_iterator it = fIDHIMEPlaParaMap.find(id);
  if(it != fIDHIMEPlaParaMap.end()) return it->second;
  else return 0;
}

void TArtSAMURAIParameters::PrintListOfHIMEPlaPara() const {
  TArtHIMEPlaParaMap::iterator it = fHIMEPlaUParaMap->begin();
  while(it !=  fHIMEPlaUParaMap->end()){
    std::cout << *(it->second) << std::endl;
    ++it;
  }
}

//
// Parser for NeuLAND Plastic
//
//__________________________________________________________
TArtNeuLANDPlaPara *TArtSAMURAIParameters::ParseNeuLANDPlaPara(TXMLNode *node)
{
  TArtNeuLANDPlaPara* para = new TArtNeuLANDPlaPara;
  Int_t sam0 = -1, gtb0 = -1, mod0 = -1, ch0 = -1;
  Int_t sam1 = -1, gtb1 = -1, mod1 = -1, ch1 = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPL") == 0){
      Int_t fpl = atoi(node->GetText());
      para->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "direction") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->SetLayer(layer);
    }else if(strcmp(node->GetNodeName(), "bar") == 0){
      Int_t bar = atoi(node->GetText());
      para->SetBarID(bar);
    }else if(strcmp(node->GetNodeName(), "qcal0") == 0){
      Double_t qcal = atof(node->GetText());
      para->SetQCal(0,qcal);
    }else if(strcmp(node->GetNodeName(), "qped0") == 0){
      Double_t qped = atof(node->GetText());
      para->SetQPed(0,qped);
    }else if(strcmp(node->GetNodeName(), "tdc0ns0") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC0ns(0,val);
    }else if(strcmp(node->GetNodeName(), "tdc25ns0") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC25ns(0,val);
    }else if(strcmp(node->GetNodeName(), "tdc0ns0_t17") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC0ns_t17(0,val);
    }else if(strcmp(node->GetNodeName(), "tdc25ns0_t17") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC25ns_t17(0,val);
    }else if(strcmp(node->GetNodeName(), "qcal1") == 0){
      Double_t qcal = atof(node->GetText());
      para->SetQCal(1,qcal);
    }else if(strcmp(node->GetNodeName(), "qped1") == 0){
      Double_t qped = atof(node->GetText());
      para->SetQPed(1,qped);
    }else if(strcmp(node->GetNodeName(), "tdc0ns1") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC0ns(1,val);
    }else if(strcmp(node->GetNodeName(), "tdc25ns1") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC25ns(1,val);
    }else if(strcmp(node->GetNodeName(), "tdc0ns1_t17") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC0ns_t17(1,val);
    }else if(strcmp(node->GetNodeName(), "tdc25ns1_t17") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDC25ns_t17(1,val);
    }else if(strcmp(node->GetNodeName(), "tdiff_offset") == 0){
      Double_t val = atof(node->GetText());
      para->SetTDiffOffset(val);
    }else if(strcmp(node->GetNodeName(), "sync_offset") == 0){
      Double_t val = atof(node->GetText());
      para->SetTSyncOffset(val);
    }else if(strcmp(node->GetNodeName(), "vscint") == 0){
      Double_t val = atof(node->GetText());
      para->SetVScint(val);
    }else if(strcmp(node->GetNodeName(), "ediff_offset") == 0){
      Double_t val = atof(node->GetText());
      para->SetEDiffOffset(val);
    }else if(strcmp(node->GetNodeName(), "esync_offset") == 0){
      Double_t val = atof(node->GetText());
      para->SetESyncOffset(val);
    }else if(strcmp(node->GetNodeName(), "att") == 0){
      Double_t val = atof(node->GetText());
      para->SetAtt(val);
    }else if(strcmp(node->GetNodeName(), "zpos") == 0){
      Double_t val = atof(node->GetText());
      para->SetZPos(val);
    }else if(strcmp(node->GetNodeName(), "sam0") == 0){
      sam0 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "gtb0") == 0){
      gtb0 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "module0") == 0){
      mod0 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "channel0") == 0){
      ch0 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "sam1") == 0){
      sam1 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "gtb1") == 0){
      gtb1 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "module1") == 0){
      mod1 = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "channel1") == 0){
      ch1 = atoi(node->GetText());
    }
  }

  para->SetMap(0,sam0,gtb0,mod0,ch0);
  para->SetMap(1,sam1,gtb1,mod1,ch1);
  fNeuLANDPlaParaMap->insert(TArtNeuLANDPlaParaPair(para->GetMap(0), para));
  fNeuLANDPlaParaMap->insert(TArtNeuLANDPlaParaPair(para->GetMap(1), para));

  fIDNeuLANDPlaParaMap.insert(std::pair<int,TArtNeuLANDPlaPara*>(para->GetID(), para));

  return para;
}

const TArtNeuLANDPlaPara* TArtSAMURAIParameters::FindNeuLANDPlaPara(const TArtTacquilaMap *rmap) const
{
  TArtNeuLANDPlaParaMap::const_iterator itr = fNeuLANDPlaParaMap->find(*rmap);
  if(itr != fNeuLANDPlaParaMap->end()) return itr->second;
  else return 0;
}

const TArtNeuLANDPlaPara* TArtSAMURAIParameters::FindNeuLANDPlaPara(Int_t id) const
{
  std::map<int,TArtNeuLANDPlaPara*>::const_iterator it = fIDNeuLANDPlaParaMap.find(id);
  if(it != fIDNeuLANDPlaParaMap.end()) return it->second;
  else return 0;
}

void TArtSAMURAIParameters::PrintListOfNeuLANDPlaPara() const {
  TArtNeuLANDPlaParaMap::iterator it = fNeuLANDPlaParaMap->begin();
  while(it !=  fNeuLANDPlaParaMap->end()){
    std::cout << *(it->second) << std::endl;
    ++it;
  }
}

//
// Parser for Tacquila
//
//__________________________________________________________
TArtTacquilaPara *TArtSAMURAIParameters::ParseTacquilaPara(TXMLNode *node)
{
  TArtTacquilaPara* para = new TArtTacquilaPara;
  Int_t sam = -1, gtb = -1, mod = -1, ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "tac2ns") == 0){
      Double_t tcal = atof(node->GetText());
      para->SetTac2Ns(tcal);
    }else if(strcmp(node->GetNodeName(), "sam") == 0){
      sam = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "gtb") == 0){
      gtb = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "module") == 0){
      mod = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "channel") == 0){
      ch = atoi(node->GetText());
    }
  }

  para->SetMap(sam,gtb,mod,ch);
  fTacquilaParaMap->insert(TArtTacquilaParaPair(para->GetMap(), para));

  fIDTacquilaParaMap.insert(std::pair<int,TArtTacquilaPara*>(para->GetID(), para));

  return para;
}

const TArtTacquilaPara* TArtSAMURAIParameters::FindTacquilaPara(const TArtTacquilaMap *rmap) const
{
  TArtTacquilaParaMap::const_iterator itr = fTacquilaParaMap->find(*rmap);
  if(itr != fTacquilaParaMap->end()) return itr->second;
  else return 0;
}

const TArtTacquilaPara* TArtSAMURAIParameters::FindTacquilaPara(Int_t id) const
{
  std::map<int,TArtTacquilaPara*>::const_iterator it = fIDTacquilaParaMap.find(id);
  if(it != fIDTacquilaParaMap.end()) return it->second;
  else return 0;
}
//__________________________________________________________
TArtNEBULAPlaPara *TArtSAMURAIParameters::ParseNeuLANDVETOPlaPara(TXMLNode *node)
{
  TArtNEBULAPlaPara* para = new TArtNEBULAPlaPara;
  Int_t tu_geo = -1, tu_ch = -1, qu_geo = -1, qu_ch = -1;
  Int_t td_geo = -1, td_ch = -1, qd_geo = -1, qd_ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      para->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "Layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->SetLayer(layer);
    }else if(strcmp(node->GetNodeName(), "SubLayer") == 0){
      Int_t sublayer = atoi(node->GetText());
      para->SetSubLayer(sublayer);
    }else if(strcmp(node->GetNodeName(), "PosX") == 0){
      Double_t posx = atof(node->GetText());
      para->SetDetPos(posx, 0);
    }else if(strcmp(node->GetNodeName(), "PosY") == 0){
      Double_t posy = atof(node->GetText());
      para->SetDetPos(posy, 1);
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      para->SetDetPos(posz, 2);
    }else if(strcmp(node->GetNodeName(), "TUCal") == 0){
      Double_t tucal = atof(node->GetText());
      para->SetTUCal(tucal);
    }else if(strcmp(node->GetNodeName(), "TUOff") == 0){
      Double_t tuoff = atof(node->GetText());
      para->SetTUOff(tuoff);
    }else if(strcmp(node->GetNodeName(), "TUSlw") == 0){
      Double_t tuslw = atof(node->GetText());
      para->SetTUSlw(tuslw);
    }else if(strcmp(node->GetNodeName(), "QUCal") == 0){
      Double_t qucal = atof(node->GetText());
      para->SetQUCal(qucal);
    }else if(strcmp(node->GetNodeName(), "QUPed") == 0){
      Double_t quped = atof(node->GetText());
      para->SetQUPed(quped);
    }else if(strcmp(node->GetNodeName(), "TDCal") == 0){
      Double_t tdcal = atof(node->GetText());
      para->SetTDCal(tdcal);
    }else if(strcmp(node->GetNodeName(), "TDOff") == 0){
      Double_t tdoff = atof(node->GetText());
      para->SetTDOff(tdoff);
    }else if(strcmp(node->GetNodeName(), "TDSlw") == 0){
      Double_t tdslw = atof(node->GetText());
      para->SetTDSlw(tdslw);
    }else if(strcmp(node->GetNodeName(), "QDCal") == 0){
      Double_t qdcal = atof(node->GetText());
      para->SetQDCal(qdcal);
    }else if(strcmp(node->GetNodeName(), "QDPed") == 0){
      Double_t qdped = atof(node->GetText());
      para->SetQDPed(qdped);
    }else if(strcmp(node->GetNodeName(), "DTCal") == 0){
      Double_t dtcal = atof(node->GetText());
      para->SetDTCal(dtcal);
    }else if(strcmp(node->GetNodeName(), "DTOff") == 0){
      Double_t dtoff = atof(node->GetText());
      para->SetDTOff(dtoff);
    }else if(strcmp(node->GetNodeName(), "TAveOff") == 0){
      Double_t taveoff = atof(node->GetText());
      para->SetTAveOff(taveoff);
    }else if(strcmp(node->GetNodeName(), "Ignore") == 0){
      Int_t ignore = atoi(node->GetText());
      para->SetIgnore(ignore);
    }else if(strcmp(node->GetNodeName(), "tu_geo") == 0){
      tu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "tu_ch") == 0){
      tu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_geo") == 0){
      qu_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qu_ch") == 0){
      qu_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_geo") == 0){
      td_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "td_ch") == 0){
      td_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_geo") == 0){
      qd_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "qd_ch") == 0){
      qd_ch = atoi(node->GetText());
    }
  }
  Int_t detector_q = NEULANDVETOQ;
  Int_t detector_t = NEULANDVETOT;

  para->SetMapU(detector_t, tu_geo, tu_ch);
  fNeuLANDVETOPlaUParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapU(), para));
  para->SetMapD(detector_t, td_geo, td_ch);
  fNeuLANDVETOPlaDParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapD(), para));

  para->SetMapU(detector_q, qu_geo, qu_ch);
  fNeuLANDVETOPlaUParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapU(), para));
  para->SetMapD(detector_q, qd_geo, qd_ch);
  fNeuLANDVETOPlaDParaMap->insert(TArtNEBULAPlaParaPair(para->GetMapD(), para));

  fIDNeuLANDVETOPlaParaMap.insert(std::pair<int,TArtNEBULAPlaPara*>(para->GetID(), para));

  return para;

}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNeuLANDVETOPlaUPara(const TArtRIDFMap &rmap) const
{
  TArtNEBULAPlaParaMap::const_iterator itr = fNeuLANDVETOPlaUParaMap->find(rmap);
  if(itr != fNeuLANDVETOPlaUParaMap->end()) return itr->second;
  else return 0;
}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNeuLANDVETOPlaDPara(const TArtRIDFMap &rmap) const
{
  TArtNEBULAPlaParaMap::const_iterator itr = fNeuLANDVETOPlaDParaMap->find(rmap);
  if(itr != fNeuLANDVETOPlaDParaMap->end()) return itr->second;
  else return 0;
}

const TArtNEBULAPlaPara* TArtSAMURAIParameters::FindNeuLANDVETOPlaPara(Int_t id) const
{
  std::map<int,TArtNEBULAPlaPara*>::const_iterator it = fIDNeuLANDVETOPlaParaMap.find(id);
  if(it != fIDNeuLANDVETOPlaParaMap.end()) return it->second;
  else return 0;
}

void TArtSAMURAIParameters::PrintListOfNeuLANDVETOPlaPara() const {
  TArtNEBULAPlaParaMap::iterator it = fNeuLANDVETOPlaUParaMap->begin();
  while(it !=  fNeuLANDVETOPlaUParaMap->end()){
    std::cout << *(it->second) << std::endl;
    ++it;
  }
  //std::cout<<fNeuLANDVETOPlaUParaMap->size()<<std::endl;
}

//
// parameter parser for HOD
//
//
//__________________________________________________________
TArtHODPlaPara *TArtSAMURAIParameters::ParseHODPlaPara(TXMLNode *node)
{

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Double_t  tup_offset = 0;
  Double_t  tup_ch2ns = 0;
  Double_t  tdown_offset = 0;
  Double_t  tdown_ch2ns = 0;
  Double_t  qped_up = 0;
  Double_t  qped_down = 0;
  Double_t  qcal_up = 0;
  Double_t  qcal_down = 0;
  Double_t  tslew_up_a = 0;
  Double_t  tslew_up_b = 0;
  Double_t  tslew_down_a = 0;
  Double_t  tslew_down_b = 0;

  Int_t tdet, tup_geo, tup_ch, tdown_geo, tdown_ch;
  Int_t qdet, qup_geo, qup_ch, qdown_geo, qdown_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tup_offset") == 0)
        tup_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tup_ch2ns") == 0)
        tup_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdown_offset") == 0)
        tdown_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdown_ch2ns") == 0)
        tdown_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_up") == 0)
        qcal_up = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_down") == 0)
        qcal_down = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_up") == 0)
        qped_up = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_down") == 0)
        qped_down = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_up_a")==0)
        tslew_up_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_up_b")==0)
        tslew_up_b = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_down_a")==0)
        tslew_down_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_down_b")==0)
        tslew_down_b = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "t_det") == 0)
        tdet = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tup_geo") == 0)
        tup_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tup_ch") == 0)
        tup_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdown_geo") == 0)
        tdown_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdown_ch") == 0)
        tdown_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q_det") == 0)
        qdet = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qup_geo") == 0)
        qup_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qup_ch") == 0)
        qup_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qdown_geo") == 0)
        qdown_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qdown_ch") == 0)
        qdown_ch = atoi(node->GetText());
    }
  }

  //  TArtCore::Info(__FILE__,"Reading database for %s HOD-Plastic",name.Data());
  TArtHODPlaPara * para =
    new TArtHODPlaPara(id, name, fpl, 
        tup_ch2ns, tdown_ch2ns,
        tup_offset, tdown_offset,
        qcal_up, qcal_down, qped_up, qped_down,
        tslew_up_a, tslew_up_b, tslew_down_a, tslew_down_b);
  para->SetMap(tdet, tup_geo, tup_ch, tdown_geo, tdown_ch,
      qdet, qup_geo, qup_ch, qdown_geo, qdown_ch);

  hod_pmap.insert(std::pair<TArtRIDFMap, TArtHODPlaPara *>(*((TArtRIDFMap *)para->GetTUMap()), para));
  hod_pmap.insert(std::pair<TArtRIDFMap, TArtHODPlaPara *>(*((TArtRIDFMap *)para->GetTDMap()), para));
  hod_pmap.insert(std::pair<TArtRIDFMap, TArtHODPlaPara *>(*((TArtRIDFMap *)para->GetQUMap()), para));
  hod_pmap.insert(std::pair<TArtRIDFMap, TArtHODPlaPara *>(*((TArtRIDFMap *)para->GetQDMap()), para));

  return para;

}

void TArtSAMURAIParameters::PrintListOfHODPlaPara(){
  TIter next(listOfHODPlaPara);
  while (TArtHODPlaPara *p = (TArtHODPlaPara *)next()) std::cout << *p;
}

const TArtHODPlaPara* TArtSAMURAIParameters::FindHODPlaPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtHODPlaPara *>::const_iterator itr = hod_pmap.find(*rmap);
  if(itr != hod_pmap.end()) return (TArtHODPlaPara*)itr->second;
  else return 0;
}

// ****************** NINJA  *******************

//__________________________________________________________
TArtNINJAPlaPara *TArtSAMURAIParameters::ParseNINJAPlaPara(TXMLNode *node)
{
  TArtNINJAPlaPara* para = new TArtNINJAPlaPara;
  Int_t t0_geo = -1, t0_ch = -1;
  Int_t t1_geo = -1, t1_ch = -1;

  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      para->fID=id;
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      para->fDetName=name;
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      para->fFpl=fpl;
    }else if(strcmp(node->GetNodeName(), "Layer") == 0){
      Int_t layer = atoi(node->GetText());
      para->fLayer=layer;
    }else if(strcmp(node->GetNodeName(), "SubLayer") == 0){
      Int_t sublayer = atoi(node->GetText());
      para->fSubLayer=sublayer;
    }else if(strcmp(node->GetNodeName(), "Direction") == 0){
      if(strcmp(node->GetText(),"H")){
        para->fDetDirect=para->Horiz;
      } else if (strcmp(node->GetText(),"V")){
        para->fDetDirect=para->Vert;
      }else{
        printf("ERROR: NINJA plastic direction improperly defined\n");
        exit(1);
      }
    }else if(strcmp(node->GetNodeName(), "PosX") == 0){
      Double_t posx = atof(node->GetText());
      para->fDetPos[0]=posx;
    }else if(strcmp(node->GetNodeName(), "PosY") == 0){
      Double_t posy = atof(node->GetText());
      para->fDetPos[1]=posy;
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      para->fDetPos[2]=posz;
    }else if(strcmp(node->GetNodeName(), "T_LECal0") == 0){
      Double_t val = atof(node->GetText());
      para->fT_LECal[0]= val;
    }else if(strcmp(node->GetNodeName(), "T_LECal1") == 0){
      Double_t val = atof(node->GetText());
      para->fT_LECal[1]= val;
    }else if(strcmp(node->GetNodeName(), "T_LEOffset0") == 0){
      Double_t val = atof(node->GetText());
      para->fT_LEOffset[0]= val;
    }else if(strcmp(node->GetNodeName(), "T_LEOffset1") == 0){
      Double_t val = atof(node->GetText());
      para->fT_LEOffset[1]= val;
    }else if(strcmp(node->GetNodeName(), "T_TECal0") == 0){
      Double_t val = atof(node->GetText());
      para->fT_TECal[0]= val;
    }else if(strcmp(node->GetNodeName(), "T_TECal1") == 0){
      Double_t val = atof(node->GetText());
      para->fT_TECal[1]= val;
    }else if(strcmp(node->GetNodeName(), "T_TEOffset0") == 0){
      Double_t val = atof(node->GetText());
      para->fT_TEOffset[0]= val;
    }else if(strcmp(node->GetNodeName(), "T_TEOffset1") == 0){
      Double_t val = atof(node->GetText());
      para->fT_TEOffset[1]= val;
    }else if(strcmp(node->GetNodeName(), "QCal0") == 0){
      Double_t val = atof(node->GetText());
      para->fQCal[0]= val;
    }else if(strcmp(node->GetNodeName(), "QOffset0") == 0){
      Double_t val = atof(node->GetText());
      para->fQOffset[0]= val;
    }else if(strcmp(node->GetNodeName(), "QCal1") == 0){
      Double_t val = atof(node->GetText());
      para->fQCal[1]= val;
    }else if(strcmp(node->GetNodeName(), "QOffset1") == 0){
      Double_t val = atof(node->GetText());
      para->fQOffset[1]= val;
    }else if(strcmp(node->GetNodeName(), "Ignore") == 0){
      Int_t ignore = atoi(node->GetText());
      para->fIgnore=ignore;
    }else if(strcmp(node->GetNodeName(), "t0_geo") == 0){
      t0_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "t0_ch") == 0){
      t0_ch = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "t1_geo") == 0){
      t1_geo = atoi(node->GetText());
    }else if(strcmp(node->GetNodeName(), "t1_ch") == 0){
      t1_ch = atoi(node->GetText());
    }

  }

  Int_t detector_t = NINJAT;


  para->SetMap0(detector_t, t0_geo, t0_ch);
  fNINJAPla0ParaMap->insert(TArtNINJAPlaParaPair(para->GetMap0(), para));
  para->SetMap1(detector_t, t1_geo, t1_ch);
  fNINJAPla1ParaMap->insert(TArtNINJAPlaParaPair(para->GetMap1(), para));

  fIDNINJAPlaParaMap.insert(std::pair<int,TArtNINJAPlaPara*>(para->fID, para));

  return para;
}

const TArtNINJAPlaPara* TArtSAMURAIParameters::FindNINJAPla0Para(const TArtRIDFMap &rmap) const
{
  TArtNINJAPlaParaMap::const_iterator itr = fNINJAPla0ParaMap->find(rmap);
  if(itr != fNINJAPla0ParaMap->end()) 
    return itr->second;
  else 
    return 0;
}

const TArtNINJAPlaPara* TArtSAMURAIParameters::FindNINJAPla1Para(const TArtRIDFMap &rmap) const
{
  TArtNINJAPlaParaMap::const_iterator itr = fNINJAPla1ParaMap->find(rmap);
  if(itr != fNINJAPla1ParaMap->end()) 
    return itr->second;
  else
    return 0;
}

const TArtNINJAPlaPara* TArtSAMURAIParameters::FindNINJAPlaPara(Int_t id) const
{
    std::map<int,TArtNINJAPlaPara*>::const_iterator it = fIDNINJAPlaParaMap.find(id);
    if(it != fIDNINJAPlaParaMap.end()) return it->second;
    else
      return 0;
}

void TArtSAMURAIParameters::PrintListOfNINJAPlaPara() const {
  TArtNINJAPlaParaMap::iterator it0 = fNINJAPla0ParaMap->begin();
  while(it0 !=  fNINJAPla1ParaMap->end()){
    std::cout << *(it0->second) << std::endl;
    ++it0;
  }
  TArtNINJAPlaParaMap::iterator it1 = fNINJAPla1ParaMap->begin();
  while(it1 !=  fNINJAPla1ParaMap->end()){
    std::cout << *(it1->second) << std::endl;
    ++it1;
  }
}







//
// parameter parser for RPTOF
//
//
//__________________________________________________________
TArtRPTOFPlaPara *TArtSAMURAIParameters::ParseRPTOFPlaPara(TXMLNode *node)
{

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Double_t  tul_v775_offset = 0;
  Double_t  tul_v775_ch2ns = 0;
  Double_t  tdr_v775_offset = 0;
  Double_t  tdr_v775_ch2ns = 0;
  Double_t  tul_v1290_offset = 0;
  Double_t  tul_v1290_ch2ns = 0;
  Double_t  tdr_v1290_offset = 0;
  Double_t  tdr_v1290_ch2ns = 0;
  Double_t  qped_ul = 0;
  Double_t  qped_dr = 0;
  Double_t  qcal_ul = 0;
  Double_t  qcal_dr = 0;

  Int_t tul_v775_geo, tul_v775_ch, tdr_v775_geo, tdr_v775_ch;
  Int_t tul_v1290_geo, tul_v1290_ch, tdr_v1290_geo, tdr_v1290_ch;
  Int_t qul_geo, qul_ch, qdr_geo, qdr_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v775_offset") == 0)
        tul_v775_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v775_ch2ns") == 0)
        tul_v775_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v775_offset") == 0)
        tdr_v775_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v775_ch2ns") == 0)
        tdr_v775_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v1290_offset") == 0)
        tul_v1290_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v1290_ch2ns") == 0)
        tul_v1290_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v1290_offset") == 0)
        tdr_v1290_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v1290_ch2ns") == 0)
        tdr_v1290_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_ul") == 0)
        qcal_ul = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_dr") == 0)
        qcal_dr = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_ul") == 0)
        qped_ul = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_dr") == 0)
        qped_dr = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "tul_v775_geo") == 0)
        tul_v775_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v775_ch") == 0)
        tul_v775_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v775_geo") == 0)
        tdr_v775_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v775_ch") == 0)
        tdr_v775_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v1290_geo") == 0)
        tul_v1290_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tul_v1290_ch") == 0)
        tul_v1290_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v1290_geo") == 0)
        tdr_v1290_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdr_v1290_ch") == 0)
        tdr_v1290_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qul_geo") == 0)
        qul_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qul_ch") == 0)
        qul_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qdr_geo") == 0)
        qdr_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qdr_ch") == 0)
        qdr_ch = atoi(node->GetText());
    }
  }

  //  TArtCore::Info(__FILE__,"Reading database for %s RPTOF-Plastic",name.Data());
  TArtRPTOFPlaPara * para =
    new TArtRPTOFPlaPara(id, name, fpl); 
  para->SetTDCV775Para(tul_v775_ch2ns, tdr_v775_ch2ns, tul_v775_offset, tdr_v775_offset);
  para->SetTDCV1290Para(tul_v1290_ch2ns, tdr_v1290_ch2ns, tul_v1290_offset, tdr_v1290_offset);
  para->SetQDCPara(qcal_ul, qcal_dr, qped_ul, qped_dr);

  para->SetTDCV775Map  (tul_v775_geo, tul_v775_ch, tdr_v775_geo, tdr_v775_ch);
  para->SetTDCV1290Map (tul_v1290_geo, tul_v1290_ch, tdr_v1290_geo, tdr_v1290_ch);
  para->SetQDCMap      (qul_geo, qul_ch, qdr_geo, qdr_ch);

  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetTULV775Map()), para));
  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetTDRV775Map()), para));
  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetTULV1290Map()), para));
  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetTDRV1290Map()), para));
  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetQULMap()), para));
  rptof_pmap.insert(std::pair<TArtRIDFMap, TArtRPTOFPlaPara *>(*((TArtRIDFMap *)para->GetQDRMap()), para));

  return para;

}

void TArtSAMURAIParameters::PrintListOfRPTOFPlaPara(){
  TIter next(listOfRPTOFPlaPara);
  while (TArtRPTOFPlaPara *p = (TArtRPTOFPlaPara *)next()) std::cout << *p;
}

const TArtRPTOFPlaPara* TArtSAMURAIParameters::FindRPTOFPlaPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtRPTOFPlaPara *>::const_iterator itr = rptof_pmap.find(*rmap);
  if(itr != rptof_pmap.end()) return (TArtRPTOFPlaPara*)itr->second;
  else return 0;
}

//
// parameter parser for TED
//
//
//__________________________________________________________
TArtTEDCsIPara *TArtSAMURAIParameters::ParseTEDCsIPara(TXMLNode *node)
{

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t row = -1, column = -1;
  Double_t  e_ped = 0;
  Double_t  e_ch2mev = 0;

  Int_t fpl_adc = 13, geo_adc, ch_adc;
  Int_t geo_tdc, ch_tdc;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch2mev") == 0)
        e_ch2mev = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ped") == 0)
        e_ped = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "row") == 0)
        row = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "column") == 0)
        column = atoi(node->GetText());

      if (strcmp(node->GetNodeName(), "e_fpl") == 0)
        fpl_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_geo") == 0)
        geo_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch") == 0)
        ch_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_geo") == 0)
        geo_tdc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_ch") == 0)
        ch_tdc = atoi(node->GetText());
    }
  }

  TArtTEDCsIPara * para = new TArtTEDCsIPara(id, name, fpl, row, column, e_ch2mev, e_ped);
  para->SetMap(fpl_adc, TEDQ, geo_adc, ch_adc,
      fpl_adc, TEDT, geo_tdc, ch_tdc);
  ted_pmap.insert(std::pair<TArtRIDFMap, TArtTEDCsIPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  ted_pmap.insert(std::pair<TArtRIDFMap, TArtTEDCsIPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtSAMURAIParameters::PrintListOfTEDCsIPara(){
  TIter next(listOfTEDCsIPara);
  while (TArtTEDCsIPara *p = (TArtTEDCsIPara *)next()) std::cout << *p;
}

const TArtTEDCsIPara* TArtSAMURAIParameters::FindTEDCsIPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtTEDCsIPara *>::const_iterator itr = ted_pmap.find(*rmap);
  if(itr != ted_pmap.end()) return (TArtTEDCsIPara*)itr->second;
  else return 0;
}

//
// parameter parser for SAMURAI-DC
//
//
//__________________________________________________________

TArtDCHitPara *TArtSAMURAIParameters::ParseDCHitPara(TXMLNode *node, std::map<TArtRIDFMap, TArtDCHitPara *> * map)
{
  Int_t     id=-1;
  TString   name;
  Int_t     fpl=-1;

  Int_t layer=-1;
  Int_t id_plane=-1;
  Int_t wireid=-9999;
  Double_t wirepos=-9999; // wire position in a layer
  Double_t wirez=-9999; // wire z position

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
      if (strcmp(node->GetNodeName(), "wirepos") == 0)
        wirepos = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "wirez") == 0)
        wirez = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tzero_offset") == 0)
        tzero_offset = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "det") == 0)
        det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "geo") == 0)
        geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch") == 0)
        ch = atoi(node->GetText());

    }
  }

  //  TArtCore::Info(__FILE__,"Reading database for %s DC Hit",name.Data());
  TArtDCHitPara * para = new TArtDCHitPara(id, name, fpl, layer, id_plane, wireid, wirepos, wirez, tzero_offset);
  para->SetMap(det, geo, ch);

  map->insert(std::pair<TArtRIDFMap, TArtDCHitPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtSAMURAIParameters::PrintListOfBPCHitPara(){
  TIter next(listOfBPCHitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfBDC1HitPara(){
  TIter next(listOfBDC1HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfBDC2HitPara(){
  TIter next(listOfBDC2HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfPDCHitPara(){
  TIter next(listOfPDCHitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfFDC0HitPara(){
  TIter next(listOfFDC0HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfFDC1HitPara(){
  TIter next(listOfFDC1HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfFDC2HitPara(){
  TIter next(listOfFDC2HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfRPDCHitPara(){
  TIter next(listOfRPDCHitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfS1DC1HitPara(){
  TIter next(listOfS1DC1HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}
void TArtSAMURAIParameters::PrintListOfS1DC2HitPara(){
  TIter next(listOfS1DC2HitPara);
  while (TArtDCHitPara *p = (TArtDCHitPara *)next()) std::cout << *p;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindBPCHitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = bpc_pmap.find(*rmap);
  if(itr != bpc_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindBDC1HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = bdc1_pmap.find(*rmap);
  if(itr != bdc1_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindBDC2HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = bdc2_pmap.find(*rmap);
  if(itr != bdc2_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindPDCHitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = pdc_pmap.find(*rmap);
  if(itr != pdc_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindFDC0HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = fdc0_pmap.find(*rmap);
  if(itr != fdc0_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindFDC1HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = fdc1_pmap.find(*rmap);
  if(itr != fdc1_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindFDC2HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = fdc2_pmap.find(*rmap);
  if(itr != fdc2_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

const TArtDCHitPara* TArtSAMURAIParameters::FindRPDCHitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = rpdc_pmap.find(*rmap);
  if(itr != rpdc_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}
const TArtDCHitPara* TArtSAMURAIParameters::FindS1DC1HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = s1dc1_pmap.find(*rmap);
  if(itr != s1dc1_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}
const TArtDCHitPara* TArtSAMURAIParameters::FindS1DC2HitPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtDCHitPara *>::const_iterator itr = s1dc2_pmap.find(*rmap);
  if(itr != s1dc2_pmap.end()) return (TArtDCHitPara*)itr->second;
  else return 0;
}

//_____________________________________________________________________
