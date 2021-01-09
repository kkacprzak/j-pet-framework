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
 *  @file JPetUnpackTask.h
 */

#ifndef JPETUNPACKTASK_H
#define JPETUNPACKTASK_H

#include "JPetTask/JPetTask.h"
#include <boost/any.hpp>
#include "Unpacker2.h"
#include <map>

class JPetUnpackTask: public JPetTask
{
public:
  using OptsStrAny = std::map<std::string, boost::any>;
  explicit JPetUnpackTask(const char* name = "");
  bool init(const JPetParams& inOptions) override;
  bool run(const JPetDataInterface& inData) override;
  bool terminate(JPetParams& outOptions) override;

  UInt_t* pHeader;
  UInt_t* pSubHeader;

protected:
  const std::string kTDCnonlinearityCalibKey = "Unpacker_TDCnonlinearityCalib_std::string";
  const std::string kTOTOffsetCalibKey = "Unpacker_TOToffsetCalib_std::string";
  std::string fTDCnonlinearityCalibFile = std::string("");
  std::string fTOTOffsetCalibFile = std::string("") = std::string("");
  OptsStrAny fOptions;




  std::string fOutputFilePath = std::string("");
  std::string fInputFilePath = std::string("");
  std::string fInputFile = std::string("");
};

#endif /* !JPETUNPACKTASK_H */
