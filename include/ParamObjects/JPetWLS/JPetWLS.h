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
 *  @file JPetWLS.h
 */

#ifndef JPETWLS_H
#define JPETWLS_H

#include "./JPetSlot/JPetSlot.h"
#include <TVector3.h>
#include <TNamed.h>
#include <TRef.h>

/**
 * @brief Representation of a WLStillator.
 *
 * Parametric class representing database information of a WLStillator in the
 * JPetSlot object.
 */
class JPetWLS: public TNamed
{
public:
  JPetWLS();
  JPetWLS(
    int id, double length, double height, double width,
    double center_x, double center_y, double center_z
  );
  JPetWLS(const JPetWLS &wls);
  explicit JPetWLS(bool isNull);
  virtual ~JPetWLS();

  void setID(int id);
  void setLength(double length);
  void setHeight(double height);
  void setWidth(double width);
  void setCenter(TVector3 center);
  void setCenterX(double centerX);
  void setCenterY(double centerY);
  void setCenterZ(double centerZ);
  void setPMIDs(std::vector<int> ids);
  void setSlot(JPetSlot& slot);
  int getID() const;
  double getLength() const;
  double getHeight() const;
  double getWidth() const;
  TVector3 getCenter() const;
  double getCenterX() const;
  double getCenterY() const;
  double getCenterZ() const;
  std::vector<int> getPMIDs() const;
  const JPetSlot& getSlot() const;
  bool operator==(const JPetWLS& WLS) const;
  bool operator!=(const JPetWLS& WLS) const;
  static JPetWLS& getDummyResult();
  bool isNullObject() const;

protected:
  void clearTRefSlot();

#ifndef __CINT__
  int fID = -1;
  double fLength = 0.0;
  double fHeight = 0.0;
  double fWidth = 0.0;
  bool fIsNullObject = false;
#else
  int fID;
  double fLength;
  double fHeight;
  double fWidth;
  bool fIsNullObject;
#endif
  std::vector<int> fPMIDsVec;
  TVector3 fWLSCenter;
  TRef fTRefSlot;

  friend class JPetParamManager;

  ClassDef(JPetWLS, 1);
};

#endif /* !JPETWLS_H */
