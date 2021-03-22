#ifndef _TARTTOF_H_
#define _TARTTOF_H_

#include "TArtDataObject.hh"
#include "TMath.h"

class TArtTOF : public TArtDataObject
{
 public: 

  TArtTOF(){Clear();
    clight = 299.792458; // mm/nsec
    length=-1;ulength=-1;dlength=-1;time_offset=0;
  }
  virtual ~TArtTOF(){}
  virtual void Clear(){tof=-1;SetDataState(0);}

  virtual void SetTOF(Double_t val){tof=val;}
  virtual void SetLength(Double_t val){length=val;}
  virtual void SetUpStreamLength(Double_t val){ulength=val;}
  virtual void SetDownStreamLength(Double_t val){dlength=val;}

  virtual Double_t GetTOF(){ return tof; }
  virtual Double_t GetBeta(){ return tof == 0 ? 0 : length/tof/clight; }
  virtual Double_t GetGamma(){ return tof == 0 ? 1 : 1/TMath::Sqrt(1-GetBeta()*GetBeta()); }
  virtual Double_t GetLength(){ return length; }
  virtual Double_t GetUpStreamLength(){ return ulength; }
  virtual Double_t GetDownStreamLength(){ return dlength; }

  // calibration parameter
  virtual void SetUpstreamPlaName(char * val){upstream_plname=val;}
  virtual void SetUpstreamPlaFpl(Int_t val){upstream_plfpl=val;}
  //virtual void SetDownstreamPlaName(char * val){downstream_plname=val;} //original
  virtual void SetDownstreamPlaName1(char * val){downstream_plname1=val;} //new
  virtual void SetDownstreamPlaName2(char * val){downstream_plname2=val;} //new
  //virtual void SetDownstreamPlaFpl(Int_t val){downstream_plfpl=val;} //original
  virtual void SetDownstreamPlaFpl1(Int_t val){downstream_plfpl1=val;} //new
  virtual void SetDownstreamPlaFpl2(Int_t val){downstream_plfpl2=val;} //new
  virtual void SetTimeOffset(Double_t val){time_offset=val;}
  virtual TString * GetUpstreamPlaName(){return &upstream_plname;}
  //virtual TString * GetDownstreamPlaName(){return &downstream_plname;} //original
  virtual TString * GetDownstreamPlaName1(){return &downstream_plname1;} //new
  virtual TString * GetDownstreamPlaName2(){return &downstream_plname2;} //new
  virtual Int_t GetUpstreamPlaFpl(){return upstream_plfpl;}
  //virtual Int_t GetDownstreamPlaFpl(){return downstream_plfpl;} //original
  virtual Int_t GetDownstreamPlaFpl1(){return downstream_plfpl1;} //new
  virtual Int_t GetDownstreamPlaFpl2(){return downstream_plfpl2;} //new
  virtual Double_t GetTimeOffset(){return time_offset;}

 private:

  // time of flight
  Double_t tof;
  // speed of light
  Double_t clight;
  // length for tof
  Double_t length;
  // upstream/downstream length of tof. used for two brho combined analysis. kind of degrader is put at middle fpl.
  Double_t ulength;
  Double_t dlength;

  TString   upstream_plname;
  //TString   downstream_plname; //original
  TString   downstream_plname1; //new
  TString   downstream_plname2; //original
  Int_t     upstream_plfpl;
  //Int_t     downstream_plfpl; //original
  Int_t     downstream_plfpl1; //new
  Int_t     downstream_plfpl2; //new
  Double_t  time_offset;

  ClassDef(TArtTOF,1);

};

#endif
