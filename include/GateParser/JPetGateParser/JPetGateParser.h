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

  unsigned long getOriginalSeed() const;

protected:
  bool fMakeHisto = true;

  double fExperimentalThreshold = 10.0;
  unsigned long fSeed = 0.0;

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
  const std::string kMakeHistogramsParamKey = "GeantParser_MakeHistograms_bool";
  const std::string kEnergyThresholdParamKey = "GeantParser_EnergyThreshold_double";

  const std::string kTimeSmearingParametersParamKey = "GeantParser_TimeSmearingParameters_std::vector<double>";
  const std::string kTimeSmearingFunctionParamKey = "GeantParser_TimeSmearingFunction_std::string";
  const std::string kTimeSmearingFunctionLimitsParamKey = "GeantParser_TimeSmearingFunctionLimits_std::vector<double>";

  const std::string kEnergySmearingParametersParamKey = "GeantParser_EnergySmearingParameters_std::vector<double>";
  const std::string kEnergySmearingFunctionParamKey = "GeantParser_EnergySmearingFunction_std::string";
  const std::string kEnergySmearingFunctionLimitsParamKey = "GeantParser_EnergySmearingFunctionLimits_std::vector<double>";

  const std::string kZPositionSmearingParametersParamKey = "GeantParser_ZPositionSmearingParameters_std::vector<double>";
  const std::string kZPositionSmearingFunctionParamKey = "GeantParser_ZPositionSmearingFunction_std::string";
  const std::string kZPositionSmearingFunctionLimitsParamKey = "GeantParser_ZPositionSmearingFunctionLimits_std::vector<double>";

  const std::string kUseDefaultZSmearingKey = "GeantParser_UseDefaultZSmearing_bool";
  const std::string kDefaultZSmearingSigmaKey = "GeantParser_DefaultZSmearingSigma_double";
  const std::string kUseDefaultTimeSmearingKey = "GeantParser_UseDefaultTimeSmearing_bool";

  const std::string kDefaultTimeSmearingSigmaKey = "GeantParser_DefaultTimeSmearingSigma_double";
  const std::string kDefaultTimeSmearingThresholdEnergyKey = "GeantParser_DefaultTimeSmearingThresholdEnergy_double";
  const std::string kDefaultTimeSmearingReferenceEnergyKey = "GeantParser_DefaultTimeSmearingReferenceEnergy_double";

  const std::string kUseDefaultEnergySmearingKey = "GeantParser_UseDefaultEnergySmearing_bool";
  const std::string kDefaultEnergySmearingFractionKey = "GeantParser_DefaultEnergySmearingFraction_double";

  const std::string kSeedParamKey = "GeantParser_Seed_int";

  ///< MC hits into single time window when it contains enough hits
  std::vector<JPetRawMCHit> fStoredMCHits;
  ///< RECONSTRUCTED MC hits into single time window when it contains enough hits
  std::vector<JPetMCRecoHit> fStoredRecoHits;

  void loadSmearingOptionsAndSetupExperimentalParametrizer();

  void saveHits();

  void initialiseBasicHistograms();
};

#endif /* !JPETGATEPARSER_H */
