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
 *  @file JPetMatrixFactory.h
 */

#ifndef JPET_MATRIX_FACTORY_H
#define JPET_MATRIX_FACTORY_H

#include "JPetParamGetter/JPetParamGetter.h"
#include "JPetScin/JPetScinFactory.h"
#include "JPetWLS/JPetWLSFactory.h"
#include "JPetMatrix/JPetMatrix.h"
#include <map>

/**
 * @brief A factory of JPetMatrix objects.
 *
 * This class is able to create those objects using data from the database.
 */
class JPetMatrixFactory
{
public:
  JPetMatrixFactory(
    JPetParamGetter& paramGetter, int runID, JPetScinFactory& scinFactory,  JPetWLSFactory& wlsFactory
  ): fParamGetter(paramGetter), fRunID(runID), fScinFactory(scinFactory), fWLSFactory(wlsFactory), fInitialized(false) {}
  std::map<int, JPetMatrix*>& getMatrices();

private:
  JPetParamGetter& fParamGetter;
  const int fRunID;
  JPetScinFactory& fScinFactory;
  JPetWLSFactory& fWLSFactory;
  bool fInitialized;
  std::map<int, JPetMatrix*> fMatrices;
  void initialize();
  JPetMatrix* build(ParamObjectDescription data);
};

#endif /* !JPET_MATRIX_FACTORY_H */
