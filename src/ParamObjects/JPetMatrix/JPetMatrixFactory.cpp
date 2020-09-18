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
 *  @file JPetMatrixFactory.cpp
 */

#include "JPetMatrix/JPetMatrixFactory.h"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <tuple>

std::map<int, JPetMatrix*>& JPetMatrixFactory::getMatrices()
{
  if (!fInitialized) { initialize(); }
  return fMatrices;
}

void JPetMatrixFactory::initialize()
{
  ParamObjectsDescriptions descriptions = fParamGetter.getAllBasicData(
    ParamObjectType::kMatrix, fRunID
  );
  if (descriptions.size() == 0) {
    ERROR(Form("No Matrix objects in run %i", fRunID));
    return;
  }
  for (auto description : descriptions) {
    fMatrices[description.first] = build(description.second);
  }
  fInitialized = false;
  ParamRelationalData relations = fParamGetter.getAllRelationalData(
    ParamObjectType::kMatrix, ParamObjectType::kParent, fRunID
  );
  for (auto relation : relations) {
    if(fMatrices[relation.first]->getType()=="WLS"){
      fMatrices[relation.first]->setWLS(*fWLSFactory.getWLSs().at(relation.second));
    }else{
      fMatrices[relation.first]->setScin(*fScinFactory.getScins().at(relation.second));
    }
  }
}

JPetMatrix* JPetMatrixFactory::build(ParamObjectDescription data)
{
  try {
    int id = boost::lexical_cast<int>(data.at("id"));
    std::string type = boost::lexical_cast<std::string>(data.at("type"));
    int pm_1_id = boost::lexical_cast<int>(data.at("pm_1_id"));
    int pm_2_id = boost::lexical_cast<int>(data.at("pm_2_id"));
    int pm_3_id = boost::lexical_cast<int>(data.at("pm_3_id"));
    int pm_4_id = boost::lexical_cast<int>(data.at("pm_4_id"));
    std::vector<int> pm_ids;
    pm_ids.push_back(pm_1_id);
    pm_ids.push_back(pm_2_id);
    pm_ids.push_back(pm_3_id);
    pm_ids.push_back(pm_4_id);
    return new JPetMatrix(id, type, pm_ids);
  } catch (const std::exception &e) {
    ERROR(Form("Failed to build Matrix with error: %s", e.what()));
    throw;
  }
}
