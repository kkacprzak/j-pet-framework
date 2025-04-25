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
 *  @file JPetManagerTest.cpp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetManagerTest

#include "./JPetManager/JPetManager.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(FirstSuite)

BOOST_AUTO_TEST_CASE(create_unique_manager)
{
  JPetManager& manager = JPetManager::getManager();
  JPetManager* pManager = &manager;
  JPetManager& manager2 = JPetManager::getManager();
  JPetManager* pManager2 = &manager2;
  BOOST_REQUIRE_EQUAL(pManager, pManager2);
}

BOOST_AUTO_TEST_CASE(threadsEnabled)
{
  JPetManager& manager = JPetManager::getManager();
  BOOST_REQUIRE(!manager.areThreadsEnabled());
  manager.setThreadsEnabled(true);
  BOOST_REQUIRE(manager.areThreadsEnabled());
  manager.setThreadsEnabled(false);
  BOOST_REQUIRE(!manager.areThreadsEnabled());
}

BOOST_AUTO_TEST_CASE(emptyRun)
{
  JPetManager& manager = JPetManager::getManager();
  BOOST_CHECK_THROW(manager.run(0, nullptr), std::exception);
}

BOOST_AUTO_TEST_CASE(goodRootRun)
{
  // goodRootFile.root needs to be updated
  JPetManager& manager = JPetManager::getManager();
  const char* args[7] = {"test/Path", "--file", "unitTestData/JPetManagerTest/goodRootFile.hits.root", "--type",
                         "root",      "-u",     "unitTestData/JPetManagerTest/userParamsDummy.json"};
  BOOST_REQUIRE_NO_THROW(manager.run(7, args));
}

BOOST_AUTO_TEST_CASE(goodZipRun)
{
  std::remove("unitTestData/JPetManagerTest/xx14099113231.hld");
  JPetManager& manager = JPetManager::getManager();
  const char* args[14] = {"test/Path",
                          "--file",
                          "unitTestData/JPetManagerTest/xx14099113231.hld.xz",
                          "--type",
                          "zip",
                          "-p",
                          "unitTestData/JPetManagerTest/conf_trb3.xml",
                          "-r",
                          "0",
                          "10",
                          "-l",
                          "unitTestData/JPetManagerTest/modular_setup_clinical_fixed_ds.json",
                          "-i",
                          "38"};
  BOOST_REQUIRE_NO_THROW(manager.run(14, args));
}

BOOST_AUTO_TEST_CASE(goodMCRun)
{
  JPetManager& manager = JPetManager::getManager();
  const char* args[16] = {"test/Path",
                          "--file",
                          "unitTestData/JPetManagerTest/goodMCFile2.mcGeant.root",
                          "--type",
                          "mcGeant",
                          "-k",
                          "mod",
                          "-u",
                          "unitTestData/JPetManagerTest/userParamsDummy.json",
                          "-l",
                          "unitTestData/JPetManagerTest/modular_setup_clinical_fixed_ds.json",
                          "-i",
                          "38",
                          "-r",
                          "0",
                          "100"};
  BOOST_REQUIRE_NO_THROW(manager.run(16, args));
}

// BOOST_AUTO_TEST_CASE(goodControlTasks)
// {
//   JPetManager& manager = JPetManager::getManager();
//   std::vector<const char*> args = {
//       "test/Path", "--file", "unitTestData/JPetManagerTest/goodRootFile.hits.root",          "--type",
//       "root",      "-u",     "unitTestData/JPetManagerTest/userParamsControlTasksGood.json",
//   };
//   manager.registerTask<TestTask>("TestTask");
//   BOOST_REQUIRE_NO_THROW(manager.run(args.size(), args.data()));
// }

BOOST_AUTO_TEST_CASE(notRegisteredTask)
{
  JPetManager& manager = JPetManager::getManager();
  std::vector<const char*> args = {
      "test/Path", "--file", "unitTestData/JPetManagerTest/goodRootFile.hits.root",          "--type",
      "root",      "-u",     "unitTestData/JPetManagerTest/userParamsUnregisteredTask.json",
  };
  BOOST_CHECK_THROW(manager.run(args.size(), args.data()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(badOptionsTask)
{
  JPetManager& manager = JPetManager::getManager();
  std::vector<const char*> args = {
      "test/Path", "--file", "unitTestData/JPetManagerTest/goodRootFile.hits.root",    "--type",
      "root",      "-u",     "unitTestData/JPetManagerTest/userParamsBadOptions.json",
  };
  BOOST_CHECK_THROW(manager.run(args.size(), args.data()), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
