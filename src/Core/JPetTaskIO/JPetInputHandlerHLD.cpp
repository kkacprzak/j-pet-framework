/**
 *  @copyright Copyright 2021 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetInputHandlerHLD.cpp
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
#include "JPetTaskIO/JPetInputHandlerHLD.h"
#include "JPetTaskIO/JPetTaskIOTools.h"

#include "unpacker.hpp"

JPetInputHandlerHLD::JPetInputHandlerHLD() {}

bool JPetInputHandlerHLD::openInput(const char* inputFilename, const JPetParams& params)
{
  using namespace jpet_options_tools;
  auto options = params.getOptions();

  fFile.open(inputFilename, std::ios::in | std::ios::binary);

  if (!fFile)
  {
    ERROR(Form("Failed to open input file: %s", inputFilename));
    return false;
  }

  if (detector_type_checker::getDetectorType(options) == detector_type_checker::DetectorType::kBarrel)
  {
    fDetectorType = detector_type_checker::DetectorType::kBarrel;

    // Large Barrel needs a TOT strecher and TDC nonlinearity calibration
    if (!loadCalibsBarrel(params))
    {
      WARNING("Failed to load TOT or TDC calibration. Unpacker will proceed without calibration.");
    }
  }
  else if (detector_type_checker::getDetectorType(options) == detector_type_checker::DetectorType::kModular)
  {
    fDetectorType = detector_type_checker::DetectorType::kModular;
    // TDC calibrtion is loaded for Modular
    if (!loadCalibModular(params))
    {
      WARNING("Failed to load TDC nonlinearity calibration. Unpacker will proceed without calibration.");
    }
  }

  return nextEntry(); /// load first entry ready for `get_entry`
}

void JPetInputHandlerHLD::closeInput() { fFile.close(); }

TObject& JPetInputHandlerHLD::getEntry() { return fEntryData; }

bool JPetInputHandlerHLD::nextEntry()
{
  if (fEntryRange.currentEntry == fEntryRange.lastEntry)
  {
    return false;
  }
  fEntryRange.currentEntry++;

  int success = 0;

  if (fDetectorType == detector_type_checker::DetectorType::kModular)
  {
    success = unpacker::get_time_window_modular(fEntryData.fMetaData, fEntryData.fOriginalData, fEntryData.fFilteredData, fEntryData.fPreprocData,
                                                fFile, fTDCCalib);
  }
  else if (fDetectorType == detector_type_checker::DetectorType::kBarrel)
  {
    success = unpacker::get_time_window_barrel(fEntryData.fMetaData, fEntryData.fOriginalData, fEntryData.fFilteredData, fEntryData.fPreprocData,
                                               fFile, fTDCCalib);
  }

  if (success == 0)
  {
    return false;
  }

  return true;
}

bool JPetInputHandlerHLD::setEntryRange(const jpet_options_tools::OptsStrAny& options)
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

std::tuple<bool, long long, long long> JPetInputHandlerHLD::calculateEntryRange(const jpet_options_tools::OptsStrAny& options) const
{
  auto totalEntries = 1000000000ll;
  // @todo: consider scanning the HLD file first to determine the number of time windows inside
  WARNING("Unpacker of the HLD files is not able to determine the total number of events. Progressbar display may not be reliable.");
  return JPetTaskIOTools::setUserLimits(options, totalEntries);
}

bool JPetInputHandlerHLD::loadCalibModular(const JPetParams& params)
{
  std::string tdcFileName = "";
  if (isOptionSet(params.getOptions(), kTDCOffsetCalibKey))
  {
    tdcFileName = getOptionAsString(params.getOptions(), kTDCOffsetCalibKey);
  }
  else
  {
    WARNING("Path to file with TDC nonlinearity calibrations was not set. Skipping TDC calibration.");
    return false;
  }

  TFile* tdcCalibRootFile = new TFile(tdcFileName.c_str(), "READ");
  if (!tdcCalibRootFile->IsOpen())
  {
    WARNING(TString::Format("Unable to open file: %s. Skipping TDC calibration.", tdcFileName.c_str()));
    return false;
  }

  int run_id = getRunNumber(params.getOptions());
  for (auto& channel : params.getParamManager()->getChannels(run_id))
  {
    uint32_t channel_no = channel.second->getID();
    uint32_t address = channel.second->getDataModule().getTBRNetAddress();
    uint32_t local_channel_no = channel_no - channel.second->getDataModule().getChannelsOffset();

    TH1F* corr_histo = dynamic_cast<TH1F*>(tdcCalibRootFile->FindObjectAny(TString::Format("correction%d", channel_no)));
    if (corr_histo == nullptr)
    {
      WARNING(TString::Format("Missing TDC correction for channel %d", channel_no));
      continue;
    }

    std::vector<uint32_t> corr_vec(150);
    for (int i = 1; i < corr_histo->GetNbinsX(); ++i)
    {
      corr_vec[i - 1] = corr_histo->GetBinContent(i) * 1000.;
    }

    fTDCCalib[address][local_channel_no] = corr_vec;
  }

  // separately handle reference channels
  for (auto& dm : params.getParamManager()->getDataModules(run_id))
  {
    uint32_t address = dm.second->getTBRNetAddress();
    uint32_t local_channel_no = dm.second->getChannelsNumber() - 1;
    uint32_t channel_no = dm.second->getChannelsOffset() + local_channel_no;

    TH1F* corr_histo = dynamic_cast<TH1F*>(tdcCalibRootFile->FindObjectAny(TString::Format("correction%d", channel_no)));
    if (corr_histo == nullptr)
    {
      WARNING(TString::Format("Missing TDC correction for channel %d", channel_no));
      continue;
    }

    std::vector<uint32_t> corr_vec(150);
    for (int i = 1; i < corr_histo->GetNbinsX(); ++i)
    {
      corr_vec[i - 1] = corr_histo->GetBinContent(i) * 1000.;
    }

    fTDCCalib[address][local_channel_no] = corr_vec;
  }

  return true;
}

bool JPetInputHandlerHLD::loadCalibsBarrel(const JPetParams& params)
{
  // Getting calib files paths
  std::string tdcFileName = "";
  if (isOptionSet(params.getOptions(), kTDCOffsetCalibKey))
  {
    tdcFileName = getOptionAsString(params.getOptions(), kTDCOffsetCalibKey);
  }
  else
  {
    WARNING("Path to file with TDC nonlinearity calibrations was not set. Skipping TDC calibration.");
    return false;
  }

  std::string totFileName = "";
  if (isOptionSet(params.getOptions(), kTOTOffsetCalibKey))
  {
    totFileName = getOptionAsString(params.getOptions(), kTOTOffsetCalibKey);
  }
  else
  {
    WARNING("Path to file with TOT stretcher calibrations was not set. Skipping TOT calibration.");
    return false;
  }

  // Reading TDC calibration file
  TFile* tdcCalibRootFile = new TFile(tdcFileName.c_str(), "READ");
  if (!tdcCalibRootFile->IsOpen())
  {
    ERROR(TString::Format("Unable to open file: %s. Skipping TDC calibration.", tdcFileName.c_str()));
    return false;
  }

  // Getting offsets from the setup configuration
  std::unordered_map<uint32_t, uint32_t> tdc_offsets;
  int run_id = getRunNumber(params.getOptions());
  for (auto& dm : params.getParamManager()->getDataModules(run_id))
  {
    tdc_offsets[dm.second->getTBRNetAddress()] = dm.second->getChannelsOffset();
  }

  for (uint32_t i = 0; i < 2080; i++)
  {
    uint32_t channel_no = i;
    uint32_t address = 0;
    for (const auto& p : tdc_offsets)
    {
      if (i - (i % 65) == p.second)
      {
        address = p.first;
      }
    }

    // prepare tdc offset map for global channel mapping
    std::vector<uint32_t> vec(1);
    vec[0] = i - (i % 65);
    fTDCCalib[0x1111][address] = vec;
    if (address == 0)
    {
      ERROR("Unable to load cTDC alibrations");
      return false;
    }

    TH1F* corr_histo = dynamic_cast<TH1F*>(tdcCalibRootFile->FindObjectAny(TString::Format("correction%d", channel_no)));
    if (corr_histo == nullptr)
    {
      WARNING(TString::Format("Missing TDC correction for channel %d", channel_no));
      continue;
    }

    std::vector<uint32_t> corr_vec(500);
    for (int i = 1; i < corr_histo->GetNbinsX(); ++i)
    {
      corr_vec[i - 1] = corr_histo->GetBinContent(i);
    }

    // store the TDC non-linearity correction vectors
    fTDCCalib[0x2222][i] = corr_vec;
  }

  if (fTDCCalib[0x1111].size() != 32 && fTDCCalib[0x2222].size() != 2080)
  {
    ERROR("Failed loading calibrations for all 32 endpoints");
    return false;
  }

  // loading TOT calibrations into tdc_calib_t as id 0x3333
  TFile* totCalibRootFile = new TFile(totFileName.c_str(), "READ");
  if (!totCalibRootFile->IsOpen())
  {
    ERROR(TString::Format("Unable to open file: %s. Skipping TDC calibration.", totFileName.c_str()));
    return false;
  }

  TH1F* corr_histo = dynamic_cast<TH1F*>(totCalibRootFile->FindObjectAny("stretcher_offsets"));
  std::vector<uint32_t> vec(2100);
  for (int i = 0; i < 2100; i++)
  {
    vec[i] = corr_histo->GetBinContent(i + 1);
  }
  fTDCCalib[0x3333][0] = vec;

  return true;
}
