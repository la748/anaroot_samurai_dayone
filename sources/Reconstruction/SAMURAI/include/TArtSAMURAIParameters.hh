#ifndef _TARTNEBULAPARAMETERS_H_
#define _TARTNEBULAPARAMETERS_H_

#include <TNamed.h>
#include <TList.h>

#include <map>

class TXMLNode;
class TArtRIDFMap;
class TArtTacquilaMap;
class TArtStoreManager;
class TArtDCHitPara;
class TArtHODPlaPara;
class TArtRPTOFPlaPara;
class TArtTEDCsIPara;
class TArtNEBULAPlaPara;

class TArtNeuLANDPlaPara;
class TArtTacquilaPara;


typedef std::map<TArtRIDFMap, const TArtNEBULAPlaPara*> TArtNEBULAPlaParaMap;
typedef std::pair<TArtRIDFMap, const TArtNEBULAPlaPara*> TArtNEBULAPlaParaPair;

class TArtHIMEPlaPara;
typedef std::map<TArtRIDFMap, const TArtHIMEPlaPara*> TArtHIMEPlaParaMap;
typedef std::pair<TArtRIDFMap, const TArtHIMEPlaPara*> TArtHIMEPlaParaPair;

class TArtNEBULAHPCPara;
typedef std::map<TArtRIDFMap, const TArtNEBULAHPCPara*> TArtNEBULAHPCParaMap;
typedef std::pair<TArtRIDFMap, const TArtNEBULAHPCPara*> TArtNEBULAHPCParaPair;

typedef std::map<TArtTacquilaMap, const TArtNeuLANDPlaPara*> TArtNeuLANDPlaParaMap;
typedef std::pair<TArtTacquilaMap, const TArtNeuLANDPlaPara*> TArtNeuLANDPlaParaPair;
typedef std::map<TArtTacquilaMap, const TArtTacquilaPara*> TArtTacquilaParaMap;
typedef std::pair<TArtTacquilaMap, const TArtTacquilaPara*> TArtTacquilaParaPair;

class TArtNINJAPlaPara;
typedef std::map<TArtRIDFMap, const TArtNINJAPlaPara*> TArtNINJAPlaParaMap;
typedef std::pair<TArtRIDFMap, const TArtNINJAPlaPara*> TArtNINJAPlaParaPair;

class TArtSAMURAIParameters : public TNamed
{
public: 
  TArtSAMURAIParameters(const char* name = "SAMURAIParameters", 
		       const char* title = "SAMURAIParameters");
  ~TArtSAMURAIParameters();

  // singleton. If you want to use multiple set of parameters, use norminal way.
  static TArtSAMURAIParameters* Instance(const char* name = "SAMURAIParameters", 
					 const char* title = "SAMURAIParameters");
  static void Delete();

  Bool_t LoadParameter(const char *xmlfile);
  Bool_t LoadNeuLANDTCal(const char *csvfile);
  Bool_t LoadTCal(const char *cfile){return LoadNeuLANDTCal(cfile);}

  void PrintListOfHODPlaPara ();
  void PrintListOfRPTOFPlaPara ();
  void PrintListOfTEDCsIPara ();
  void PrintListOfBPCHitPara ();
  void PrintListOfBDC1HitPara();
  void PrintListOfBDC2HitPara();
  void PrintListOfPDCHitPara();
  void PrintListOfFDC0HitPara();
  void PrintListOfFDC1HitPara();
  void PrintListOfFDC2HitPara();
  void PrintListOfRPDCHitPara();
  void PrintListOfS1DC1HitPara();
  void PrintListOfS1DC2HitPara();
  void PrintListOfNEBULAPlaPara() const;
  void PrintListOfHIMEPlaPara() const;
  void PrintListOfNeuLANDPlaPara() const;
  void PrintListOfNeuLANDVETOPlaPara() const;
  //  void PrintListOfTacquilaPara() const;
  void PrintListOfNINJAPlaPara() const;

