#ifndef _TARTRUNINFO_H_
#define _TARTRUNINFO_H_

#include "TNamed.h"
#include "TString.h"

class TArtRunInfo : public TNamed {

 public:
  TArtRunInfo(){Clear();}
  virtual ~TArtRunInfo(){}
  virtual void Clear(){
    runname.Clear();
    runnumber.Clear();
    starttime.Clear();
    stoptime.Clear();
    date.Clear();
    rev.Clear();
    header.Clear();
    ender.Clear();
  }

  virtual void SetRunName  (char *val){runname = val;}
  virtual void SetRunNumber(char *val){runnumber = val;}
  virtual void SetStartTime(char *val){starttime = val;}
  virtual void SetStopTime (char *val){stoptime = val;}
  virtual void SetDate     (char *val){date = val;}
  virtual void SetHeader   (char *val){header = val;}
  virtual void SetEnder    (char *val){ender = val;}

  virtual TString * GetRunName  (){return &runname;}
  virtual TString * GetRunNumber(){return &runnumber;}
  virtual TString * GetStartTime(){return &starttime;}
  virtual TString * GetStopTime (){return &stoptime;}
  virtual TString * GetDate     (){return &date;}
  virtual TString * GetHeader   (){return &header;}
  virtual TString * GetEnder    (){return &ender;}

 private:

  TString runname;
  TString runnumber;
  TString starttime;
  TString stoptime;
  TString date;
  TString rev;
  TString header;
  TString ender;

  ClassDef(TArtRunInfo,1);

};

#endif

