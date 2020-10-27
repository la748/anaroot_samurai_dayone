#ifndef TARTRAWFADCDATAOBJECT_H
#define TARTRAWFADCDATAOBJECT_H

#include "TNamed.h"
#include "TArtCore.hh"

class TArtRawFADCDataObject : public TNamed
{
 public: 
  TArtRawFADCDataObject(){}
  TArtRawFADCDataObject(int inch) : channel(inch){}
  virtual ~TArtRawFADCDataObject(){};

  virtual void SetCh (const int inch){channel = inch;} 
  virtual void SetGeo(const int ingeo){geo = ingeo;} 
  virtual void AddVal(const int tb, const int val){
    timebacket.push_back(tb);
    value.push_back(val);
  } 

  virtual int GetCh(){return channel;}
  virtual int GetGeo(){return geo;}
  virtual int GetNData(){return timebacket.size();}
  virtual int GetTimeBacket(int i){return timebacket[i];}
  virtual int GetVal(int i){return value[i];}
  virtual void DumpVal(){
    for(int i=0;i<(int)timebacket.size();i++) 
      TArtCore::Info(__FILE__,"geo=%d ch=%d tb=%d : %d",geo,channel,timebacket[i],value[i]);
  }

 protected:

  // geo
  int geo;  

  // channel in the (CAMAC/VME) module
  int channel;  

  // time backet
  std::vector <int> timebacket;

  // data value
  std::vector <int> value;

  ClassDef(TArtRawFADCDataObject,1);

};

#endif
