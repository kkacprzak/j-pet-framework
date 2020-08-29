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
 *  @file JPetWLS.cpp
 */

#include "JPetLoggerInclude.h"
#include "JPetWLS/JPetWLS.h"

ClassImp(JPetWLS);

JPetWLS::JPetWLS()
{
  SetName("JPetWLS");
}

JPetWLS::JPetWLS(
  int id, double length, double height, double width,
  double center_x, double center_y, double center_z):
  fID(id), fLength(length), fHeight(height), fWidth(width),
  fWLSCenter(center_x, center_y, center_z)
{
  SetName("JPetWLS");
}

JPetWLS::JPetWLS(const JPetWLS &wls):
  fID(wls.getID()), fLength(wls.getLength()), fHeight(wls.getHeight()),
  fWidth(wls.getWidth()), fWLSCenter(wls.getCenterX(), wls.getCenterY(), wls.getCenterZ())
{
  SetName("JPetWLS");
}

JPetWLS::JPetWLS(bool isNull): fIsNullObject(isNull)
{
  SetName("JPetWLS");
}

JPetWLS::~JPetWLS(){}

void JPetWLS::setID(int id)
{
  fID = id;
}

void JPetWLS::setLength(double length)
{
  fLength = length;
}

void JPetWLS::setHeight(double height)
{
  fHeight = height;
}

void JPetWLS::setWidth(double width)
{
  fWidth = width;
}

void JPetWLS::setCenter(TVector3 center)
{
  fWLSCenter = center;
}

void JPetWLS::setCenterX(double centerX)
{
  fWLSCenter[0] = centerX;
}

void JPetWLS::setCenterY(double centerY)
{
  fWLSCenter[1] = centerY;
}

void JPetWLS::setCenterZ(double centerZ)
{
  fWLSCenter[2] = centerZ;
}

void JPetWLS::setPMIDs(std::vector<int> ids)
{
  fPMIDsVec = ids;
}

void JPetWLS::setSlot(JPetSlot& slot)
{
  fTRefSlot = &slot;
}

int JPetWLS::getID() const
{
  return fID;
}

double JPetWLS::getLength() const
{
  return fLength;
}

double JPetWLS::getHeight() const
{
  return fHeight;
}

double JPetWLS::getWidth() const
{
  return fWidth;
}

TVector3 JPetWLS::getCenter() const
{
  return fWLSCenter;
}

double JPetWLS::getCenterX() const
{
  return fWLSCenter.X();
}

double JPetWLS::getCenterY() const
{
  return fWLSCenter.Y();
}

double JPetWLS::getCenterZ() const
{
  return fWLSCenter.Z();
}

std::vector<int> JPetWLS::getPMIDs() const
{
  return fPMIDsVec;
}

const JPetSlot& JPetWLS::getSlot() const
{
  if (fTRefSlot.GetObject()) {
    return static_cast<JPetSlot&>(*(fTRefSlot.GetObject()));
  } else {
    ERROR("No JPetSlot set, Null object will be returned");
    return JPetSlot::getDummyResult();
  }
}

bool JPetWLS::operator==(const JPetWLS& wls) const
{
  return this->getID() == wls.getID()
    && this->getLength() == wls.getLength()
    && this->getHeight() == wls.getHeight()
    && this->getWidth() == wls.getWidth()
    && this->getCenter() == wls.getCenter()
    && this->getPMIDs() == wls.getPMIDs()
    && this->getSlot() == wls.getSlot();
}

bool JPetWLS::operator!=(const JPetWLS& wls) const
{
  return !(*this == wls);
}

JPetWLS& JPetWLS::getDummyResult()
{
  static JPetWLS DummyResult(true);
  return DummyResult;
}

bool JPetWLS::isNullObject() const
{
  return fIsNullObject;
}

void JPetWLS::clearTRefSlot()
{
  fTRefSlot = NULL;
}
