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
 *  @file JPetWLSFactory.cpp
 */

#include "JPetWLS/JPetWLSFactory.h"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <tuple>

std::map<int, JPetWLS*>& JPetWLSFactory::getWLSs()
{
  if (!fInitialized) { initialize(); }
  return fWLSs;
}

void JPetWLSFactory::initialize()
{
  ParamObjectsDescriptions descriptions = fParamGetter.getAllBasicData(
    ParamObjectType::kWLS, fRunID
  );
  if (descriptions.size() == 0) {
    ERROR(Form("No WLS objects in run %i", fRunID));
    return;
  }
  for (auto description : descriptions) {
    fWLSs[description.first] = build(description.second);
  }
  fInitialized = true;
  ParamRelationalData relations = fParamGetter.getAllRelationalData(
    ParamObjectType::kWLS, ParamObjectType::kSlot, fRunID
  );
  for (auto relation : relations) {
    fWLSs[relation.first]->setSlot(*fSlotFactory.getSlots().at(relation.second));
  }
}

JPetWLS* JPetWLSFactory::build(ParamObjectDescription data)
{
  try {
    int id = boost::lexical_cast<int>(data.at("id"));
    double length = boost::lexical_cast<double>(data.at("length"));
    double height = boost::lexical_cast<double>(data.at("height"));
    double width = boost::lexical_cast<double>(data.at("width"));
    double centerX = boost::lexical_cast<double>(data.at("xcenter"));
    double centerY = boost::lexical_cast<double>(data.at("ycenter"));
    double centerZ = boost::lexical_cast<double>(data.at("zcenter"));
    return new JPetWLS(id, length, height, width, centerX, centerY, centerZ);
  } catch (const std::exception &e) {
    ERROR(Form("Failed to build WLStillator with error: %s", e.what()));
    throw;
  }
}
