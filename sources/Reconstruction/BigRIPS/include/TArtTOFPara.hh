#ifndef _TARTTOFPARA_H_
#define _TARTTOFPARA_H_

#include <TNamed.h>

class TArtTOFPara : public TNamed {
  
  /*public:
  TArtTOFPara(){}
  TArtTOFPara(char* name, char *upla, char *dpla, Double_t toffset=0, Int_t mfpl=-1) :
    offset(toffset) {
    SetName(name);
    upstream_plname = upla;
    downstream_plname = dpla;
    middle_fpl = mfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla);
    detname = dname;
  }
  TArtTOFPara(char *upla, char *dpla, Double_t toffset=0, Int_t mfpl=-1) :
    offset(toffset) {
    upstream_plname = upla;
    downstream_plname = dpla;
    middle_fpl = mfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla);
    detname = dname;
    } //original*/

  public:
  TArtTOFPara(){}
  TArtTOFPara(char* name, char *upla, char *dpla1, char *dpla2, Double_t toffset=0, Int_t mfpl=-1) :
    offset(toffset) {
    SetName(name);
    upstream_plname = upla;
    downstream_plname1 = dpla1;
    downstream_plname2 = dpla2;
    middle_fpl = mfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla1,dpla2);
    detname = dname;
  }
  TArtTOFPara(char *upla, char *dpla1, char *dpla2, Double_t toffset=0, Int_t mfpl=-1) :
    offset(toffset) {
    upstream_plname = upla;
    downstream_plname1 = dpla1;
    downstream_plname2 = dpla2;
    middle_fpl = mfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla1,dpla2);
    detname = dname;
  }
  
  ~TArtTOFPara(){}

  void SetUpstreamPlName   (char *v) { upstream_plname = v; }
  //void SetDownstreamPlName (char *v) { downstream_plname = v; } //original
  void SetDownstreamPlName1 (char *v) { downstream_plname1 = v; } //new
  void SetDownstreamPlName2 (char *v) { downstream_plname2 = v; } //new
  void SetOffset        (Double_t v) { offset = v; }
  void SetMiddleFpl        (Int_t v) { middle_fpl = v; }

  TString * GetDetectorName      () { return &detname; }
  TString * GetUpstreamPlName() { return &upstream_plname; }
  //TString * GetDownstreamPlName() { return &downstream_plname; } //original
  TString * GetDownstreamPlName1() { return &downstream_plname1; } //new
  TString * GetDownstreamPlName2() { return &downstream_plname2; } //new
  Double_t GetOffset() const { return offset; }
  Int_t GetMiddleFpl() const { return middle_fpl; }

 private:
  TString   detname;
  TString   upstream_plname;
  //TString   downstream_plname; //original
  TString   downstream_plname1; //new
  TString   downstream_plname2; //new
  Double_t  offset;
  Int_t     middle_fpl;

  ClassDef(TArtTOFPara,1);

};

#endif
