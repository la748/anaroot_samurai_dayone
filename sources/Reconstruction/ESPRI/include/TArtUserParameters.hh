#ifndef _TARTUSERPARAMETERS_H_
#define _TARTUSERPARAMETERS_H_

#include <TNamed.h>
#include <TString.h>

#include <map>

class TXMLNode;
class TArtStoreManager;

class TArtUserParameters : public TNamed
{
public: 
  TArtUserParameters(const char* name = "UserParameters", 
		       const char* title = "UserParameters");
  ~TArtUserParameters();
  static TArtUserParameters* Instance(const char* name = "UserParameters", 
					 const char* title = "UserParameters");
  static void Delete();
  Bool_t   LoadParameter(const char *xmlfile);
  void     PrintPara();
  Int_t    FindIntPara(const TString str) const;
  Double_t FindDoublePara(const TString str) const;
  TString  FindStringPara(const TString str) const;
  TString  FindParaComment(const TString str) const;
  void     SetPara(const TString name, const TString type, 
		   const TString value, const TString comment="");
  void     SetIntPara(const TString name, const Int_t value, 
		      const TString comment="");
  void     SetDoublePara(const TString name, const Double_t value, 
			 const TString comment="");
  void     SetStringPara(const TString name, const TString value, 
			 const TString comment="");

private:
  void ParseParaList(TXMLNode* node);
  void ParsePara(TXMLNode *node);

  std::map<TString, Int_t>    imap;// map for int parameters
  std::map<TString, Double_t> dmap;// map for double parameters
  std::map<TString, TString>  smap;// map for string parameters
  std::map<TString, TString>  cmap;// map for comments

  TArtStoreManager* fStoreManager;
  static TArtUserParameters* fParameters;

  ClassDef(TArtUserParameters,1);
};

#endif
