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
 *  @file JPetInputHandlerGATE.cpp
 */

#include <TDirectory.h>
#include <TFile.h>
#include <TH1F.h>
#include <TString.h>
#include <iostream>
#include <string>
#include <vector>

#include "JPetCommonTools/JPetCommonTools.h"
#include "JPetLoggerInclude.h"
#include "JPetOptionsGenerator/JPetOptionsGeneratorTools.h"
#include "JPetOptionsTools/JPetOptionsTools.h"
#include "JPetParamBank/JPetParamBank.h"
#include "JPetTaskIO/JPetInputHandlerGATE.h"
#include "JPetTaskIO/JPetTaskIOTools.h"

JPetInputHandlerGATE::JPetInputHandlerGATE() {}

bool JPetInputHandlerGATE::openInput(const char* inputFilename, const JPetParams& params)
{
  using namespace jpet_options_tools;
  auto options = params.getOptions();

  std::string treeName = kGATERootTreeName;
  if (isOptionSet(options, kInputROOTFileTTreeName))
  {
    treeName = getOptionAsString(options, kInputROOTFileTTreeName);
  }

  fFile = new TFile(inputFilename);
  fTree = dynamic_cast<TTree*>(fFile->Get(treeName.c_str()));
  fTree->SetBranchAddress("runID", &fEntryData.fRunID);
  fTree->SetBranchAddress("eventID", &fEntryData.fEventID);
  fTree->SetBranchAddress("sourceID", &fEntryData.fSourceID);
  fTree->SetBranchAddress("sourcePosX", &fEntryData.fSourcePosX);
  fTree->SetBranchAddress("sourcePosY", &fEntryData.fSourcePosY);
  fTree->SetBranchAddress("sourcePosZ", &fEntryData.fSourcePosZ);
  fTree->SetBranchAddress("time", &fEntryData.fTime);
  fTree->SetBranchAddress("energy", &fEntryData.fEnergy);
  fTree->SetBranchAddress("globalPosX", &fEntryData.fGlobalPosX);
  fTree->SetBranchAddress("globalPosY", &fEntryData.fGlobalPosY);
  fTree->SetBranchAddress("globalPosZ", &fEntryData.fGlobalPosZ);
  fTree->SetBranchAddress("gantryID", &fEntryData.fGantryID);
  fTree->SetBranchAddress("rsectorID", &fEntryData.fRSectorID);
  fTree->SetBranchAddress("moduleID", &fEntryData.fModuleID);
  fTree->SetBranchAddress("submoduleID", &fEntryData.fSubmoduleID);
  fTree->SetBranchAddress("crystalID", &fEntryData.fCrystalID);
  fTree->SetBranchAddress("layerID", &fEntryData.fLayerID);
  fTree->SetBranchAddress("comptonPhantom", &fEntryData.fComptonPhantomID);
  fTree->SetBranchAddress("comptonCrystal", &fEntryData.fComptonCrystalID);
  fTree->SetBranchAddress("RayleighPhantom", &fEntryData.fRayleighPhantomID);
  fTree->SetBranchAddress("RayleighCrystal", &fEntryData.fRayleighCrystalID);
  fTree->SetBranchAddress("axialPos", &fEntryData.fAxialPos);
  fTree->SetBranchAddress("rotationAngle", &fEntryData.fRotationAngle);
  fTree->SetBranchAddress("comptVolName", &fEntryData.fComptVolName);
  fTree->SetBranchAddress("RayleighVolName", &fEntryData.fRayleighVolName);

  return nextEntry();
}

void JPetInputHandlerGATE::closeInput() { fFile->Close(); }

TObject& JPetInputHandlerGATE::getEntry() { return fEntryData; }

bool JPetInputHandlerGATE::nextEntry()
{
  if (fEntryRange.currentEntry == fEntryRange.lastEntry)
  {
    return false;
  }
  fEntryRange.currentEntry++;

  // Readint next entry from the GATE tree to an object
  int success = fTree->GetEntry(fEntryRange.currentEntry);

  if (success == 0 || success == -1)
  {
    return false;
  }

  return true;
}

bool JPetInputHandlerGATE::setEntryRange(const jpet_options_tools::OptsStrAny& options)
{
  bool isOK = false;
  auto firstEntry = 0ll;
  auto lastEntry = 0ll;
  std::tie(isOK, firstEntry, lastEntry) = calculateEntryRange(options);
  if (!isOK)
  {
    ERROR("Some error occured in setEntryRange");
    return false;
  }
  fEntryRange.firstEntry = firstEntry;
  fEntryRange.lastEntry = lastEntry;
  fEntryRange.currentEntry = firstEntry;
  return true;
}

std::tuple<bool, long long, long long> JPetInputHandlerGATE::calculateEntryRange(const jpet_options_tools::OptsStrAny& options) const
{
  auto totalEntries = fTree ? fTree->GetEntries() : 0ll;
  return JPetTaskIOTools::setUserLimits(options, totalEntries);
}
