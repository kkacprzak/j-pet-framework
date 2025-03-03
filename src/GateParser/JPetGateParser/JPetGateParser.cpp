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

  if (fMakeHisto)
  {
    initialiseBasicHistograms();
  }

  INFO("GATE MC Hit wrapper started.");

  return true;
}

int JPetGateParser::mapScintillatorFromGate(JPetGATEData* mcEntry)
{
  if (mcEntry->fRSectorID < 6)
  {
	return 266 + mcEntry->fCrystalID - 13*mcEntry->fRSectorID;
  }
  return 266 + mcEntry->fCrystalID - 13*mcEntry->fRSectorID + 312;
}

bool JPetGateParser::checkIfInCurrentTimeWindow(double fTime_us_inTimeWindow, unsigned long long int fWindowNumber)
{
  double timeBasedOnPreviousTimeWindow = fTime_us_inTimeWindow - (double)fWindowNumber*fClockWindowTime_us;
  if (timeBasedOnPreviousTimeWindow < fClockWindowTime_us)
  {
	return true;
  }
  return false;
}

bool JPetGateParser::exec()
{
  try
  {
    if (auto& mcEntry = dynamic_cast<JPetGATEData* const>(fEvent))
    {
	  fTime_us_inTimeWindow = (mcEntry->fTime)*pow(10.,6);	// switch to us

	  if (!checkIfInCurrentTimeWindow(fTime_us_inTimeWindow, fWindowNumber) && fStoredMCHits.size() != 0)
	  {
		saveHits();
    	fStoredMCHits.clear();
		fStoredRecoHits.clear();
	  }
	  fWindowNumber = (unsigned long long int)(fTime_us_inTimeWindow/fClockWindowTime_us);	// period of 'unsigned long long int' is too long to worry about crossing the limit

	  if (checkIfInCurrentTimeWindow(fTime_us_inTimeWindow, fWindowNumber))
	  {
	  double timeBasedOnPreviousTimeWindow = fTime_us_inTimeWindow - fWindowNumber*fClockWindowTime_us;
	  fTime_ps_inTimeWindow = timeBasedOnPreviousTimeWindow*pow(10.,6);	// switch to ps
	  fGlobalPosX_cm = mcEntry->fGlobalPosX/10.;	// switch to cm
	  fGlobalPosY_cm = mcEntry->fGlobalPosY/10.;	// switch to cm
	  fGlobalPosZ_cm = mcEntry->fGlobalPosZ/10.;	// switch to cm
	  fEnergy_keV = mcEntry->fEnergy*pow(10.,3);	// switch to keV

	  const JPetParamBank& paramBank = getParamBank();

      JPetRawMCHit rawHit;
      rawHit.setTime(fTime_ps_inTimeWindow);
      rawHit.setEnergy(fEnergy_keV);
      rawHit.setPosX(fGlobalPosX_cm);
      rawHit.setPosY(fGlobalPosY_cm);
      rawHit.setPosZ(fGlobalPosZ_cm);
      rawHit.setScin(paramBank.getScin(mapScintillatorFromGate(mcEntry)));
      
      JPetMCRecoHit recoHit;	// TODO add time, 'z' smearings
      recoHit.setTime(rawHit.getTime());
      recoHit.setEnergy(rawHit.getEnergy());
      recoHit.setPosX(rawHit.getScin().getCenterX());
      recoHit.setPosY(rawHit.getScin().getCenterY());
      recoHit.setPosZ(rawHit.getPosZ());
      
	  if (fEnergy_keV > fExperimentalThreshold)
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
  getStatistics().createHistogram(new TH1F("gen_hits_per_time_window", "Number of Hits in Time Window", 101, -0.5, 500.5));

  getStatistics().createHistogram(new TH1F("gen_hits_z_pos", "hits Z position", 100, -60.0, 60.0));

  getStatistics().createHistogram(new TH2F("gen_hits_xy_pos", "hits XY pos", 121, -60.5, 60.5, 121, -60.5, 60.5));

  getStatistics().createHistogram(new TH1F("gen_hit_time", "hit time", 20000, 0.0, 20000000.0));

  getStatistics().createHistogram(new TH1F("gen_hit_eneDepos", "hit energy deposition", 750, 0.0, 1500.0));

  getStatistics().createHistogram(new TH1F("rec_hits_per_time_window", "Number of Hits in Time Window", 101, -0.5, 500.5));

  getStatistics().createHistogram(new TH1F("rec_hits_z_pos", "hits Z position", 100, -60.0, 60.0));

  getStatistics().createHistogram(new TH2F("rec_hits_xy_pos", "hits XY pos", 121, -60.5, 60.5, 121, -60.5, 60.5));

  getStatistics().createHistogram(new TH1F("rec_hit_time", "hit time", 20000, 0.0, 20000000.0));

  getStatistics().createHistogram(new TH1F("rec_hit_eneDepos", "hit energy deposition", 750, 0.0, 1500.0));
}