  TList* GetListOfHODPlaPara () {return listOfHODPlaPara; }
  TList* GetListOfRPTOFPlaPara () {return listOfRPTOFPlaPara; }
  TList* GetListOfTEDCsIPara () {return listOfTEDCsIPara; }
  TList* GetListOfBPCHitPara () {return listOfBPCHitPara; }
  TList* GetListOfBDC1HitPara() {return listOfBDC1HitPara; }
  TList* GetListOfBDC2HitPara() {return listOfBDC2HitPara; }
  TList* GetListOfPDCHitPara() {return listOfPDCHitPara; }
  TList* GetListOfFDC0HitPara() {return listOfFDC0HitPara; }
  TList* GetListOfFDC1HitPara() {return listOfFDC1HitPara; }
  TList* GetListOfFDC2HitPara() {return listOfFDC2HitPara; }
  TList* GetListOfRPDCHitPara() {return listOfBDC1HitPara; }
  TList* GetListOfS1DC1HitPara() {return listOfS1DC1HitPara; }
  TList* GetListOfS1DC2HitPara() {return listOfS1DC2HitPara; }

  const TArtHODPlaPara* FindHODPlaPara   (TArtRIDFMap *rmap) const;
  const TArtRPTOFPlaPara* FindRPTOFPlaPara   (TArtRIDFMap *rmap) const;
  const TArtTEDCsIPara* FindTEDCsIPara   (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindBPCHitPara   (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindBDC1HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindBDC2HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindPDCHitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindFDC0HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindFDC1HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindFDC2HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindRPDCHitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindS1DC1HitPara  (TArtRIDFMap *rmap) const;
  const TArtDCHitPara * FindS1DC2HitPara  (TArtRIDFMap *rmap) const;

  const TArtNEBULAPlaPara* FindNEBULAPlaUPara(const TArtRIDFMap &rmap) const;
  const TArtNEBULAPlaPara* FindNEBULAPlaDPara(const TArtRIDFMap &rmap) const;
  const TArtNEBULAPlaPara* FindNEBULAPlaPara(Int_t id) const;//find by id

  const TArtHIMEPlaPara* FindHIMEPlaUPara(const TArtRIDFMap &rmap) const;
  const TArtHIMEPlaPara* FindHIMEPlaDPara(const TArtRIDFMap &rmap) const;
  const TArtHIMEPlaPara* FindHIMEPlaPara(Int_t id) const;//find by id

  const TArtNEBULAHPCPara* FindNEBULAHPCPara(const TArtRIDFMap &rmap) const;
  const TArtNEBULAHPCPara* FindNEBULAHPCPara(Int_t id) const;//find by id

  const TArtNeuLANDPlaPara* FindNeuLANDPlaPara(const TArtTacquilaMap *rmap) const;
  const TArtNeuLANDPlaPara* FindNeuLANDPlaPara(Int_t id) const;//find by id

  const TArtTacquilaPara* FindTacquilaPara(const TArtTacquilaMap *rmap) const;
  const TArtTacquilaPara* FindTacquilaPara(Int_t id) const;//find by id
  const TArtNEBULAPlaPara* FindNeuLANDVETOPlaUPara(const TArtRIDFMap &rmap) const;
  const TArtNEBULAPlaPara* FindNeuLANDVETOPlaDPara(const TArtRIDFMap &rmap) const;
  const TArtNEBULAPlaPara* FindNeuLANDVETOPlaPara(Int_t id) const;//find by id

//  const TArtHODPlaPara* FindNINJAPlaPara   (TArtRIDFMap *rmap) const;

  const TArtNINJAPlaPara* FindNINJAPla1Para(const TArtRIDFMap &rmap) const;
  const TArtNINJAPlaPara* FindNINJAPla0Para(const TArtRIDFMap &rmap) const;
  const TArtNINJAPlaPara* FindNINJAPlaPara(Int_t id) const;//find by id


private:
  void ParseParaList(TXMLNode* node);
  TArtDCHitPara* ParseDCHitPara(TXMLNode* node, std::map<TArtRIDFMap, TArtDCHitPara *> * map);
  TArtHODPlaPara*    ParseHODPlaPara(TXMLNode* node);
  TArtRPTOFPlaPara*  ParseRPTOFPlaPara(TXMLNode* node);
  TArtTEDCsIPara*    ParseTEDCsIPara(TXMLNode* node);
  TArtNEBULAPlaPara* ParseNEBULAPlaPara(TXMLNode* node);
  TArtHIMEPlaPara*   ParseHIMEPlaPara(TXMLNode* node);
  TArtNEBULAHPCPara* ParseNEBULAHPCPara(TXMLNode* node);
  TArtNeuLANDPlaPara* ParseNeuLANDPlaPara(TXMLNode* node);
  TArtTacquilaPara*  ParseTacquilaPara(TXMLNode* node);
  TArtNEBULAPlaPara* ParseNeuLANDVETOPlaPara(TXMLNode* node);
  TArtNINJAPlaPara*   ParseNINJAPlaPara(TXMLNode* node);

  std::map<TArtRIDFMap, TArtHODPlaPara *> hod_pmap;
  std::map<TArtRIDFMap, TArtRPTOFPlaPara *> rptof_pmap;
  std::map<TArtRIDFMap, TArtTEDCsIPara *> ted_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> bpc_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> bdc1_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> bdc2_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> pdc_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> fdc0_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> fdc1_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> fdc2_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> rpdc_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> s1dc1_pmap;
  std::map<TArtRIDFMap, TArtDCHitPara *> s1dc2_pmap;

  TArtNEBULAPlaParaMap  *fNEBULAPlaUParaMap, *fNEBULAPlaDParaMap;
  TArtHIMEPlaParaMap    *fHIMEPlaUParaMap, *fHIMEPlaDParaMap;
  TArtNEBULAHPCParaMap  *fNEBULAHPCParaMap;
  TArtNeuLANDPlaParaMap *fNeuLANDPlaParaMap;
  TArtTacquilaParaMap   *fTacquilaParaMap;
  TArtNEBULAPlaParaMap  *fNeuLANDVETOPlaUParaMap, *fNeuLANDVETOPlaDParaMap;

  TArtNINJAPlaParaMap    *fNINJAPla0ParaMap, *fNINJAPla1ParaMap;

  // list of parameters 
  TList *listOfHODPlaPara;
  TList *listOfRPTOFPlaPara;
  TList *listOfTEDCsIPara;
  TList *listOfBPCHitPara;
  TList *listOfBDC1HitPara;
  TList *listOfBDC2HitPara;
  TList *listOfPDCHitPara;
  TList *listOfFDC0HitPara;
  TList *listOfFDC1HitPara;
  TList *listOfFDC2HitPara;
  TList *listOfRPDCHitPara;
  TList *listOfS1DC1HitPara;
  TList *listOfS1DC2HitPara;

  // for optimization
  std::map<int, TArtNEBULAPlaPara*> fIDNEBULAPlaParaMap;
  std::map<int, TArtHIMEPlaPara*>   fIDHIMEPlaParaMap;
  std::map<int, TArtNEBULAHPCPara*> fIDNEBULAHPCParaMap;
  std::map<int, TArtNeuLANDPlaPara*> fIDNeuLANDPlaParaMap;
  std::map<int, TArtTacquilaPara*> fIDTacquilaParaMap;
  std::map<int, TArtNEBULAPlaPara*> fIDNeuLANDVETOPlaParaMap;
  std::map<int, TArtNINJAPlaPara*>   fIDNINJAPlaParaMap;

  TArtStoreManager* fStoreManager;
  static TArtSAMURAIParameters* fSAMURAIParameters;

  ClassDef(TArtSAMURAIParameters,1);
};

#endif
