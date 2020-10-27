#include "TArtUserParameters.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>

using namespace std;
TArtUserParameters* TArtUserParameters::fParameters = 0;

//__________________________________________________________
TArtUserParameters::TArtUserParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating parameters...");
  fStoreManager = TArtStoreManager::Instance();
  fStoreManager->AddParameters(this);
}
//__________________________________________________________
TArtUserParameters::~TArtUserParameters() 
{
  fParameters = 0;
}
//__________________________________________________________
TArtUserParameters* TArtUserParameters::Instance(const char* name, const char* title)
{
  if(!fParameters) fParameters = new TArtUserParameters(name, title);
  return fParameters;
}    

//__________________________________________________________
void TArtUserParameters::Delete()
{
  delete fParameters;
  fParameters = 0;  
}
//__________________________________________________________
Bool_t TArtUserParameters::LoadParameter(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser domParser;
  domParser.SetValidate(false);
  Int_t parsecode = domParser.ParseFile(xmlfile);
  if(parsecode < 0){
    cerr << domParser.GetParseCodeMessage(parsecode) << endl;
    return false;
  }
  TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  return true;
}
//__________________________________________________________
void TArtUserParameters::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "USERPARAMETER") == 0)
      ParsePara(node->GetChildren());
    else 
      TArtCore::Info(__FILE__,"Unknown Parameter %s.",node->GetNodeName());
  }
}
//__________________________________________________________
void TArtUserParameters::ParsePara(TXMLNode *node)
{
  TString name;
  TString type;
  TString value;
  TString comment;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node

      if (strcmp(node->GetNodeName(), "name") == 0)
        name = node->GetText();
      else if (strcmp(node->GetNodeName(), "type") == 0)
        type = node->GetText();
      else if (strcmp(node->GetNodeName(), "value") == 0)
        value = node->GetText();
      else if (strcmp(node->GetNodeName(), "comment") == 0)
        comment = node->GetText();
    }
  }
  SetPara(name, type, value, comment);
}
//_____________________________________________________________________
void TArtUserParameters::PrintPara(){

  map<TString, Int_t>::iterator i_itr;
  TArtCore::Info(__FILE__,"Parameters (Int_t)");
  for (i_itr=imap.begin();i_itr!=imap.end();i_itr++){
    cout<<i_itr->first<<" = "<<i_itr->second<<" ("
	<<FindParaComment(i_itr->first).Data()<<")"<<endl;
  }

  map<TString, Double_t>::iterator d_itr;
  TArtCore::Info(__FILE__,"Parameters (Double_t)");
  for (d_itr=dmap.begin();d_itr!=dmap.end();d_itr++){
    cout<<d_itr->first<<" = "<<d_itr->second<<" ("
	<<FindParaComment(d_itr->first).Data()<<")"<<endl;
  }

  map<TString, TString>::iterator s_itr;
  TArtCore::Info(__FILE__,"Parameters (TString)");
  for (s_itr=smap.begin();s_itr!=smap.end();s_itr++){
    cout<<s_itr->first<<" = "<<s_itr->second<<" ("
	<<FindParaComment(s_itr->first).Data()<<")"<<endl;
  }
}
//_____________________________________________________________________
Int_t TArtUserParameters::FindIntPara(const TString str) const
{
  map<TString, Int_t>::const_iterator itr = imap.find(str);
  if(itr != imap.end()) return itr->second;
  else return 0;
}
//_____________________________________________________________________
Double_t TArtUserParameters::FindDoublePara(const TString str) const
{
  map<TString, Double_t>::const_iterator itr = dmap.find(str);
  if(itr != dmap.end()) return itr->second;
  else return 0;
}
//_____________________________________________________________________
TString TArtUserParameters::FindStringPara(const TString str) const
{
  map<TString, TString>::const_iterator itr = smap.find(str);
  if(itr != smap.end()) return itr->second;
  else return "";
}
//_____________________________________________________________________
TString TArtUserParameters::FindParaComment(const TString str) const
{
  map<TString, TString>::const_iterator itr = cmap.find(str);
  if(itr != cmap.end()) return itr->second;
  else return "";
}
//_____________________________________________________________________
void TArtUserParameters::SetPara(const TString name, const TString type, 
				 const TString value, const TString comment){

  if (type == "int" || type=="Int_t"){
    if (value.IsDigit())  SetIntPara(name,value.Atoi(),comment);
    else TArtCore::Info(__FILE__,"type %s is invalid for %s (value = %s).",
			type.Data(),name.Data(),value.Data());
  }

  else if (type == "double" || type=="Double_t"){
    if (value.IsFloat()) SetDoublePara(name,value.Atof(),comment);
    else TArtCore::Info(__FILE__,"type %s is invalid for %s (value = %s).",
			type.Data(),name.Data(),value.Data());
  }

  else if (type == "string" || type=="TString")
    SetStringPara(name,value,comment);

  else
    TArtCore::Info(__FILE__,"Unknown parameter type %s for %s.",type.Data(),name.Data());
}
//_____________________________________________________________________
void TArtUserParameters::SetIntPara(const TString name, const Int_t value, 
				    const TString comment){
  imap.insert(pair<TString,Int_t>(name,value) );
  cmap.insert(pair<TString,TString>(name,comment));
}
//_____________________________________________________________________
void TArtUserParameters::SetDoublePara(const TString name, const Double_t value, 
				       const TString comment){
  dmap.insert(pair<TString,Double_t>(name,value) );
  cmap.insert(pair<TString,TString>(name,comment));
}
//_____________________________________________________________________
void TArtUserParameters::SetStringPara(const TString name, const TString value, 
				       const TString comment){
  smap.insert(pair<TString,TString>(name,value) );
  cmap.insert(pair<TString,TString>(name,comment));
}
//_____________________________________________________________________
