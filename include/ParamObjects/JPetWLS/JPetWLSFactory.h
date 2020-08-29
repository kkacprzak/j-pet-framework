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
 *  @file JPetWLSFactory.h
 */

#ifndef JPET_WLS_FACTORY_H
#define JPET_WLS_FACTORY_H

#include "./JPetParamGetter/JPetParamGetter.h"
#include "./JPetSlot/JPetSlotFactory.h"
#include "JPetWLS.h"
#include <map>

/**
 * @brief A factory of JPetWLS objects.
 *
 * This class is able to create those objects using data from the database.
 */
class JPetWLSFactory
{
public:
  JPetWLSFactory(JPetParamGetter& paramGetter, int runID, JPetSlotFactory& slotFactory):
    fParamGetter(paramGetter), fRunID(runID), fSlotFactory(slotFactory), fInitialized(false) {}
  std::map<int, JPetWLS*>& getWLSs();

private:
  JPetParamGetter& fParamGetter;
  const int fRunID;
  JPetSlotFactory& fSlotFactory;
  bool fInitialized;
  std::map<int, JPetWLS*> fWLSs;
  void initialize();
  JPetWLS* build(ParamObjectDescription data);
};

#endif /* !JPET_WLS_FACTORY_H */
