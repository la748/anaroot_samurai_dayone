#ifndef _TARTPLASTICPARA_H_
#define _TARTPLASTICPARA_H_

#include <map>
#include <Riostream.h>
#include <stdio.h>
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtPlastic.hh"

class TArtPlasticPara : public TNamed {

 public:
  TArtPlasticPara(){}
  TArtPlasticPara(Int_t i, TString n, Int_t f, 
		  Double_t tcl, Double_t tcr, 
		  Double_t qpedl, Double_t qpedr, 
		  Double_t ts_al, Double_t ts_bl, Double_t ts_ar, Double_t ts_br, 
		  Double_t tdc_uf, Double_t tdc_of, 
		  Double_t z) :
    id(i), detname(n), fpl(f), tcal_left(tcl), tcal_right(tcr), qped_left(qpedl), qped_right(qpedr), tslew_left_a(ts_al), tslew_left_b(ts_bl), tslew_right_a(ts_ar), tslew_right_b(ts_br), tdc_underflow(tdc_uf), tdc_overflow(tdc_of), zoffset(z){
    qtc_underflow=0;
    qtc_overflow=10000000;
    multdc_underflow=0;
    multdc_overflow=10000000;
  }
  ~TArtPlasticPara() {}

  void SetPalameters(Int_t i, TString n, Int_t f, 
		     Double_t tcl, Double_t tcr, 
		     Double_t qpedl, Double_t qpedr, 
		     Double_t ts_al, Double_t ts_bl, 
		     Double_t ts_ar, Double_t ts_br, 
		     Double_t tdc_uf, 
		     Double_t tdc_of, 
		     Double_t z){
    id=i; detname=n; fpl=f; 
    tcal_left=tcl; tcal_right=tcr; qped_left=qpedl; qped_right=qpedr; 
    tslew_left_a=ts_al; tslew_left_b=ts_bl; 
    tslew_right_a=ts_ar; tslew_right_b=ts_br; tdc_underflow=tdc_uf; tdc_overflow=tdc_of; zoffset=z; }

  void SetQTCWindow(Double_t qtc_uf, Double_t qtc_of){
    qtc_underflow=qtc_uf; qtc_overflow=qtc_of;
  }
  void SetMulHitTDCWindow(Double_t mult_uf, Double_t mult_of){
    multdc_underflow=mult_uf; multdc_overflow=mult_of;
  }

  void SetMap(Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc) {
    map_tleft.SetMap (tf,td,tlg,tlc); 
    map_tright.SetMap(tf,td,trg,trc); 
    map_qleft.SetMap (qf,qd,qlg,qlc); 
    map_qright.SetMap(qf,qd,qrg,qrc); 
  }
  void SetMap(Int_t tdev, Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qdev, Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc) {
    map_tleft.SetMap (tdev,tf,td,tlg,tlc); 
    map_tright.SetMap(tdev,tf,td,trg,trc); 
    map_qleft.SetMap (qdev,qf,qd,qlg,qlc); 
    map_qright.SetMap(qdev,qf,qd,qrg,qrc); 
  }

  void SetMap(Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc,
	      Int_t qtcf, Int_t qtcd, 
	      Int_t qtclg, Int_t qtclc, Int_t qtcrg, Int_t qtcrc) {
    map_tleft.SetMap (tf,td,tlg,tlc); 
    map_tright.SetMap(tf,td,trg,trc); 
    map_qleft.SetMap (qf,qd,qlg,qlc); 
    map_qright.SetMap(qf,qd,qrg,qrc); 
    map_qtcleft.SetMap (qtcf,qtcd,qtclg,qtclc); 
    map_qtcright.SetMap(qtcf,qtcd,qtcrg,qtcrc); 
  }
  void SetMap(Int_t tdev, Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qdev, Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc,
	      Int_t qtcdev, Int_t qtcf, Int_t qtcd, 
	      Int_t qtclg, Int_t qtclc, Int_t qtcrg, Int_t qtcrc) {
    map_tleft.SetMap (tdev,tf,td,tlg,tlc); 
    map_tright.SetMap(tdev,tf,td,trg,trc); 
    map_qleft.SetMap (qdev,qf,qd,qlg,qlc); 
    map_qright.SetMap(qdev,qf,qd,qrg,qrc); 
    map_qtcleft.SetMap (qtcdev,qtcf,qtcd,qtclg,qtclc); 
    map_qtcright.SetMap(qtcdev,qtcf,qtcd,qtcrg,qtcrc); 
  }

