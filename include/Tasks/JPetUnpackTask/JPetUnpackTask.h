/**
 *  @copyright Copyright 2019 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetUnpackTask.h
 */

#ifndef JPETUNPACKTASK_H
#define JPETUNPACKTASK_H

#include "JPetTOMBChannel/JPetTOMBChannel.h"
#include "JPetUserTask/JPetUserTask.h"
// #include <boost/any.hpp>
#include <TH1.h>
#include <map>

class JPetWriter;

#ifdef __CINT__
#define override
#endif

/**
 * TODO Desc
 */
class JPetUnpackTask: public JPetUserTask
{
public:
  JPetUnpackTask(const char* name);
  virtual ~JPetUnpackTask();
  virtual bool init() override;
  virtual bool exec() override;
  virtual bool terminate() override;

private:
  const std::string kTDCnonlinearityCalibKey = "Unpacker_TDCnonlinearityCalib_std::string";
  const std::string kTOTOffsetCalibKey = "Unpacker_TOToffsetCalib_std::string";
  void saveTimeWindow(const std::vector<JPetTimeWindow>& allTimeSlots);
  std::string fTDCnonlinearityCalibFile;
  std::string fTOTOffsetCalibFile;
  TH1F* fTOTCalibHist = nullptr;
  TH1F* fTDCCalibHist = nullptr;
  bool fDoReverseHex = false;
  bool fDebugMode = true;
  std::ifstream* fHLDFile;
  void initialiseHistograms();

  int* pHDR;
  int* pSubHDR;

  // TODO - use -r option to determine number of entries to analyze
  int fEntriesToAnalyze;
  size_t fInitEntrySize = 0;
  size_t fEntrySize = 0;
  int fAnalyzedEntry = 0;
  long int fFileSize = 0;
  bool fFullSetup = true;
  std::map<unsigned long, int> fTDCOffsetsMap;

  TH1F ** fTDCCorrections = nullptr;
};

#endif /* !JPETUNPACKTASK_H */
