// NeuLANDVETO calibration class
// 

#ifndef _TARTCALIBNEULANDVETO_H_
#define _TARTCALIBNEULANDVETO_H_

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
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>

class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNEBULAPla;
class TArtNEBULAPlaPara;
class TArtCalibSAMURAIT0;

class TString;

class TArtCalibNeuLANDVETO : public TArtReconstruction
{
public:
  TArtCalibNeuLANDVETO();
  virtual ~TArtCalibNeuLANDVETO();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumNeuLANDVETOPla() const;
  TArtNEBULAPla* GetNeuLANDVETOPla(Int_t i) const;
  TArtNEBULAPla* FindNeuLANDVETOPla(Int_t id) const;//find by id
  TClonesArray* GetNeuLANDVETOPlaArray() const;
  Int_t GetNumNeuLANDVETOPlaPara() const;
  TArtNEBULAPlaPara* GetNeuLANDVETOPlaPara(Int_t i) const;
  TArtNEBULAPlaPara* FindNeuLANDVETOPlaPara(Int_t id) const;//find by id
  TClonesArray* GetNeuLANDVETOPlaParaArray() const;

private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TClonesArray* fNeuLANDVETOPlaArray;
  TClonesArray* fNeuLANDVETOPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;
  TArtNEBULAPla *fNeuLANDVETOPlaMasterStart;

  ClassDef(TArtCalibNeuLANDVETO,1);
};

#endif
