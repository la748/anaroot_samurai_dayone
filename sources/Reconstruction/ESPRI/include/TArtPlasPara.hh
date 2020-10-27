#ifndef _TARTPLASPARA_H_
#define _TARTPLASPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtPlasPara : public TNamed {

 public:
  TArtPlasPara(){}
  TArtPlasPara(Int_t i, TString n, Int_t f, Int_t l, Int_t idp, Int_t ch, Double_t qc, Double_t qp) :
    id(i), detname(n), fpl(f), layer(l), id_plane(idp), channel(ch), qcal(qc), qped(qp){ }
  ~TArtPlasPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, Int_t l, Int_t idp, Int_t ch, Double_t qc, Double_t qp) 
  { id=i; detname=n; fpl=f; layer=l; id_plane=idp; channel=ch; qcal=qc; qped=qp;}

  void SetMap(Int_t dt, Int_t geo, Int_t ch) {
    map.SetMap(fpl,dt,geo,ch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLayer() const { return layer; }
  Int_t GetPlaneID() const { return id_plane; }
  Int_t GetCh() const { return channel; }
  Double_t GetQCal() const { return qcal; } 
  Double_t GetQPed() const { return qped; } 

  TArtRIDFMap * GetPlasMap() { return &map;  }

  friend std::ostream & operator << (std::ostream& out, const TArtPlasPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Plas Layer: " << p.layer << ", ";
    out << "Plane-ID: " << p.id_plane << ", ";
    out << "Plas Channel: " << p.channel << ", ";
    out << "QDC Calib: " << p.qcal << ", ";
    out << "QDC pedestal: " << p.qped << ", ";
    out << "Map of Plas: "  << p.map;
    return out;
  }

 private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     layer;
  Int_t     id_plane;
  Int_t     channel;
  Double_t  qcal;
  Double_t  qped;

  TArtRIDFMap map;

  ClassDef(TArtPlasPara,1);

};

#endif
