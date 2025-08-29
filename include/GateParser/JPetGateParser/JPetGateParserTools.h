/**
 *  @copyright Copyright 2021 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetGateParserTools.h
 */

#ifndef JPETGEANTPARSERTOOLS_H
#define JPETGEANTPARSERTOOLS_H

#include "Hits/JPetMCRecoHit/JPetMCRecoHit.h"
#include "JPetParamBank/JPetParamBank.h"
#include "JPetRawMCHit/JPetRawMCHit.h"
#include "JPetSmearingFunctions/JPetSmearingFunctions.h"
#include <JPetTaskIO/JPetInputHandlerGATE.h>
#include <TRandom3.h>
#include <array>
#include <functional>
#include <map>
#include <tuple>
#include <vector>

class JPetGateParserTools
{
public:
  static int mapScintillatorFromGate(int rSectorID, int crystalID);
  
  static bool checkIfInCurrentTimeWindow(double fTime_ps, unsigned long long int fWindowNumber, double fClockWindowTime);

  static JPetMCRecoHit reconstructHit(JPetRawMCHit& hit, JPetHitExperimentalParametrizer& parametrizer);

  static bool isHitReconstructed(JPetMCRecoHit& hit, const double th);

  static void setSeedTogRandom(unsigned long seed);
};

#endif /* !JPETGEANTPARSERTOOLS_H */
