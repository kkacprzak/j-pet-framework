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
 *  @file JPetGateParser.cpp
 */

#include <JPetAnalysisTools/JPetAnalysisTools.h>
#include <JPetGateParser/JPetGateParser.h>
#include <JPetGeantParser/JPetGeantParserTools.h>
#include <JPetOptionsTools/JPetOptionsTools.h>
#include <JPetScin/JPetScin.h>
#include <JPetTaskIO/JPetInputHandlerGATE.h>
#include <JPetWriter/JPetWriter.h>

#include <TMath.h>
#include <array>
#include <cmath>
#include <iostream>
#include <string>

using namespace jpet_options_tools;

JPetGateParser::JPetGateParser(const char* name) : JPetUserTask(name) {}

JPetGateParser::~JPetGateParser() {}

bool JPetGateParser::init()
{
  fOutputEvents = new JPetTimeWindowMC("JPetMCRecoHit", "JPetRawMCHit", "JPetMCDecayTree");
  auto opts = getOptions();

  if (isOptionSet(fParams.getOptions(), kMakeHistogramsParamKey))
  {
    fMakeHisto = getOptionAsBool(fParams.getOptions(), kMakeHistogramsParamKey);
  }
  if (isOptionSet(fParams.getOptions(), kEnergyThresholdParamKey))
  {
    fExperimentalThreshold = getOptionAsDouble(fParams.getOptions(), kEnergyThresholdParamKey);
  }
  if (isOptionSet(fParams.getOptions(), kSeedParamKey))
  {
    fSeed = getOptionAsInt(fParams.getOptions(), kSeedParamKey);
  }

  JPetGeantParserTools::setSeedTogRandom(getOriginalSeed());
  INFO("Seed value used for resolution smearing of MC simulation data: " << boost::lexical_cast<std::string>(getOriginalSeed()));

  loadSmearingOptionsAndSetupExperimentalParametrizer();

  if (fMakeHisto)
  {
    initialiseBasicHistograms();
  }

  INFO("GATE MC Hit wrapper started.");

  return true;
}

