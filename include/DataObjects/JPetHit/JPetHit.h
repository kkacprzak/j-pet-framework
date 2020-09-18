/**
 *  @copyright Copyright 2020 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetHit.h
 */

#ifndef JPETHIT_H
#define JPETHIT_H

#include "JPetMatrixSignal/JPetMatrixSignal.h"
#include "JPetScin/JPetScin.h"
#include "TVector3.h"
#include "TObject.h"
#include <cstddef>
#include <utility>
#include <TRef.h>

class JPetMatrixSignal;

/**
 * @brief Data class representing a reconstructed hit of a photon in the scintillator strip.
 *
 * Hit class contains two objects of type JPetMatrixSignal (from "Side A" and "Side B"
 * of the Barrel) which represent signals at two ends of a scintillator strip,
 * from which the hit was reconstructed. Analyst can set the energy, time and position.
 * Agreed convention of units: energy [keV], time [ps], position [cm].
 * User can describe quality of energy, hit time and time difference
 * between the two signals in the hit.
 */
class JPetHit: public TObject
{
public:
  enum Side { SideA, SideB, WLS };
  enum RecoFlag { Good, Corrupted, Unknown };
  JPetHit();
  JPetHit(
    double energy, double qualityOfEnergy, double time, double qualityOfTime,
    TVector3& position, JPetMatrixSignal& signalA, JPetMatrixSignal& signalB,
    JPetMatrixSignal& wlsSignal, JPetScin& scin, JPetWLS&  wls
  );
  virtual ~JPetHit();

  JPetHit::RecoFlag getRecoFlag() const;
  double getEnergy() const;
  double getQualityOfEnergy() const;
  double getTime() const;
  double getTimeDiff() const;
  double getQualityOfTime() const;
  double getQualityOfTimeDiff() const;

  double getPosX() const;
  double getPosY() const;
  double getPosZ() const;
  double getPos(int index) const;
  const TVector3& getPos() const;

  const JPetMatrixSignal& getSignal(Side side) const;
  const JPetMatrixSignal& getSignalA() const;
  const JPetMatrixSignal& getSignalB() const;
  const JPetMatrixSignal& getSignalWLS() const;

  const JPetScin& getScin() const;
  const JPetWLS& getWLS() const;

  void setRecoFlag(JPetHit::RecoFlag flag);
  void setEnergy(double energy);
  void setQualityOfEnergy(double qualityOfEnergy);
  void setTime(double time);
  void setQualityOfTime(double qualityOfTime);
  void setTimeDiff(double td);
  void setQualityOfTimeDiff(double qtd);

  void setPosX(double x);
  void setPosY(double y);
  void setPosZ(double z);
  void setPos(double x, double y, double z);
  void setSignals(
    const JPetMatrixSignal& sigA, const JPetMatrixSignal& sigB, const JPetMatrixSignal& sigWLS
  );
  void setSignalA(const JPetMatrixSignal& sig);
  void setSignalB(const JPetMatrixSignal& sig);
  void setSignalWLS(const JPetMatrixSignal& sig);

  void setScin(JPetScin& scin);
  void setWLS(JPetWLS& wls);

  bool isSignalASet()const;
  bool isSignalBSet()const;
  bool isSignalWLSSet()const;
  bool checkConsistency() const;
  void Clear(Option_t* opt  = "");

private:
  RecoFlag fFlag = JPetHit::Unknown;
  double fEnergy = 0.0f;
  double fQualityOfEnergy = 0.0f;
  double fTime = 0.0f;
  double fQualityOfTime = 0.0f;
  double fTimeDiff = 0.0f;
  double fQualityOfTimeDiff = 0.0f;
  bool fIsSignalAset = false;
  bool fIsSignalBset = false;
  bool fIsSignalWLSset = false;
  TVector3 fPos;
  JPetMatrixSignal fSignalA;
  JPetMatrixSignal fSignalB;
  JPetMatrixSignal fSignalWLS;
  TRef fScin = NULL;
  TRef fWLS = NULL;

  ClassDef(JPetHit, 13);
};

#endif /* !JPETHIT_H */
