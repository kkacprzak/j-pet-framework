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
 *  @file JPetMatrixSignal.h
 */

#ifndef JPETMATRIXSIGNAL_H
#define JPETMATRIXSIGNAL_H

#include "JPetRawSignal/JPetRawSignal.h"
#include "JPetMatrix/JPetMatrix.h"

/**
 * @brief Data class representing a physical signal reconstructed
 * in a matrix of SiPMs on a one side of a scintillator in a modular layer
 *
 */
class JPetMatrixSignal: public JPetBaseSignal
{
public:
  JPetMatrixSignal();
  explicit JPetMatrixSignal(double time);
  virtual ~JPetMatrixSignal();
  bool isNullObject() const;
  explicit JPetMatrixSignal(bool isNull);

  double getTime() const;
  void setTime(double time);
  double getTOT() const;
  bool addRawSignal(const JPetRawSignal& rawSignal);
  std::map<int, JPetRawSignal> getRawSignals() const;
  void Clear(Option_t * opt = "");

  void setMatrix(const JPetMatrix& matrix);
  const JPetMatrix& getMatrix() const;

private:
  double fTime;
  TRef fMatrix;
  std::map<int, JPetRawSignal> fRawSignalsMap;

protected:
  #ifndef __CINT__
  bool fIsNullObject = false;
  #else
  bool fIsNullObject;
  #endif

  ClassDef(JPetMatrixSignal, 5);
};

#endif /* !JPETMATRIXSIGNAL_H */