void JPetGateParser::loadSmearingOptionsAndSetupExperimentalParametrizer()
{
  std::vector<double> timeSmearingParameters;
  if (isOptionSet(fParams.getOptions(), kTimeSmearingParametersParamKey))
  {
    timeSmearingParameters = getOptionAsVectorOfDoubles(fParams.getOptions(), kTimeSmearingParametersParamKey);
  }

  std::string timeSmearingFormula;
  if (isOptionSet(fParams.getOptions(), kTimeSmearingFunctionParamKey))
  {
    timeSmearingFormula = getOptionAsString(fParams.getOptions(), kTimeSmearingFunctionParamKey);
  }

  std::vector<double> timeSmearingLimits;
  if (isOptionSet(fParams.getOptions(), kTimeSmearingFunctionLimitsParamKey))
  {
    timeSmearingLimits = getOptionAsVectorOfDoubles(fParams.getOptions(), kTimeSmearingFunctionLimitsParamKey);
  }

  std::vector<double> energySmearingParameters;
  if (isOptionSet(fParams.getOptions(), kEnergySmearingParametersParamKey))
  {
    energySmearingParameters = getOptionAsVectorOfDoubles(fParams.getOptions(), kEnergySmearingParametersParamKey);
  }

  std::string energySmearingFormula;
  if (isOptionSet(fParams.getOptions(), kEnergySmearingFunctionParamKey))
  {
    energySmearingFormula = getOptionAsString(fParams.getOptions(), kEnergySmearingFunctionParamKey);
  }

  std::vector<double> energySmearingLimits;
  if (isOptionSet(fParams.getOptions(), kEnergySmearingFunctionLimitsParamKey))
  {
    energySmearingLimits = getOptionAsVectorOfDoubles(fParams.getOptions(), kEnergySmearingFunctionLimitsParamKey);
  }

  std::vector<double> zPositionSmearingParameters;
  if (isOptionSet(fParams.getOptions(), kZPositionSmearingParametersParamKey))
  {
    zPositionSmearingParameters = getOptionAsVectorOfDoubles(fParams.getOptions(), kZPositionSmearingParametersParamKey);
  }

  std::string zPositionSmearingFormula;
  if (isOptionSet(fParams.getOptions(), kZPositionSmearingFunctionParamKey))
  {
    zPositionSmearingFormula = getOptionAsString(fParams.getOptions(), kZPositionSmearingFunctionParamKey);
  }

  std::vector<double> zPositionSmearingLimits;
  if (isOptionSet(fParams.getOptions(), kZPositionSmearingFunctionLimitsParamKey))
  {
    zPositionSmearingLimits = getOptionAsVectorOfDoubles(fParams.getOptions(), kZPositionSmearingFunctionLimitsParamKey);
  }

  if (isOptionSet(fParams.getOptions(), kUseDefaultZSmearingKey))
  {
    fUseDefaultZSmearing = getOptionAsDouble(fParams.getOptions(), kUseDefaultZSmearingKey);
  }

  if (isOptionSet(fParams.getOptions(), kUseDefaultTimeSmearingKey))
  {
    fUseDefaultTimeSmearing = getOptionAsDouble(fParams.getOptions(), kUseDefaultTimeSmearingKey);
  }

  if (isOptionSet(fParams.getOptions(), kUseDefaultEnergySmearingKey))
  {
    fUseDefaultEnergySmearing = getOptionAsDouble(fParams.getOptions(), kUseDefaultEnergySmearingKey);
  }

  if (isOptionSet(fParams.getOptions(), kDefaultZSmearingSigmaKey))
  {
    fDefaultZSmearingSigma = getOptionAsDouble(fParams.getOptions(), kDefaultZSmearingSigmaKey);
  }

  if (isOptionSet(fParams.getOptions(), kDefaultTimeSmearingSigmaKey))
  {
    fDefaultTimeSmearingSigma = getOptionAsDouble(fParams.getOptions(), kDefaultTimeSmearingSigmaKey);
  }

  if (isOptionSet(fParams.getOptions(), kDefaultTimeSmearingThresholdEnergyKey))
  {
    fDefaultTimeSmearingThresholdEnergy = getOptionAsDouble(fParams.getOptions(), kDefaultTimeSmearingThresholdEnergyKey);
  }

  if (isOptionSet(fParams.getOptions(), kDefaultTimeSmearingReferenceEnergyKey))
  {
    fDefaultTimeSmearingReferenceEnergy = getOptionAsDouble(fParams.getOptions(), kDefaultTimeSmearingReferenceEnergyKey);
  }

  if (isOptionSet(fParams.getOptions(), kDefaultEnergySmearingFractionKey))
  {
    fDefaultEnergySmearingFraction = getOptionAsDouble(fParams.getOptions(), kDefaultEnergySmearingFractionKey);
  }

  fExperimentalParametrizer.setSmearingFunctions({{timeSmearingFormula, timeSmearingParameters},
                                                  {energySmearingFormula, energySmearingParameters},
                                                  {zPositionSmearingFormula, zPositionSmearingParameters}});

  std::vector<std::pair<double, double>> limits;

  if (timeSmearingLimits.size() == 2)
  {
    limits.push_back({timeSmearingLimits[0], timeSmearingLimits[1]});
  }
  else
  {
    limits.push_back({-1, -1});
  }

  if (energySmearingLimits.size() == 2)
  {
    limits.push_back({energySmearingLimits[0], energySmearingLimits[1]});
  }
  else
  {
    limits.push_back({-1, -1});
  }

  if (zPositionSmearingLimits.size() == 2)
  {
    limits.push_back({zPositionSmearingLimits[0], zPositionSmearingLimits[1]});
  }
  else
  {
    limits.push_back({-1, -1});
  }

  fExperimentalParametrizer.setSmearingFunctionLimits(limits);

  fExperimentalParametrizer.setShouldUseDefaultSmearing(fUseDefaultZSmearing, fUseDefaultTimeSmearing, fUseDefaultEnergySmearing);

  if (fUseDefaultZSmearing)
  {
    fExperimentalParametrizer.setDefaultZSmearingSigma(fDefaultZSmearingSigma);
  }

  if (fUseDefaultTimeSmearing)
  {
    fExperimentalParametrizer.setDefaultTimeSmearingSigma(fDefaultTimeSmearingSigma);
    fExperimentalParametrizer.setDefaultTimeSmearingReferenceEnergy(fDefaultTimeSmearingReferenceEnergy);
    fExperimentalParametrizer.setDefaultTimeSmearingThresholdEnergy(fDefaultTimeSmearingThresholdEnergy);
  }

  if (fUseDefaultEnergySmearing)
  {
    fExperimentalParametrizer.setDefaultEnergySmearingFraction(fDefaultEnergySmearingFraction);
  }
}

