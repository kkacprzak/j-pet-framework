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
 *  @file JPetGeanDecayTreeTest.cpp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetGeantParserDecayTreeTest
#include <boost/test/unit_test.hpp>

#include "JPetGeantDecayTree/JPetGeantDecayTree.h"
#include "JPetGeantDecayTreeBranch/JPetGeantDecayTreeBranch.h"

BOOST_AUTO_TEST_SUITE(FirstSuite)

BOOST_AUTO_TEST_CASE(branch_default_constructor)
{
  JPetGeantDecayTreeBranch branch;

  BOOST_REQUIRE_EQUAL(branch.GetTrackID(), -1);
  BOOST_REQUIRE_EQUAL(branch.GetPrimaryBranchID(), -1);
  BOOST_REQUIRE_EQUAL(branch.GetNumberOfNodes(), 0);
  BOOST_REQUIRE_EQUAL(branch.GetPrimaryNodeID(), -1);
  BOOST_REQUIRE_EQUAL(branch.GetLastNodeID(), -1);
  BOOST_REQUIRE_EQUAL(branch.GetPreviousNodeID(0), -1);
  BOOST_REQUIRE_EQUAL(branch.GetInteractionType(0), InteractionType::kUnknownInteractionType);
}

BOOST_AUTO_TEST_CASE(branch_setter_getter)
{
  int primaryBranchID = 1;
  int trackID = 2;

  JPetGeantDecayTreeBranch branch;
  branch.SetTrackID(trackID);
  branch.SetPrimaryBranchID(primaryBranchID);
  
  BOOST_REQUIRE_EQUAL(branch.GetTrackID(), trackID);
  BOOST_REQUIRE_EQUAL(branch.GetPrimaryBranchID(), primaryBranchID);
}

BOOST_AUTO_TEST_CASE(check_branch_setting_one_element)
{
  int primaryBranchID = 1;
  int trackID = 2;
  int nodeID = 3;
  InteractionType interactionType = InteractionType::kPrimaryGamma;
  
  JPetGeantDecayTreeBranch branch;
  branch.SetTrackID(trackID);
  branch.SetPrimaryBranchID(primaryBranchID);
  branch.AddNodeID(nodeID, interactionType);
  
  BOOST_REQUIRE_EQUAL(branch.GetPrimaryNodeID(), nodeID);
  BOOST_REQUIRE_EQUAL(branch.GetLastNodeID(), nodeID);
  BOOST_REQUIRE_EQUAL(branch.GetPreviousNodeID(nodeID), nodeID);
  BOOST_REQUIRE_EQUAL(branch.GetInteractionType(nodeID), interactionType);
}

BOOST_AUTO_TEST_CASE(check_branch_setting_more_elements)
{
  int primaryBranchID = 1;
  int trackID = 2;
  int firstNodeID = 3;
  int secondNodeID = 13;
  int thirdNodeID = 113;
  InteractionType firstInteractionType = InteractionType::kPrimaryGamma;
  InteractionType secondInteractionType = InteractionType::kScattNonActivePart;
  InteractionType thirdInteractionType = InteractionType::kScattActivePart;
  
  JPetGeantDecayTreeBranch branch;
  branch.SetTrackID(trackID);
  branch.SetPrimaryBranchID(primaryBranchID);
  branch.AddNodeID(firstNodeID, firstInteractionType);
  branch.AddNodeID(secondNodeID, secondInteractionType);
  branch.AddNodeID(thirdNodeID, thirdInteractionType);
  
  BOOST_REQUIRE_EQUAL(branch.GetPrimaryNodeID(), firstNodeID);
  BOOST_REQUIRE_EQUAL(branch.GetLastNodeID(), thirdNodeID);
  BOOST_REQUIRE_EQUAL(branch.GetPreviousNodeID(thirdNodeID), secondNodeID);
  BOOST_REQUIRE_EQUAL(branch.GetPreviousNodeID(secondNodeID), firstNodeID);
  BOOST_REQUIRE_EQUAL(branch.GetInteractionType(firstNodeID), firstInteractionType);
  BOOST_REQUIRE_EQUAL(branch.GetInteractionType(secondNodeID), secondInteractionType);
  BOOST_REQUIRE_EQUAL(branch.GetInteractionType(thirdNodeID), thirdInteractionType);
}



BOOST_AUTO_TEST_SUITE_END()
