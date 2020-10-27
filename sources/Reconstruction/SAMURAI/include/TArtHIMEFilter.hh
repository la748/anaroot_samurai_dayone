
#ifndef _TARTHIMEFILTER_H_
#define _TARTHIMEFILTER_H_

#include "TNamed.h"

class TArtCalibHIME;

class TArtHIMEFilter
{
public:
  static void IHitMin(TArtCalibHIME *fCalibHIME, Int_t ihitmin_n, Int_t ihitmin_v);
  static void Threshold(TArtCalibHIME *fCalibHIME);
  static void Threshold(TArtCalibHIME *fCalibHIME, Double_t threshold_n, Double_t threshold_v);
  static void TOF(TArtCalibHIME *fCalibHIME, Double_t min, Double_t max);
  static void Veto(TArtCalibHIME *fCalibHIME,  Int_t VetoNum);
  static void HitMinPos(TArtCalibHIME *fCalibHIME);
  static void HitMinTime(TArtCalibHIME *fCalibHIME);
  static void HitMinPos2(TArtCalibHIME *fCalibHIME);

private:
  TArtHIMEFilter();
  virtual ~TArtHIMEFilter();

  ClassDef(TArtHIMEFilter,1);
};

#endif
