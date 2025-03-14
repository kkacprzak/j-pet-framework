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
 *  @file JPetGateParserToolsTest.cpp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetGateParserToolsTest
#include <boost/test/unit_test.hpp>

#include "JPetGateParser/JPetGateParserTools.h"

BOOST_AUTO_TEST_SUITE(JPetGateParserToolsTestSuite)

BOOST_AUTO_TEST_CASE(testGateFrameworkMapping)
{
  int id1 = JPetGateParserTools::mapScintillatorFromGate(0,0);
  int id2 = JPetGateParserTools::mapScintillatorFromGate(5,12);
  int id3 = JPetGateParserTools::mapScintillatorFromGate(6,1);
  int id4 = JPetGateParserTools::mapScintillatorFromGate(7,0);
  BOOST_REQUIRE_EQUAL(id1,266);
  BOOST_REQUIRE_EQUAL(id2,213);
  BOOST_REQUIRE_EQUAL(id3,501);
  BOOST_REQUIRE_EQUAL(id4,487);
}

BOOST_AUTO_TEST_SUITE_END()
