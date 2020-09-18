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
 *  @file JPetPM.cpp
 */

#include "JPetLoggerInclude.h"
#include "JPetPM/JPetPM.h"

ClassImp(JPetPM);

JPetPM::JPetPM() { SetName("JPetPM"); }

JPetPM::JPetPM(int id,std::string desc, double position):
  fID(id), fDesc(desc), fPosition(position)
{
  SetName("JPetPM");
}

JPetPM::JPetPM(const JPetPM &pm):
  fID(pm.getID()), fDesc(pm.getDesc()), fPosition(pm.getPosition())
{
  SetName("JPetPM");
}

JPetPM::JPetPM(bool isNull): fIsNullObject(isNull)
{
  SetName("JPetPM");
}

JPetPM::~JPetPM(){}

void JPetPM::setID(int id)
{
  fID = id;
}

void JPetPM::setDesc(std::string desc)
{
  fDesc = desc;
}

void JPetPM::setPosition(double position)
{
  fPosition = position;
}

int JPetPM::getID() const
{
  return fID;
}

std::string JPetPM::getDesc() const
{
  return fDesc;
}

double JPetPM::getPosition() const
{
  return fPosition;
}

bool JPetPM::operator==(const JPetPM& pm) const
{
  return this->getID() == pm.getID()
    && this->getDesc() == pm.getDesc()
    && this->getPosition() == pm.getPosition();
}

bool JPetPM::operator!=(const JPetPM& pm) const
{
  return !(*this == pm);
}

bool JPetPM::isNullObject() const
{
  return fIsNullObject;
}

JPetPM& JPetPM::getDummyResult()
{
  static JPetPM DummyResult(true);
  return DummyResult;
}
