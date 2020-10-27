#ifndef TARTRAWDATAOBJECT_H
#define TARTRAWDATAOBJECT_H

#include "TNamed.h"

class TArtRawDataObject : public TNamed
{
 public: 
  TArtRawDataObject();
  TArtRawDataObject(int ingeo, 
		    int inch,
		    unsigned int inval){
    geo=ingeo;
    channel=inch;
    value=inval;

    edge=-1; categoryid=-1; detectorid=-1; datatype=-1;
  }

  virtual ~TArtRawDataObject(){};

  virtual void SetGeo(const int ingeo){geo = ingeo;} 
  virtual void SetCh(const int inch){channel = inch;} 
  virtual void SetVal(const unsigned int inval){value = inval;} 
  virtual void SetEdge(const int inedge){edge = inedge;} 

  virtual int GetGeo(){return geo;}
  virtual int GetCh(){return channel;}
  virtual unsigned int GetVal(){return value;}
  virtual int GetEdge(){return edge;}

  virtual void SetCategoryID(const int incat){categoryid = incat;} 
  virtual void SetDetectorID(const int indet){detectorid = indet;} 
  virtual void SetDatatypeID(const int inid){datatype = inid;} 

  virtual int GetCategoryID(){return categoryid;} 
  virtual int GetDetectorID(){return detectorid;} 
  virtual int GetDatatypeID(){return datatype;} 

  virtual void Clear();

 protected:

  // geometry code slot # in a crate
  int geo;

  // channel in the (CAMAC/VME) module
  int channel;  

  // data value
  unsigned int value;

  // edge: -1:undefined, 0: rising, 1: falling
  int edge;

  // for mapped analysis, each parameter will be given if you define map. 
  // detector id, such as PPAC, Plastic, and IC
  int categoryid;
  // detector #
  int detectorid;
  // channel # of a detector
  int datatype;

  ClassDef(TArtRawDataObject,1);

};

#endif
