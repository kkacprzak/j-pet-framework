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
 *  @file JPetParamGetterAscii.h
 */

#ifndef JPETPARAMGETTERASCII_H
#define JPETPARAMGETTERASCII_H

#include "JPetParamGetter/JPetParamGetter.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <map>

class JPetParamGetterAscii : public JPetParamGetter
{
public:
  JPetParamGetterAscii(std::string filename) : fFilename(filename) {}
  ~JPetParamGetterAscii() {}
  ParamObjectsDescriptions getAllBasicData(ParamObjectType type, const int runID);
  ParamRelationalData getAllRelationalData(
    ParamObjectType type1, ParamObjectType type2, const int runID);

private:
  JPetParamGetterAscii(const JPetParamGetterAscii &paramGetterAscii);
  JPetParamGetterAscii& operator=(const JPetParamGetterAscii &paramGetterAscii);
  ParamObjectDescription toDescription(boost::property_tree::ptree & info);
  std::string fFilename;
};

#endif /* !JPETPARAMGETTERASCII_H */
