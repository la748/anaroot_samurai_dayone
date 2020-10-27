// HIME calibration class
// 

#ifndef _TARTCALIBHIME_H_
#define _TARTCALIBHIME_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtHIMEPla.hh"
#include "TArtHIMEPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>



class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtHIMEPla;
typedef TClonesArray TArtHIMEPlaArray;
class TArtHIMEPlaPara;
typedef TClonesArray TArtHIMEPlaParaArray;
class TArtCalibSAMURAIT0;

class TString;

class TArtCalibHIMEMult : public TObject
{
public:
  TArtCalibHIMEMult();
  ~TArtCalibHIMEMult();

  void ClearData();

  // HIME layout = vetos + 5 layers of 10 bars for S027
  // direction of sublayers (V: vertical, H: horizontal)
  // beam --->   Veto V H V H V

  //  const Int_t NSubLayers = 5;

  #define NSubLayers 5

  
  Int_t fMultiplicity_N[NSubLayers+1];
  Int_t fMultiplicity_V;

  ClassDef(TArtCalibHIMEMult,1)
};

class TArtCalibHIME : public TArtReconstruction
{
public:
  TArtCalibHIME();
  virtual ~TArtCalibHIME();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumHIMEPla() const;
  TArtHIMEPla* GetHIMEPla(Int_t i) const;
  TArtHIMEPla* FindHIMEPla(Int_t id) const;//find by id
  TArtHIMEPlaArray* GetHIMEPlaArray() const;
  Int_t GetNumHIMEPlaPara() const;
  TArtHIMEPlaPara* GetHIMEPlaPara(Int_t i) const;
  TArtHIMEPlaPara* FindHIMEPlaPara(Int_t id) const;//find by id
  TArtHIMEPlaParaArray* GetHIMEPlaParaArray() const;

  void  CalMultiplicity(); // calculate multiplicity
  

  Int_t GetMultiplicity_N (){return fMult.fMultiplicity_N[0]; }
  Int_t GetMultiplicity_N1(){return fMult.fMultiplicity_N[1];}
  Int_t GetMultiplicity_N2(){return fMult.fMultiplicity_N[2];}
  Int_t GetMultiplicity_N3(){return fMult.fMultiplicity_N[3];}
  Int_t GetMultiplicity_N4(){return fMult.fMultiplicity_N[4];}
  Int_t GetMultiplicity_N5(){return fMult.fMultiplicity_N[5];}
  Int_t GetMultiplicity_V (){return fMult.fMultiplicity_V; }

  TArtCalibHIMEMult* GetMultiplicities(){return &fMult;} 
 
private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TArtHIMEPlaArray* fHIMEPlaArray;
  TArtHIMEPlaParaArray* fHIMEPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  TArtCalibHIMEMult fMult;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;

  ClassDef(TArtCalibHIME,1);
};

#endif
