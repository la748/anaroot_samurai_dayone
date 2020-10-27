#ifndef _TARTTKEPARA_H_
#define _TARTTKEPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtTKE.hh"

class TArtTKEPara : public TNamed {

 public:
  TArtTKEPara(){ }
  TArtTKEPara(Int_t i, TString n, Int_t f, 
             Double_t ech2mev, Double_t eped) :
    id(i), detname(n), fpl(f), numadc(1), e_ped(eped), e_ch2mev(ech2mev) {}
  TArtTKEPara(Int_t i, TString n, Int_t f, 
             Double_t *ech2mev, Double_t *eped) :
    id(i), detname(n), fpl(f), numadc(4){ 
    chmap.clear();
    for(int ii=0;ii<4;ii++){
      e_ch2mevarray[ii] = ech2mev[ii]; e_pedarray[ii] = eped[ii];
    }
  }

  TArtTKEPara(Int_t i, TString n, Int_t f, 
	     Double_t ech2mev, Double_t eped, Double_t tch2nsec, Double_t toffset) :
    id(i), detname(n), fpl(f), numadc(1),
    e_ped(eped), e_ch2mev(ech2mev), 
    t_offset(toffset), t_ch2nsec(tch2nsec){} 

  TArtTKEPara(Int_t i, TString n, Int_t f, 
	     Double_t *ech2mev, Double_t *eped, Double_t *tch2nsec, Double_t *toffset) :
    id(i), detname(n), fpl(f), numadc(4){ 
    chmap.clear();
    for(int ii=0;ii<4;ii++){
      e_ch2mevarray[ii] = ech2mev[ii]; e_pedarray[ii] = eped[ii];
      t_ch2nsecarray[ii] = tch2nsec[ii]; t_offsetarray[ii] = toffset[ii];
    }
  }
  ~TArtTKEPara() {}

  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t ech2mev, Double_t eped, Double_t tch2nsec, Double_t toffset) {
    id=i; detname=n; fpl=f; 
    e_ch2mev = ech2mev; e_ped = eped; t_ch2nsec = tch2nsec; t_offset = toffset;
    numadc = 1;
  }
  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t *ech2mev, Double_t *eped, Double_t *tch2nsec, Double_t *toffset) {
    id=i; detname=n; fpl=f; 
    for(int ii=0;ii<4;ii++){
      e_ch2mevarray[ii] = ech2mev[ii]; e_pedarray[ii] = eped[ii];
      t_ch2nsecarray[ii] = tch2nsec[ii]; t_offsetarray[ii] = toffset[ii];}
    numadc = 4;
  }

  void SetMap(Int_t qfpl, Int_t qdet, Int_t qgeo, Int_t qch, 
              Int_t tfpl, Int_t tdet, Int_t tgeo, Int_t tch) {
    map_adc.SetMap(qfpl,qdet,qgeo,qch); 
    map_tdc.SetMap(tfpl,tdet,tgeo,tch); 
  }
  void SetMap(Int_t i, Int_t qfpl, Int_t qdet, Int_t qgeo, Int_t qch, 
                       Int_t tfpl, Int_t tdet, Int_t tgeo, Int_t tch) {
    map_adcarray[i].SetMap(qfpl,qdet,qgeo,qch); 
    chmap.insert(std::pair<TArtRIDFMap, int>(map_adcarray[i],i));
    map_tdcarray[i].SetMap(tfpl,tdet,tgeo,tch); 
    chmap.insert(std::pair<TArtRIDFMap, int>(map_tdcarray[i],i));
  }
  void SetMap(Int_t tdet, Int_t tgeo, Int_t tch,
              Int_t qdet, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(fpl,qdet,qgeo,qch);
    map_tdc.SetMap(fpl,tdet,tgeo,tch);
  }
  void SetMap(Int_t qfpl, Int_t qdet, Int_t qgeo, Int_t qch){
    map_adc.SetMap(qfpl,qdet,qgeo,qch);
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetECh2MeV() const { return e_ch2mev; } 
  Double_t GetEPedestal() const { return e_ped; } 
  Double_t GetECh2MeV(Int_t i) const { return e_ch2mevarray[i]; } 
  Double_t GetEPedestal(Int_t i) const { return e_pedarray[i]; } 
  Double_t GetTCh2nsec() const { return t_ch2nsec; } 
  Double_t GetTOffset() const { return t_offset; } 
  Double_t GetTCh2nsec(Int_t i) const { return t_ch2nsecarray[i]; } 
  Double_t GetTOffset(Int_t i) const { return t_offsetarray[i]; } 

  Int_t GetNumADC(){return numadc;}
  TArtRIDFMap * GetADCMap() { return &map_adc;  }
  TArtRIDFMap * GetADCMap(Int_t i) { return &map_adcarray[i];  }
  Int_t FindCh(TArtRIDFMap *rmap){
    std::map<TArtRIDFMap, int>::iterator itr = chmap.find(*rmap);
    if(itr != chmap.end()){return itr->second;}
    return -1;
  }
  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }
  TArtRIDFMap * GetTDCMap(Int_t i) { return &map_tdcarray[i];  }
  //Int_t FindCh(TArtRIDFMap *rmap){
  //  std::map<TArtRIDFMap, int>::iterator itr = chmap.find(*rmap);
  //  if(itr != chmap.end()){return itr->second;}
  //  return -1;
  //}


  friend std::ostream & operator << (std::ostream& out, const TArtTKEPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "e_ped: " << p.e_ped << ", ";
    out << "e_ch2mev: " << p.e_ch2mev << ", "; 
    out << "t_offset: " << p.t_offset << ", ";
    out << "t_ch2nsec: " << p.t_ch2nsec << ", "; 
    out << std::endl;
    out << "Map of adc: "  << p.map_adc;
    out << "Map of tdc: "  << p.map_tdc;
    return out;
  }

protected:

  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     numadc;

  Double_t  e_ped;
  Double_t  e_ch2mev;
  Double_t  t_offset;
  Double_t  t_ch2nsec;
  TArtRIDFMap map_adc;
  TArtRIDFMap map_tdc;

  Double_t  e_pedarray[NUM_TKE_CHANNEL];
  Double_t  e_ch2mevarray[NUM_TKE_CHANNEL];
  Double_t  t_offsetarray[NUM_TKE_CHANNEL];
  Double_t  t_ch2nsecarray[NUM_TKE_CHANNEL];
  TArtRIDFMap map_adcarray[NUM_TKE_CHANNEL];
  TArtRIDFMap map_tdcarray[NUM_TKE_CHANNEL];

  std::map<TArtRIDFMap, int> chmap;

  ClassDef(TArtTKEPara,1);

};

#endif
