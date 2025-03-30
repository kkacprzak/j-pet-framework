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
 *  @file JPetInputHandlerNTU.cpp
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
#include "JPetTaskIO/JPetInputHandlerNTU.h"
#include "JPetTaskIO/JPetTaskIOTools.h"

JPetInputHandlerNTU::JPetInputHandlerNTU() {}

bool JPetInputHandlerNTU::openInput(const char* inputFilename, const JPetParams& params)
{
  using namespace jpet_options_tools;
  auto options = params.getOptions();

  fFile = new TFile(inputFilename);
  fTree = dynamic_cast<TTree*>(fFile->Get(kNTURootTreeName.c_str()));
  fTree->SetBranchAddress("nhits", &fEntryData.fNumberOfHits);
  fTree->SetBranchAddress("times", &fEntryData.fHitTimes);
  fTree->SetBranchAddress("tots", &fEntryData.fHitTOTs);
  fTree->SetBranchAddress("scins", &fEntryData.fHitScinIDs);
  fTree->SetBranchAddress("pos", &fEntryData.fHitPos);

  return nextEntry();
}

void JPetInputHandlerNTU::closeInput() { fFile->Close(); }

TObject& JPetInputHandlerNTU::getEntry() { return fEntryData; }

bool JPetInputHandlerNTU::nextEntry()
{
  if (fEntryRange.currentEntry == fEntryRange.lastEntry)
  {
    return false;
  }
  fEntryRange.currentEntry++;

  int success = fTree->GetEntry(fEntryRange.currentEntry);

  if (success == 0 || success == -1)
  {
    return false;
  }

  return true;
}

bool JPetInputHandlerNTU::setEntryRange(const jpet_options_tools::OptsStrAny& options)
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

std::tuple<bool, long long, long long> JPetInputHandlerNTU::calculateEntryRange(const jpet_options_tools::OptsStrAny& options) const
{
  auto totalEntries = fTree ? fTree->GetEntries() : 0ll;
  return JPetTaskIOTools::setUserLimits(options, totalEntries);
}
