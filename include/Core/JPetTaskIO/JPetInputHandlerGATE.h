/**
 *  @copyright Copyright 2025 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetInputHandlerGATE.h
 */

#ifndef JPETINPUTHANDLERGATE_H
#define JPETINPUTHANDLERGATE_H

#include <fstream>
#include <string>

#include <TObject.h>
#include <TVector3.h>

#include "JPetParamBank/JPetParamBank.h"
#include "JPetTaskIO/JPetInputHandler.h"

class JPetGATEData : public TObject
{
public:
  Int_t fRunID = 0;
  Int_t fEventID = 0;
  Int_t fSourceID = 0;
  float fSourcePosX;
  float fSourcePosY;
  float fSourcePosZ;
  double fTime = 0.0;
  float fEnergy = 0.0;
  float fGlobalPosX;
  float fGlobalPosY;
  float fGlobalPosZ;
  Int_t fGantryID = 0;
  Int_t fRSectorID = 0;
  Int_t fModuleID = 0;
  Int_t fSubmoduleID = 0;
  Int_t fCrystalID = 0;
  Int_t fLayerID = 0;
  Int_t fComptonPhantomID = 0;
  Int_t fComptonCrystalID = 0;
  Int_t fRayleighPhantomID = 0;
  Int_t fRayleighCrystalID = 0;
  float fAxialPos = 0.0;
  float fRotationAngle = 0.0;
  Char_t fComptVolName[300];
  Char_t fRayleighVolName[300];
};

class JPetInputHandlerGATE : public JPetInputHandler
{
public:
  JPetInputHandlerGATE();
  JPetInputHandlerGATE(const JPetInputHandlerGATE&) = delete;
  void operator=(const JPetInputHandlerGATE&) = delete;

  bool nextEntry() override;
  bool openInput(const char* inputFileName, const JPetParams& params) override;
  void closeInput() override;

  TObject& getEntry() override;

  bool setEntryRange(const jpet_options_tools::OptsStrAny& options) override;
  std::tuple<bool, long long, long long> calculateEntryRange(const jpet_options_tools::OptsStrAny& options) const override;

private:
  TFile* fFile;
  TTree* fTree;
  JPetGATEData fEntryData;
  const std::string kInputROOTFileTTreeName = "Input_ROOT_File_TTree_Name_std::string";
  const std::string kGATERootTreeName = "Singles";
};

#endif /* !JPETINPUTHANDLERGATE_H */
