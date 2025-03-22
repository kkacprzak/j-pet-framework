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
#include "JPetSmearingFunctions/JPetSmearingFunctions.h"
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

protected:
  double fClockWindowTime = 20000000.0;
  unsigned long long int fWindowNumber = 0;
  double fTime_ps = 0.0;
  double fTimeInClockWindow = 0.0;
  float fEnergy_keV = 0.0;
  float fGlobalPosX_cm = 0.0;
  float fGlobalPosY_cm = 0.0;
  float fGlobalPosZ_cm = 0.0;

  bool fMakeHisto = true;

  double fExperimentalThreshold = 10.0;

  bool fUseDefaultZSmearing = false;
  bool fUseDefaultTimeSmearing = false;
  bool fUseDefaultEnergySmearing = false;

  float fDefaultZSmearingSigma = 3.0;
  float fDefaultTimeSmearingSigma = 220.0;
  float fDefaultTimeSmearingThresholdEnergy = 200.0;
  float fDefaultTimeSmearingReferenceEnergy = 270.0;
  float fDefaultEnergySmearingFraction = 0.044;

  JPetHitExperimentalParametrizer fExperimentalParametrizer;

  ///< internal variables

  const std::string kClockWindowTime = "GateParser_ClockWindowTimeInps_double";

  const std::string kMakeHistogramsParamKey = "GateParser_MakeHistograms_bool";
  const std::string kEnergyThresholdParamKey = "GateParser_EnergyThreshold_double";

  const std::string kTimeSmearingParametersParamKey = "GateParser_TimeSmearingParameters_std::vector<double>";
  const std::string kTimeSmearingFunctionParamKey = "GateParser_TimeSmearingFunction_std::string";
  const std::string kTimeSmearingFunctionLimitsParamKey = "GateParser_TimeSmearingFunctionLimits_std::vector<double>";

  const std::string kEnergySmearingParametersParamKey = "GateParser_EnergySmearingParameters_std::vector<double>";
  const std::string kEnergySmearingFunctionParamKey = "GateParser_EnergySmearingFunction_std::string";
  const std::string kEnergySmearingFunctionLimitsParamKey = "GateParser_EnergySmearingFunctionLimits_std::vector<double>";

  const std::string kZPositionSmearingParametersParamKey = "GateParser_ZPositionSmearingParameters_std::vector<double>";
  const std::string kZPositionSmearingFunctionParamKey = "GateParser_ZPositionSmearingFunction_std::string";
  const std::string kZPositionSmearingFunctionLimitsParamKey = "GateParser_ZPositionSmearingFunctionLimits_std::vector<double>";

  const std::string kUseDefaultZSmearingKey = "GateParser_UseDefaultZSmearing_bool";
  const std::string kDefaultZSmearingSigmaKey = "GateParser_DefaultZSmearingSigma_double";

  const std::string kUseDefaultTimeSmearingKey = "GateParser_UseDefaultTimeSmearing_bool";
  const std::string kDefaultTimeSmearingSigmaKey = "GateParser_DefaultTimeSmearingSigma_double";
  const std::string kDefaultTimeSmearingThresholdEnergyKey = "GateParser_DefaultTimeSmearingThresholdEnergy_double";
  const std::string kDefaultTimeSmearingReferenceEnergyKey = "GateParser_DefaultTimeSmearingReferenceEnergy_double";

  const std::string kUseDefaultEnergySmearingKey = "GateParser_UseDefaultEnergySmearing_bool";
  const std::string kDefaultEnergySmearingFractionKey = "GateParser_DefaultEnergySmearingFraction_double";

  std::vector<JPetRawMCHit> fStoredMCHits;
  std::vector<JPetMCRecoHit> fStoredRecoHits;

  void loadSmearingOptionsAndSetupExperimentalParametrizer();

  void saveHits();

  void initialiseBasicHistograms();
};

#endif /* !JPETGATEPARSER_H */
