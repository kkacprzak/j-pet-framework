/**
 *  @copyright Copyright 2021 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetBaseMCHitTest.cpp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetBaseMCHitTest

#include "JPetBaseMCHit/JPetBaseMCHit.h"
#include "JPetScin/JPetScin.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(FirstSuite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  JPetBaseMCHit mcHit;
  BOOST_REQUIRE_EQUAL(mcHit.getMCDecayTreeIndex(), 0);
  BOOST_REQUIRE_EQUAL(mcHit.getMCVtxIndex(), 0);
  BOOST_REQUIRE_EQUAL(mcHit.getMultiplicityTag(), JPetBaseMCHit::kUnknown);

  mcHit.setMCDecayTreeIndex(11);
  mcHit.setMCVtxIndex(22);
  mcHit.setPolarization(1.1, 2.2, 3.3);
  mcHit.setMomentum(4.4, 5.5, 6.6);
  mcHit.setMultiplicityTag(JPetBaseMCHit::k3gOPS);

  double epsilon = 0.0001;
  BOOST_REQUIRE_EQUAL(mcHit.getMCDecayTreeIndex(), 11);
  BOOST_REQUIRE_EQUAL(mcHit.getMCVtxIndex(), 22);
  BOOST_REQUIRE_CLOSE(mcHit.getPolarization().X(), 1.1, epsilon);
  BOOST_REQUIRE_CLOSE(mcHit.getPolarization().Y(), 2.2, epsilon);
  BOOST_REQUIRE_CLOSE(mcHit.getPolarization().Z(), 3.3, epsilon);
  BOOST_REQUIRE_CLOSE(mcHit.getMomentum().X(), 4.4, epsilon);
  BOOST_REQUIRE_CLOSE(mcHit.getMomentum().Y(), 5.5, epsilon);
  BOOST_REQUIRE_CLOSE(mcHit.getMomentum().Z(), 6.6, epsilon);
  BOOST_REQUIRE_EQUAL(mcHit.getMultiplicityTag(), JPetBaseMCHit::k3gOPS);
}

BOOST_AUTO_TEST_SUITE_END()
