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
#include <JPetGateParser/JPetGateParserTools.h>
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

  if (isOptionSet(fParams.getOptions(), kClockWindowTime))
  {
    fClockWindowTime = getOptionAsDouble(fParams.getOptions(), kClockWindowTime);
  }
  if (isOptionSet(fParams.getOptions(), kMakeHistogramsParamKey))
  {
    fMakeHisto = getOptionAsBool(fParams.getOptions(), kMakeHistogramsParamKey);
  }
  if (isOptionSet(fParams.getOptions(), kEnergyThresholdParamKey))
  {
    fExperimentalThreshold = getOptionAsDouble(fParams.getOptions(), kEnergyThresholdParamKey);
  }

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
	  fTime_ps = (mcEntry->fTime)*pow(10.,12);	// switch to ps

	  if (!JPetGateParserTools::checkIfInCurrentTimeWindow(fTime_ps, fWindowNumber, fClockWindowTime) && fStoredMCHits.size() != 0)
	  {
		saveHits();
    	fStoredMCHits.clear();
		fStoredRecoHits.clear();
	  }
	  fWindowNumber = (unsigned long long int)(fTime_ps/fClockWindowTime);	// period of 'unsigned long long int' is too long to worry about crossing the limit

	  if (JPetGateParserTools::checkIfInCurrentTimeWindow(fTime_ps, fWindowNumber, fClockWindowTime))
	  {
	  fTimeInClockWindow = fTime_ps - (double)fWindowNumber*fClockWindowTime;
	  fGlobalPosX_cm = mcEntry->fGlobalPosX/10.;	// switch to cm
	  fGlobalPosY_cm = mcEntry->fGlobalPosY/10.;	// switch to cm
	  fGlobalPosZ_cm = mcEntry->fGlobalPosZ/10.;	// switch to cm
	  fEnergy_keV = mcEntry->fEnergy*pow(10.,3);	// switch to keV

	  const JPetParamBank& paramBank = getParamBank();

      JPetRawMCHit rawHit;
      rawHit.setTime(fTimeInClockWindow);
      rawHit.setEnergy(fEnergy_keV);
      rawHit.setPosX(fGlobalPosX_cm);
      rawHit.setPosY(fGlobalPosY_cm);
      rawHit.setPosZ(fGlobalPosZ_cm);
      rawHit.setScin(paramBank.getScin(JPetGateParserTools::mapScintillatorFromGate(mcEntry->fRSectorID, mcEntry->fCrystalID)));
      
      JPetMCRecoHit recoHit = JPetGateParserTools::reconstructHit(rawHit, fExperimentalParametrizer);
      
	  if (JPetGateParserTools::isHitReconstructed(recoHit, fExperimentalThreshold))
	  {
		fStoredMCHits.push_back(rawHit);
    	fStoredRecoHits.push_back(recoHit);
	  }
	  }
    }
  }
  catch (std::bad_cast& bc)
  {
    std::cerr << "bad_cast caught: " << bc.what() << std::endl;
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
  for (const auto& mcHit : fStoredMCHits)
  {
    dynamic_cast<JPetTimeWindowMC*>(fOutputEvents)->addMCHit<JPetRawMCHit>(mcHit);
	if (fMakeHisto)
	{
	  getStatistics().getHisto1D("gen_hits_per_time_window")->Fill(fStoredMCHits.size());
	  getStatistics().getHisto1D("gen_hits_z_pos")->Fill(mcHit.getPosZ());
	  getStatistics().getHisto2D("gen_hits_xy_pos")->Fill(mcHit.getPosX(), mcHit.getPosY());
	  getStatistics().getHisto1D("gen_hit_time")->Fill(mcHit.getTime());
	  getStatistics().getHisto1D("gen_hit_eneDepos")->Fill(mcHit.getEnergy());
	}
  }

  for (const auto& recoHit : fStoredRecoHits)
  {
    dynamic_cast<JPetTimeWindow*>(fOutputEvents)->add<JPetMCRecoHit>(recoHit);
	if (fMakeHisto)
	{
	  getStatistics().getHisto1D("rec_hits_per_time_window")->Fill(fStoredRecoHits.size());
	  getStatistics().getHisto1D("rec_hits_z_pos")->Fill(recoHit.getPosZ());
	  getStatistics().getHisto2D("rec_hits_xy_pos")->Fill(recoHit.getPosX(), recoHit.getPosY());
	  getStatistics().getHisto1D("rec_hit_time")->Fill(recoHit.getTime());
	  getStatistics().getHisto1D("rec_hit_eneDepos")->Fill(recoHit.getEnergy());
	}
  }

  fStoredMCHits.clear();
  fStoredRecoHits.clear();
}

void JPetGateParser::initialiseBasicHistograms()
{
  getStatistics().createHistogram(new TH1F("gen_hits_per_time_window", "Number of Hits in Time Window;Multiplicity", 101, -0.5, 500.5));

  getStatistics().createHistogram(new TH1F("gen_hits_z_pos", "Hits 'Z' position;[cm];", 100, -60.0, 60.0));

  getStatistics().createHistogram(new TH2F("gen_hits_xy_pos", "Hits 'XY' position;[cm];[cm]", 121, -60.5, 60.5, 121, -60.5, 60.5));

  getStatistics().createHistogram(new TH1F("gen_hit_time", "Hit time;[ps];", 20000, 0.0, fClockWindowTime));

  getStatistics().createHistogram(new TH1F("gen_hit_eneDepos", "Hit energy deposition;[keV];", 750, 0.0, 1500.0));

  getStatistics().createHistogram(new TH1F("rec_hits_per_time_window", "Number of Hits in Time Window;Multiplicity", 101, -0.5, 500.5));

  getStatistics().createHistogram(new TH1F("rec_hits_z_pos", "Hits 'Z' position;[cm];", 100, -60.0, 60.0));

  getStatistics().createHistogram(new TH2F("rec_hits_xy_pos", "Hits 'XY' position;[cm];[cm]", 121, -60.5, 60.5, 121, -60.5, 60.5));

  getStatistics().createHistogram(new TH1F("rec_hit_time", "Hit time;[ps];", 20000, 0.0, fClockWindowTime));

  getStatistics().createHistogram(new TH1F("rec_hit_eneDepos", "Hit energy deposition;[keV];", 750, 0.0, 1500.0));
}
