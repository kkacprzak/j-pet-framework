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
 *  @file JPetInputHandlerNTU.h
 */

#ifndef JPETINPUTHANDLERNTU_H
#define JPETINPUTHANDLERNTU_H

#include <fstream>
#include <string>

#include <TObject.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <TVector3.h>

#include "JPetParamBank/JPetParamBank.h"
#include "JPetTaskIO/JPetInputHandler.h"

class JPetNTUData : public TObject
{
public:
  Int_t fNumberOfHits;
  std::vector<double>* fHitTimes = nullptr;
  std::vector<TVector3>* fHitPos = nullptr;
  std::vector<double>* fHitTOTs = nullptr;
  std::vector<UInt_t>* fHitScinIDs = nullptr;
};

class JPetInputHandlerNTU : public JPetInputHandler
{
public:
  JPetInputHandlerNTU();
  JPetInputHandlerNTU(const JPetInputHandlerNTU&) = delete;
  void operator=(const JPetInputHandlerNTU&) = delete;

  bool nextEntry() override;
  bool openInput(const char* inputFileName, const JPetParams& params) override;
  void closeInput() override;

  TObject& getEntry() override;

  bool setEntryRange(const jpet_options_tools::OptsStrAny& options) override;
  std::tuple<bool, long long, long long> calculateEntryRange(const jpet_options_tools::OptsStrAny& options) const override;

private:
  TFile* fFile;
  TTree* fTree;
  JPetNTUData fEntryData;
  const std::string kNTURootTreeName = "T";
};

#endif /* !JPETINPUTHANDLERNTU_H */
