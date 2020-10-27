#ifndef TARTRAWTACQUILADATAOBJECT_H
#define TARTRAWTACQUILADATAOBJECT_H

#include "TNamed.h"
#include "TArtCore.hh"

class TArtRawTacquilaDataObject : public TNamed
{
 public: 
  TArtRawTacquilaDataObject(){Clear();}
  TArtRawTacquilaDataObject(int insam, int ingtb, int inmod, int inch, int inadc, int incycle, int intac){
    sam = insam; gtb = ingtb; module = inmod; channel = inch; adc = inadc; cycle = incycle; tac = intac;}
  virtual ~TArtRawTacquilaDataObject(){}

  virtual void SetSam(const int val){sam=val;}
  virtual void SetGtb(const int val){gtb=val;}
  virtual void SetModule(const int val){module=val;}
  virtual void SetChannel(const int val){channel=val;}
  virtual void SetAdc(const int val){adc=val;}
  virtual void SetCycle(const int val){cycle=val;}
  virtual void SetTac(const int val){tac=val;}

  virtual int GetSam(){return sam;}
  virtual int GetGtb(){return gtb;}
  virtual int GetModule(){return module;}
  virtual int GetChannel(){return channel;}
  virtual int GetAdc(){return adc;}
  virtual int GetCycle(){return cycle;}
  virtual int GetTac(){return tac;}

  virtual void Clear() {
    sam = -1;
    gtb = -1;
    module = -1;
    channel = -1;
    adc = -1;
    cycle = -1;
    tac = -1;
  }

  virtual void DumpVal(){
    TArtCore::Info(__FILE__,"sam=%d gtb=%d mod=%d, ch=%d, adc=%d, cycle=%d, tac=%d",sam,gtb,module,channel,adc,cycle,tac);
  }

 protected:

  // sam
  int sam;  

  // gtb
  int gtb;  

  // module;
  int module;

  // channel 
  int channel;  

  // adc
  int adc;

  // cycle
  int cycle;

  // tac
  int tac;

  ClassDef(TArtRawTacquilaDataObject,1);

};

#endif
