#ifndef _TARTRNAIPARA_H_
#define _TARTRNAIPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtRNaIPara : public TNamed {

 public:
  TArtRNaIPara(){}
  TArtRNaIPara(Int_t i, TString n, Int_t f, Int_t l, Double_t tc, Double_t to, Double_t qc, Double_t qp) :
    id(i), detname(n), fpl(f), layer(l), tcal(tc), toff(to), qcal(qc), qped(qp){ }
  ~TArtRNaIPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, Int_t l, Double_t tc, Double_t to, Double_t qc, Double_t qp) { id=i; detname=n; fpl=f; layer=l; tcal=tc; toff=to; qcal=qc; qped=qp;}

  void SetMap(Int_t tgeo, Int_t tch, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(fpl,NAIE,qgeo,qch); //ESPRI2016
    map_tdc.SetMap(fpl,NAIT,tgeo,tch); //ESPRI2016
    //map_adc.SetMap(0,5,qgeo,qch); //ESRPI2013
    //map_tdc.SetMap(0,17,tgeo,tch);//ESRPI2013
    //cout << "NaI is mapped to " << map_adc.GetMapInfo() << " " << map_adc.GetName() << endl;
    //cout << "NaI is mapped to " << map_tdc.GetMapInfo() << " " << map_tdc.GetName() << endl;
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLayer() const { return layer; }
  Double_t GetTCal() const { return tcal; } 
  Double_t GetTOffset() const { return toff; } 
  Double_t GetQCal() const { return qcal; } 
  Double_t GetQPed() const { return qped; } 

  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }
  TArtRIDFMap * GetADCMap() { return &map_adc;  }

  friend std::ostream & operator << (std::ostream& out, const TArtRNaIPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "NaI Layer: " << p.layer << ", ";
    out << "Time Calib: " << p.tcal << ", ";
    out << "Time Offset: " << p.toff << ", ";
    out << "QDC Calib: " << p.qcal << ", ";
    out << "QDC pedestal: " << p.qped << ", ";
    out << "Map of ADC: "  << p.map_adc;
    out << "Map of TDC: "  << p.map_tdc;
    return out;
  }

 private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     layer;
  Double_t  tcal;
  Double_t  toff;
  Double_t  qcal;
  Double_t  qped;
  TArtRIDFMap map_tdc;
  TArtRIDFMap map_adc;

  ClassDef(TArtRNaIPara,1);

};

#endif