  void SetMap(Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc,
	      Int_t qtcf, Int_t qtcd, 
	      Int_t qtclg, Int_t qtclc, Int_t qtcrg, Int_t qtcrc,
	      Int_t multf, Int_t multd, 
	      Int_t multlg, Int_t multlc, Int_t multrg, Int_t multrc) {
    map_tleft.SetMap (tf,td,tlg,tlc); 
    map_tright.SetMap(tf,td,trg,trc); 
    map_qleft.SetMap (qf,qd,qlg,qlc); 
    map_qright.SetMap(qf,qd,qrg,qrc); 
    map_qtcleft.SetMap (qtcf,qtcd,qtclg,qtclc); 
    map_qtcright.SetMap(qtcf,qtcd,qtcrg,qtcrc); 
    map_multleft.SetMap (multf,multd,multlg,multlc); 
    map_multright.SetMap(multf,multd,multrg,multrc); 
  }
  void SetMap(Int_t tdev, Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qdev, Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc,
	      Int_t qtcdev, Int_t qtcf, Int_t qtcd, 
	      Int_t qtclg, Int_t qtclc, Int_t qtcrg, Int_t qtcrc,
	      Int_t multdev, Int_t multf, Int_t multd, 
	      Int_t multlg, Int_t multlc, Int_t multrg, Int_t multrc) {
    map_tleft.SetMap (tdev,tf,td,tlg,tlc); 
    map_tright.SetMap(tdev,tf,td,trg,trc); 
    map_qleft.SetMap (qdev,qf,qd,qlg,qlc); 
    map_qright.SetMap(qdev,qf,qd,qrg,qrc); 
    map_qtcleft.SetMap (qtcdev,qtcf,qtcd,qtclg,qtclc); 
    map_qtcright.SetMap(qtcdev,qtcf,qtcd,qtcrg,qtcrc); 
    map_multleft.SetMap (qtcdev,multf,multd,multlg,multlc); 
    map_multright.SetMap(qtcdev,multf,multd,multrg,multrc); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetTCalLeft() const { return tcal_left; } 
  Double_t GetTCalRight() const { return tcal_right; } 
  Double_t GetQPedLeft() const { return qped_left; } 
  Double_t GetQPedRight() const { return qped_right; } 
  Double_t GetTLSlewA() const { return tslew_left_a; } 
  Double_t GetTLSlewB() const { return tslew_left_b; } 
  Double_t GetTRSlewA() const { return tslew_right_a; } 
  Double_t GetTRSlewB() const { return tslew_right_b; } 
  Double_t GetTDCUnderflow() const {return tdc_underflow;}
  Double_t GetTDCOverflow() const {return tdc_overflow;}
  Double_t GetQTCUnderflow() const {return qtc_underflow;}
  Double_t GetQTCOverflow() const {return qtc_overflow;}
  Double_t GetMulHitTDCUnderflow() const {return multdc_underflow;}
  Double_t GetMulHitTDCOverflow() const {return multdc_overflow;}
  Double_t GetZoffset() const { return zoffset; } 

  TArtRIDFMap * GetTLMap() { return &map_tleft;  }
  TArtRIDFMap * GetTRMap() { return &map_tright; }
  TArtRIDFMap * GetQLMap() { return &map_qleft;  }
  TArtRIDFMap * GetQRMap() { return &map_qright; }
  TArtRIDFMap * GetQTCLMap() { return &map_qtcleft;  }
  TArtRIDFMap * GetQTCRMap() { return &map_qtcright; }
  TArtRIDFMap * GetMulHitTDCLMap() { return &map_multleft;  }
  TArtRIDFMap * GetMulHitTDCRMap() { return &map_multright; }

  friend std::ostream & operator << (std::ostream& out, const TArtPlasticPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Time Calib Left: " << p.tcal_left << ", ";
    out << "Time Calib Right: " << p.tcal_right << ", ";
    out << "QDC Pedestal Left: " << p.qped_left << ", ";
    out << "QDC Pedestal Right: " << p.qped_right << ", ";
    out << "Time Slewing Corr. A of Left: " << p.tslew_left_a << ", ";
    out << "Time Slewing Corr. B of Left: " << p.tslew_left_b << ", ";
    out << "Time Slewing Corr. A of Right: " << p.tslew_right_a << ", ";
    out << "Time Slewing Corr. B of Right: " << p.tslew_right_b << ", ";
    out << "Time Calib Right: " << p.tcal_right << ", ";
    out << "TDC Underflow: " << p.tdc_underflow << std::endl;
    out << "TDC Overflow: " << p.tdc_overflow << std::endl;
    out << "QTC Underflow: " << p.qtc_underflow << std::endl;
    out << "QTC Overflow: " << p.qtc_overflow << std::endl;
    out << "Z-position offset: " << p.zoffset << std::endl;
    out << "Map of T-Left: "  << p.map_tleft;
    out << "Map of T-Right: " << p.map_tright;
    out << "Map of Q-Left: "  << p.map_qleft;
    out << "Map of Q-Right: " << p.map_qright;
    out << "Map of QTC-Left: "  << p.map_qtcleft;
    out << "Map of QTC-Right: " << p.map_qtcright;
    out << "Map of MULT-Left: "  << p.map_multleft;
    out << "Map of MULT-Right: " << p.map_multright;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;

  // calibration value for TDC -> timing
  Double_t  tcal_left;
  Double_t  tcal_right;

  // calibration value to subtract pedestal
  Double_t  qped_left;
  Double_t  qped_right;

  // Slew correction parameters for left, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_left_a;
  Double_t  tslew_left_b;

  // Slew correction parameters for right, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_right_a;
  Double_t  tslew_right_b;

  // TDC range where timing hit will be seeked.
  Double_t  tdc_underflow;
  Double_t  tdc_overflow;

  // TDC range where timing hit from QTC will be seeked.
  Double_t  qtc_underflow;
  Double_t  qtc_overflow;

  // TDC range where multi-timing hit will be seeked.
  Double_t  multdc_underflow;
  Double_t  multdc_overflow;

  // position of plastic counter w.r.t. focal plane
  Double_t  zoffset; // named "zpos" in 

  // mapping class for tdc which will be used for TOF calculation.
  // parameters named "t_fpl", "t_det", "tleft_geo/tright_geo", "tleft_ch/tright_ch" are needed to be mapped.
  // "dev", "fpl" and "det" values will be used as dafault of "t_dev", "t_fpl" and "t_det" if those are not given.
  TArtRIDFMap map_tleft;
  TArtRIDFMap map_tright;

  // mapping class for qdc
  // parameters named "q_fpl", "q_det", "qleft_geo/qright_geo", "qleft_ch/qright_ch" are needed to be mapped.
  // "dev", "fpl" and "det" values will be used as dafault of "q_dev", "q_fpl" and "q_det" if those are not given.
  TArtRIDFMap map_qleft;
  TArtRIDFMap map_qright;

  // mapping class for qtc
  // parameters named "qtc_fpl", "qtc_det", "qtcleft_geo/qtcright_geo", "qtcleft_ch/qtcright_ch" are needed to be mapped.
  // "dev", "fpl" and "det" values will be used as dafault of "qtc_dev", "qtc_fpl" and "qtc_det" if those are not given.
  TArtRIDFMap map_qtcleft;
  TArtRIDFMap map_qtcright;

  // mapping class for multihit tdc. tdc module used for multihit purpose is sometimes diffent from that for high resolution tdc. 
  // parameters named "mult_fpl", "mult_det", "multleft_geo/multright_geo", "multleft_ch/multright_ch" are needed to be mapped.
  // "dev", "fpl" and "det" values will be used as dafault of "mult_dev", "mult_fpl" and "mult_det" if those are not given.
  TArtRIDFMap map_multleft;
  TArtRIDFMap map_multright;

  ClassDef(TArtPlasticPara,1);

};

#endif
