#ifndef _TARTESPRIPARAMETERS_H_
#define _TARTESPRIPARAMETERS_H_

#include <TNamed.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtRIDFMap;
class TArtStoreManager;
class TArtTDCHitPara;
class TArtRNaIPara;
class TArtFADCPara;
class TArtPlasPara;

class TArtESPRIParameters : public TNamed
{
public: 
  TArtESPRIParameters(const char* name = "ESPRIParameters", 
		       const char* title = "ESPRIParameters");
  ~TArtESPRIParameters();
  // singleton. If you want to use multiple set of parameters, use norminal way.
  static TArtESPRIParameters* Instance(const char* name = "ESPRIParameters", 
					 const char* title = "ESPRIParameters");
  static void Delete();

  Bool_t LoadParameter(const char *xmlfile);

  void PrintListOfRNaIPara ();
  void PrintListOfTDCHitPara();
  void PrintListOfFADCPara();
  void PrintListOfPlasPara();

  TList* GetListOfRNaIPara () {return listOfRNaIPara; }
  TList* GetListOfTDCHitPara() {return listOfTDCHitPara; }
  TList* GetListOfFADCPara() {return listOfFADCPara; }
  TList* GetListOfPlasPara() {return listOfPlasPara; }

  const TArtRNaIPara* FindRNaIPara   (TArtRIDFMap *rmap) const;
  const TArtTDCHitPara* FindTDCHitPara  (TArtRIDFMap *rmap) const;
  const TArtFADCPara* FindFADCPara  (TArtRIDFMap *rmap) const;
  const TArtPlasPara* FindPlasPara  (TArtRIDFMap *rmap) const;

private:
  //protected:
  void ParseParaList(TXMLNode* node);
  TArtRNaIPara*  ParseRNaIPara(TXMLNode* node);
  TArtTDCHitPara* ParseTDCHitPara(TXMLNode* node, std::map<TArtRIDFMap, TArtTDCHitPara *> * map);
  TArtFADCPara* ParseFADCPara(TXMLNode* node, std::map<TArtRIDFMap, TArtFADCPara *> * map);
  TArtPlasPara* ParsePlasPara(TXMLNode* node, std::map<TArtRIDFMap, TArtPlasPara *> * map);

  std::map<TArtRIDFMap, TArtRNaIPara *> rnai_pmap;
  std::map<TArtRIDFMap, TArtTDCHitPara *> tdc_pmap;
  std::map<TArtRIDFMap, TArtFADCPara *> fadc_pmap;
  std::map<TArtRIDFMap, TArtPlasPara *> plas_pmap;

  // list of parameters 
  TList *listOfRNaIPara;
  TList *listOfTDCHitPara;
  TList *listOfFADCPara;
  TList *listOfPlasPara;

  TArtStoreManager* fStoreManager;
  static TArtESPRIParameters* fESPRIParameters;

  ClassDef(TArtESPRIParameters,1);
};

#endif
