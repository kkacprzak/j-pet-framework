/**
 *  @copyright Copyright 2022 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetGateParserTools.cpp
 */

#include "JPetGateParser/JPetGateParserTools.h"
#include "JPetGateParser/JPetGateParser.h"
#include "JPetSmearingFunctions/JPetSmearingFunctions.h"
#include <TMath.h>

using namespace std;

int JPetGateParserTools::mapScintillatorFromGate(int rSectorID, int crystalID)
{
  if (rSectorID < 6)
  {
	return 266 + crystalID - 13*rSectorID;
  }
  return 266 + crystalID - 13*rSectorID + 312;
}

bool JPetGateParserTools::checkIfInCurrentTimeWindow(double fTime_ps_inTimeWindow, unsigned long long int fWindowNumber, double fClockWindowTime_ps)
{
  double timeBasedOnPreviousTimeWindow = fTime_ps_inTimeWindow - (double)fWindowNumber*fClockWindowTime_ps;
  if (timeBasedOnPreviousTimeWindow < fClockWindowTime_ps)
  {
	return true;
  }
  return false;
}

JPetMCRecoHit JPetGateParserTools::reconstructHit(JPetRawMCHit& mcHit, JPetHitExperimentalParametrizer& parametrizer)
{
  JPetMCRecoHit recoHit;
  recoHit.setPosX(mcHit.getScin().getCenterX());
  recoHit.setPosY(mcHit.getScin().getCenterY());
  recoHit.setPosZ(parametrizer.addZHitSmearing(mcHit.getScin().getID(), mcHit.getPosZ(), mcHit.getEnergy(), mcHit.getTime()));
  recoHit.setTime(parametrizer.addTimeSmearing(mcHit.getScin().getID(), mcHit.getPosZ(), mcHit.getEnergy(), mcHit.getTime()));
  recoHit.setEnergy(mcHit.getEnergy());
  return recoHit;
}

bool JPetGateParserTools::isHitReconstructed(JPetMCRecoHit& recoHit, const double energyThreshold) { return recoHit.getEnergy() >= energyThreshold; }

