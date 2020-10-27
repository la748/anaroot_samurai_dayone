#ifndef _TARTSILICONSPARAMETERS_H_
#define _TARTSILICONSPARAMETERS_H_

#include <TROOT.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtSILICONSPara;
class TArtRIDFMap;

class TArtSILICONSParameters : public TNamed
{
 public: 

  TArtSILICONSParameters(const char* name = "SILICONSParameters",  
		     const char* title = "SILICONSParameters");
  ~TArtSILICONSParameters();
  static TArtSILICONSParameters* Instance(const char* name = "SILICONSParameters", 
					const char* title = "SILICONSParameters");
  static void Delete();

  Bool_t LoadParameter(char *xmlfile);

  void PrintListOfPara();
  TList* GetListOfPara() {return listOfPara; }
  Int_t GetNumPara() {return listOfPara->GetEntries(); }
  TArtSILICONSPara * GetSILICONSPara(TArtRIDFMap *rmap);

 
 protected:

  std::map<TArtRIDFMap, TArtSILICONSPara *> pmap;
  std::map<TArtRIDFMap, TArtSILICONSPara *>::iterator itrEnd;
  std::map<TArtRIDFMap, TArtSILICONSPara *>::iterator itr;

  TList *listOfPara;

  void ParseParaList(TXMLNode *node);
  TArtSILICONSPara * ParseSILICONSPara(TXMLNode *node);

  static TArtSILICONSParameters* fSILICONSParameters;

  ClassDef(TArtSILICONSParameters,1);

};

#endif
