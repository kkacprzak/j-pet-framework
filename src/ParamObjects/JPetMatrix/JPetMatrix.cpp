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
 *  @file JPetMatrix.cpp
 */

#include "JPetMatrix/JPetMatrix.h"
#include "JPetLoggerInclude.h"

ClassImp(JPetMatrix);

using namespace std;

JPetMatrix::JPetMatrix() { SetName("JPetMatrix"); }

JPetMatrix::JPetMatrix(int id, std::string type, vector<int> pmIDs):
  fID(id), fType(type), fPMIDsVec(pmIDs)
{
  SetName("JPetMatrix");
}

JPetMatrix::JPetMatrix(const JPetMatrix &matrix):
  fID(matrix.getID()), fType(matrix.getType()), fPMIDsVec(matrix.getPMIDs())
{
  SetName("JPetMatrix");
}

JPetMatrix::JPetMatrix(bool isNull): fIsNullObject(isNull)
{
  SetName("JPetMatrix");
}

JPetMatrix::~JPetMatrix(){}

void JPetMatrix::setID(int id) { fID = id; }

void JPetMatrix::setType(string type) { fType = type; }

void JPetMatrix::setPMIDs(std::vector<int> ids) { fPMIDsVec = ids; }

void JPetMatrix::setScin(JPetScin& scin)
{
  fTRefScin = &scin;
}

void JPetMatrix::setWLS(JPetWLS& wls)
{
  fTRefWLS = &wls;
}

int JPetMatrix::getID() const
{
  return fID;
}

string JPetMatrix::getType() const
{
  return fType;
}

vector<int> JPetMatrix::getPMIDs() const
{
  return fPMIDsVec;
}

JPetScin& JPetMatrix::getScin() const
{
  if (fTRefScin.GetObject()) {
    return static_cast<JPetScin&>(*(fTRefScin.GetObject()));
  } else {
    ERROR("No JPetScin set, Null object will be returned");
    return JPetScin::getDummyResult();
  }
}

JPetWLS& JPetMatrix::getWLS() const
{
  if (fTRefWLS.GetObject()) {
    return static_cast<JPetWLS&>(*(fTRefWLS.GetObject()));
  } else {
    ERROR("No JPetWLS set, Null object will be returned");
    return JPetWLS::getDummyResult();
  }
}

bool JPetMatrix::operator==(const JPetMatrix& matrix) const
{
  if(matrix.getType()=="WLS") {
    return this->getID() == matrix.getID()
    && this->getType() == matrix.getType()
    && this->getPMIDs() == matrix.getPMIDs()
    && this->getWLS() == matrix.getWLS();
  } else {
    return this->getID() == matrix.getID()
    && this->getType() == matrix.getType()
    && this->getPMIDs() == matrix.getPMIDs()
    && this->getScin() == matrix.getScin();
  }


}

bool JPetMatrix::operator!=(const JPetMatrix& matrix) const
{
  return !(*this == matrix);
}

JPetMatrix& JPetMatrix::getDummyResult()
{
  static JPetMatrix DummyResult(true);
  return DummyResult;
}

bool JPetMatrix::isNullObject() const
{
  return fIsNullObject;
}

void JPetMatrix::clearTRefs()
{
  fTRefScin = NULL;
  fTRefWLS = NULL;
}
