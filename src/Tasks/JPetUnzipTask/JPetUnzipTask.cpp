/**
 *  @copyright Copyright 2019 The J-PET Framework Authors. All rights reserved.
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
 *  @file JPetUnzipTask.cpp
 */

#include "./JPetOptionsGenerator/JPetOptionsGeneratorTools.h"
#include "./JPetOptionsTools/JPetOptionsTools.h"
#include "./JPetCommonTools/JPetCommonTools.h"
#include "./JPetParams/JPetParams.h"
#include "./JPetUnzipTask.h"

using namespace jpet_options_tools;
using namespace std;

JPetUnzipTask::JPetUnzipTask(const char* name): JPetTask(name) {}

bool JPetUnzipTask::init(const JPetParams& inParams)
{
  INFO("UnzipTask started.");
  fOptions = inParams.getOptions();
  return true;
}

bool JPetUnzipTask::run(const JPetDataInterface&)
{
  auto inputFile = getInputFile(fOptions);
  auto outputPath = getOutputPath(fOptions);
  INFO(Form("Unzipping file: %s into %s", inputFile.c_str(), outputPath.c_str()));
  if (!unzipFile(inputFile, outputPath)) {
    ERROR(Form("Problem with unzipping file: %s", inputFile.c_str()));
    return false;
  }
  return true;
}

bool JPetUnzipTask::terminate(JPetParams& outParams)
{
  OptsStrAny new_opts;
  jpet_options_generator_tools::setOutputFileType(new_opts, "hld");
  jpet_options_generator_tools::setOutputFile(
    new_opts,
    JPetCommonTools::stripFileNameSuffix(getInputFile(fOptions))+"hld"
  );
  if (jpet_options_tools::isOptionSet(fOptions, "firstEvent_int")
  && jpet_options_tools::isOptionSet(fOptions, "lastEvent_int")) {
    if (jpet_options_tools::getOptionAsInt(fOptions, "firstEvent_int") != -1
    && jpet_options_tools::getOptionAsInt(fOptions, "lastEvent_int") != -1) {
      jpet_options_generator_tools::setResetEventRangeOption(new_opts, true);
    }
  }
  outParams = JPetParams(new_opts, outParams.getParamManagerAsShared());
  INFO(Form(
    "UnzipTask finished, unzipped file name: %s",
    (JPetCommonTools::stripFileNameSuffix(getInputFile(fOptions))+"hld").c_str()
  ));
  return true;
}

bool JPetUnzipTask::unzipFile(string filename, string outputPath)
{
  int unzip = 1;
  int move = 1;

  if (JPetCommonTools::exctractFileNameSuffix(filename) == ".gz") {
    unzip = system((string("gzip -dk ") + filename).c_str());
    move = system((string("mv ")+filename.substr(0, filename.size()-3)+string(" ")+outputPath).c_str());
  } else if (JPetCommonTools::exctractFileNameSuffix(filename) == ".xz") {
    unzip = system((string("xz -dk ") + string(filename)).c_str());
    move = system((string("mv ")+filename.substr(0, filename.size()-3)+string(" ")+outputPath).c_str());
  } else if (JPetCommonTools::exctractFileNameSuffix(filename) == ".bz2") {
    unzip = system((string("bzip2 -dk ") + string(filename)).c_str());
    move = system((string("mv ")+filename.substr(0, filename.size()-4)+string(" ")+outputPath).c_str());
  } else if (JPetCommonTools::exctractFileNameSuffix(filename) == ".zip") {
    unzip = system((string("unzip ") + string(filename)).c_str());
    move = system((string("mv ")+filename.substr(0, filename.size()-4)+string(" ")+outputPath).c_str());
  }

  return !(unzip+move);
}
