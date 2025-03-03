/**
 *  @copyright Copyright 2025 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  @file JPetGateParser.h
 */

#ifndef JPETGATEPARSER_H
#define JPETGATEPARSER_H

#include "Hits/JPetMCRecoHit/JPetMCRecoHit.h"
#include "JPetRawMCHit/JPetRawMCHit.h"
//#include "JPetSmearingFunctions/JPetSmearingFunctions.h"
#include "JPetUserTask/JPetUserTask.h"
#include <JPetTaskIO/JPetInputHandlerGATE.h>
#include <functional>
#include <map>
#include <tuple>
#include <vector>

class JPetWriter;

/**
 * @brief Module responsible for creating JPetMCHit from GATE MC simulations
 */
class JPetGateParser : public JPetUserTask
{

public:
  JPetGateParser(const char* name);
  virtual ~JPetGateParser();
  virtual bool init() override;
  virtual bool exec() override;
  virtual bool terminate() override;

  bool checkIfInCurrentTimeWindow(double fTime_us_inTimeWindow, unsigned long long int fWindowNumber);
  int mapScintillatorFromGate(JPetGATEData* mcEntry);

protected:
  double fClockWindowTime_us = 20.0;
  unsigned long long int fWindowNumber = 0;
  double fTime_us_inTimeWindow = 0.0;
  double fTime_ps_inTimeWindow = 0.0;
  float fEnergy_keV = 0.0;
  float fGlobalPosX_cm = 0.0;
  float fGlobalPosY_cm = 0.0;
  float fGlobalPosZ_cm = 0.0;

  bool fMakeHisto = true;

  double fExperimentalThreshold = 10.0;

  ///< internal variables
  const std::string kMakeHistogramsParamKey = "GateParser_MakeHistograms_bool";
  const std::string kEnergyThresholdParamKey = "GateParser_EnergyThreshold_double";

  std::vector<JPetRawMCHit> fStoredMCHits;
  std::vector<JPetMCRecoHit> fStoredRecoHits;

  void saveHits();

  void initialiseBasicHistograms();
};

#endif /* !JPETGATEPARSER_H */
