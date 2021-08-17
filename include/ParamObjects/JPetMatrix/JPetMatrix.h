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
 *  @file JPetMatrix.h
 */

#ifndef JPET_MATRIX
#define JPET_MATRIX

#include "JPetScin/JPetScin.h"
#include "JPetWLS/JPetWLS.h"
#include <TNamed.h>
#include <TRef.h>

class JPetScin;
class JPetWLS;

/**
 * @brief Representation of a photomultiplier.
 *
 * Parametric class representing database information of a single photomultiplier.
 */
class JPetMatrix : public TNamed
{
public:
  JPetMatrix();
  JPetMatrix(int id, std::string type, std::vector<int> pmIDs);
  JPetMatrix(const JPetMatrix& matrix);
  explicit JPetMatrix(bool isNull);
  virtual ~JPetMatrix();
  void setID(int id);
  void setType(std::string type);
  void setPMIDs(std::vector<int> ids);

  void setScin(JPetScin& scin);
  void setWLS(JPetWLS& wls);

  int getID() const;
  std::string getType() const;
  std::vector<int> getPMIDs() const;

  JPetScin& getScin() const;
  JPetWLS& getWLS() const;

  bool operator==(const JPetMatrix& matrix) const;
  bool operator!=(const JPetMatrix& matrix) const;

  bool isNullObject() const;
  static JPetMatrix& getDummyResult();

protected:
  void clearTRefs();

#ifndef __CINT__
  int fID = -1;
  std::string fType = "";
  bool fIsNullObject = false;
#else
  int fID;
  std::string fType;
  bool fIsNullObject;
#endif
  std::vector<int> fPMIDsVec;
  TRef fTRefScin;
  TRef fTRefWLS;

  friend class JPetParamManager;

  ClassDef(JPetMatrix, 2);
};

#endif /* !JPET_MATRIX */