bool JPetGateParser::exec()
{
  try
  {
    if (auto& mcEntry = dynamic_cast<JPetGATEData* const>(fEvent))
    {
      // dummy read and reconstruct hits here
      // create JPetRawMCHit
      // create JPetMCRecoHit using smearing functions
      JPetRawMCHit rawHit;
      rawHit.setTime(mcEntry->fTime);
      // std::cout << "Hit time " << mcEntry->fTime << " " << rawHit.getTime() << std::endl;
      JPetMCRecoHit recoHit;
      recoHit.setTime(rawHit.getTime() * 1);
      recoHit.setMCindex(fStoredMCHits.size());
      fStoredMCHits.push_back(rawHit);
      fStoredRecoHits.push_back(recoHit);
    }
  }
  catch (std::bad_cast& bc)
  {
    std::cerr << "bad_cast caught: " << bc.what() << std::endl;
  }

  // Adding 5 hits to time window
  if (fStoredMCHits.size() >= 5)
  {
    saveHits();
    fStoredMCHits.clear();
    fStoredRecoHits.clear();
  }
  return true;
}

bool JPetGateParser::terminate()
{
  INFO("GATE MC events processing ended");

  return true;
}

void JPetGateParser::saveHits()
{
  // std::cout << "Hit number " << fStoredMCHits.size() << fStoredRecoHits.size() << std::endl;

  for (const auto& mcHit : fStoredMCHits)
  {
    dynamic_cast<JPetTimeWindowMC*>(fOutputEvents)->addMCHit<JPetRawMCHit>(mcHit);
  }

  for (const auto& recoHit : fStoredRecoHits)
  {
    dynamic_cast<JPetTimeWindow*>(fOutputEvents)->add<JPetMCRecoHit>(recoHit);
  }

  if (fMakeHisto)
  {
    // Fill histograms
    getStatistics().fillHistogram("hits_per_time_window", fStoredRecoHits.size());
  }

  fStoredMCHits.clear();
  fStoredRecoHits.clear();
}

void JPetGateParser::initialiseBasicHistograms()
{
  getStatistics().createHistogram(new TH1F("hits_per_time_window", "Number of Hits in Time Window", 101, -0.5, 500.5));

  getStatistics().createHistogram(new TH1F("hits_z_pos", "hits Z position", 100, -60.0, 60.0));

  getStatistics().createHistogram(new TH2F("hits_xy_pos", "hits XY pos", 121, -60.5, 60.5, 121, -60.5, 60.5));

  getStatistics().createHistogram(new TH1F("rec_hit_time", "hit time", 100, 0.0, 15000.0));

  getStatistics().createHistogram(new TH1F("rec_hit_eneDepos", "hit ene deposition", 750, 0.0, 1500.0));
}

unsigned long JPetGateParser::getOriginalSeed() const { return fSeed; }
