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

/* JPetGateParserTools::mapScintillatorFromGate function converts the simulated with GATE ID of scintillators
to the real mapping of scintilaltors. It takes as arguments IDs of module and scintillator in which the simulated
interaction was registered. Currently, the function has hardcoded conversion for Modular J-PET hospital version. */

int JPetGateParserTools::mapScintillatorFromGate(int rSectorID, int crystalID)
{
  if (rSectorID < 6)
  {
    return 266 + crystalID - 13 * rSectorID;
  }
  return 266 + crystalID - 13 * rSectorID + 312;
}

/* JPetGateParserTools::checkIfInCurrentTimeWindow function checks if currently investigated interaction should fall
into the current electronics time window (dicated by the fClockWindowTime value). It takes as arguments simulated
time of interaction, number of current electronics time window and length of the electronics time window. */

bool JPetGateParserTools::checkIfInCurrentTimeWindow(double fTime_ps, unsigned long long int fWindowNumber, double fClockWindowTime)
{
  double timeBasedOnPreviousTimeWindow = fTime_ps - (double)fWindowNumber * fClockWindowTime;
  if (timeBasedOnPreviousTimeWindow < fClockWindowTime)
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

void JPetGateParserTools::setSeedTogRandom(unsigned long seed)
{
  if (!gRandom)
  {
    ERROR("gRandom is not set and we cannot set the seed");
  }
  else
  {
    gRandom->SetSeed(seed);
  }
}
