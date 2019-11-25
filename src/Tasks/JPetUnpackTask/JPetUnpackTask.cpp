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
 *  @file JPetUnpackTask.cpp
 */

#include "./JPetUnpackTask/JPetUnpackTaskTools.h"
#include "./JPetOptionsTools/JPetOptionsTools.h"
#include "./JPetCommonTools/JPetCommonTools.h"
#include "./JPetUnpackTask/JPetUnpackTask.h"
#include "./JPetParams/JPetParams.h"
#include <fstream>

using namespace jpet_options_tools;
using namespace std;

JPetUnpackTask::JPetUnpackTask(const char* name): JPetUserTask(name) {}

bool JPetUnpackTask::init()
{
  INFO("UnpackTask started.");
  fOptions = inParams.getOptions();
  fOutputEvents = new JPetTimeWindow("JPetSigCh");

  // Reading parameters from the user json file
  if (isOptionSet(inParams.getOptions(), fTOTOffsetCalibFile)) {
    fTOTOffsetCalibFile = getOptionAsString(inParams.getOptions(), kTOTOffsetCalibKey);
  } else {
    ERROR(Form("No TOT offset calibration file found with the provided name: %s", fTOTOffsetCalibFile.c_str()));
  }
  if (isOptionSet(inParams.getOptions(), kTDCnonlinearityCalibKey)) {
    fTDCnonlinearityCalibFile = getOptionAsString(inParams.getOptions(), kTDCnonlinearityCalibKey);
  } else {
    ERROR(Form("No TDC nonlinearity file found with the provided name: %s", fTDCnonlinearityCalibFile.c_str()));
  }
  // If files were found and are good, the histograms will be read,
  // in the other case histograms will be filled with zeros.
  fTOTCalibHist = JPetUnpackTaskTools::getTOTCalibHist(fTOTOffsetCalibFile);
  fTDCCalibHist = JPetUnpackTaskTools::getTDCCalibHist(fTDCnonlinearityCalibFile);

  // Rewrite adresses and offsets from parambank to helper map
  // auto& tdcMap = getParamBank().getTDCs();
  // for(auto& tdc : tdcMap){
  //   fTDCOffsetsMap[tdc.getNetAddress()] = tdc.getOffset();
  // }
  // Arguments for unpack single step
  // const char* hldFile, const char* configFile, int numberOfEvents,
  //                                  int refChannelOffset, const char* TOTcalibFile,
  //                                  const char* TDCcalibFile
  // fOptions = inParams.getOptions();
  // TODO substitute conf_trb3.xml file with extended json
  // and read it to ParamBank

  return true;
}

bool JPetUnpackTask::exec()
{
  auto inputFile = getInputFile(fOptions);
  auto inputFileType = FileTypeChecker::getInputFileType(fOptions);

  if (inputFileType == FileTypeChecker::kHld) {
    fHLDFile = new std::ifstream(inputFile);
    if (fHLDFile->is_open()) {

      fAnalyzedEntry = 0;

      // skip the file header
      fHLDFile->ignore(32);

      auto allTimeSlots = JPetUnpackTaskTools::unpackAllTimeSlots();
      saveTimeWindow(allTimeSlots);




      bool doFirstLoop = true;
      while(doFirstLoop) {
        doFirstLoop = performOne();
      }
    } else {
      ERROR(Form("Error in opening the HLD file %s", inputFile.c_str()));
    }
    fHLDFile->close();
  } else {
    return false;
  }
  return true;
}

bool JPetUnpackTask::terminate(JPetParams& outParams)
{
  // if (fUnpackHappened) {
  //   OptsStrAny new_opts;
  //   jpet_options_generator_tools::setOutputFileType(new_opts, "hldRoot");
  //   if (jpet_options_tools::isOptionSet(fOptions, "firstEvent_int") &&
  //       jpet_options_tools::isOptionSet(fOptions, "lastEvent_int")) {
  //     if ( jpet_options_tools::getOptionAsInt(fOptions, "firstEvent_int") != -1 &&
  //          jpet_options_tools::getOptionAsInt(fOptions, "lastEvent_int") != -1 ) {
  //       jpet_options_generator_tools::setResetEventRangeOption(new_opts, true);
  //     }
  //   }
  //   jpet_options_generator_tools::setOutputFile(new_opts,
  //       JPetCommonTools::replaceDataTypeInFileName(getInputFile(fOptions), "hld")
  //                );
  //   outParams = JPetParams(new_opts, outParams.getParamManagerAsShared());
  // }
  return true;
}

void TimeWindowCreator::saveTimeWindow(const vector<JPetTimeWindow>& allTimeSlots)
{
  for (auto & timeSlot : allTimeSlots) {
    fOutputEvents->add<JPetTimeWindow>(timeSlot);
    // TODO add histograms
  }
}


bool JPetUnpackTask::performOne()
{
  if(fDebugMode) {
    INFO(Form("Position in file at %d", fHLDFile->tellg()));
  }

  // read out the header into HDR structure
  pHDR = (UInt_t*) &hdr;
  fHLDFile->read((char *) (pHDR), JPetUnpackTaskTools::getHDRSize());
  fEntrySize = (size_t) JPetUnpackTaskTools::getFullSize();
  fInitEntrySize = fEntrySize;

  if(fDebugMode) {
    INFO(Form("Current hdr size %d", fEntrySize));
  }

  if (fEntrySize == 32) {
    return true;
  }

  bool doSecondLoop = true;
  while(doSecondLoop) {
    doSecondLoop = performTwo();
  }

  fAnalyzedEntry++;

  if(fDebugMode) {
    INFO(Form("Ignoring padding of the event %d", (JPetUnpackTaskTools::align8(entrySize) - entrySize)));
    INFO(Form("File pointer at %d of %d bytes", fHLDFile->tellg(), fFileSize));
  }

  if (!fFullSetup) {
    fHLDFile->ignore(JPetUnpackTaskTools::align8(fEntrySize) - fEntrySize);
  }

  // check the end of loop conditions
  if((fFileSize - ((int) fHLDFile->tellg())) < 500) { return false; }
  if((fHLDFile->eof()) || ((int) fHLDFile->tellg() == fFileSize)) { return false; }
  if(fAnalyzedEntry == fEntriesToAnalyze) { return false; }
  return true;
}

bool JPetUnpackTask::performTwo()
{
  pSubHDR = (UInt_t*) &subHdr;
  fHLDFile->read((char *) pSubHDR, JPetUnpackTaskTools::getSubHDRSize());

  // read out the entire data of the event
  UInt_t* dataPtr = new UInt_t[JPetUnpackTaskTools::getDataSize(fDoReverseHex)];
  UInt_t* data = dataPtr;
  fHLDFile->read((char*) (dataPtr), JPetUnpackTaskTools::getDataSize(fDoReverseHex));

  if(fDebugMode) {
    INFO(Form("Sub data size: %d", JPetUnpackTaskTools::getDataSize(fDoReverseHex)));
    INFO(Form("Sub decoding %d hub %d trgNr %d", ((subHDR*) pSubHDR)->decoding), ((subHDR*) pSubHDR)->hubAddress, ((subHDR*) pSubHDR)->trgNr);
  }

  size_t dataSize = JPetUnpackTaskTools::getDataSize(fDoReverseHex);
  size_t initialDataSize = dataSize;

  if ((*data) != 0) {

    if(fDebugMode) {
      INFO(Form("Processing entry %d on %d", fAnalyzedEntry, JPetUnpackTaskTools::getHubAddress(fDoReverseHex)));
      INFO(Form("Receiving %d words to analyze", dataSize));
    }

    bool doThirdLoop = true;
    while(dataSize > 0) {
      doThirdLoop = performThree(data, dataSize, initialDataSize);
    }

  } else if((*data) == 0) {
    WARNING(Form("First data word empty, skipping entry nr %d", fAnalyzedEntry));
  }

  if(fDebugMode) {
    INFO(Form(
      "Ignoring %d bytes and reducing entry size by %d",
      (JPetUnpackTaskTools::getPaddedDataSize(fDoReverseHex)
        - JPetUnpackTaskTools::getDataSize(fDoReverseHex)),
        JPetUnpackTaskTools::getDataSize(fDoReverseHex))
    );
  }

  delete[] dataPtr;

  // remove the padding bytes
  fHLDFile->ignore(
    JPetUnpackTaskTools::getPaddedDataSize(fDoReverseHex) - JPetUnpackTaskTools::getDataSize(fDoReverseHex)
  );
  fEntrySize -= JPetUnpackTaskTools::getDataSize(fDoReverseHex);

  if(fDebugMode) {
    INFO(Form("Leaving entry size of %d", dataSize));
  }

  if(fEntrySize > fInitEntrySize) {
    ERROR(Form("Incorrect entry size, the input file is likely corrupted"));
    ERROR(Form("Stopping sub loop after %d entries read", dataSize));
    return false;
  }

  if(fEntrySize <= 48 && !fFullSetup) { return false; }

  fEntrySize -= JPetUnpackTaskTools::getPaddedDataSize(fDoReverseHex) - JPetUnpackTaskTools::getDataSize(fDoReverseHex);

  if((fEntrySize <= 64) && fFullSetup) { return false; }
  if((fEntrySize <= 176) && fFullSetup) { return false; }

  return true;
}

bool JPetUnpackTask::performThree(UInt_t* data, size_t dataSize, size_t initialDataSize)

  if(dataSize > initialDataSize) {
    ERROR("Incorrect data size encountered, the input file is likely corrupted, stopping the sub loop");
    return false;
  }

  UInt_t data_i = JPetUnpackTaskTools::reverseHex(*data);
  size_t internalSize = data_i >> 16;
  UInt_t tdcAddress = data_i & 0xffff;

  if(fDebugMode) {
    INFO(Form("Current tdc adress %d", tdcAddress));
  }

  int channelOffset = -1;
  auto search = fTDCOffsetsMap.find(tdcAddress);
  if (search != fTDCOffsetsMap.end()) {
    channelOffset = search.second;

    size_t is = internalSize + 1;

    while(is > 0) {
      performFour();
    }


  } else {
    if(fDebugMode){
      ERROR(Form("TDC with net addres %d not found, skipping %d bytes", tdcAddress, internalSize));
      data += internalSize + 1;
    }
  }

  dataSize -= (internalSize + 1) * 4;

  return true;
}

bool JPetUnpackTask::performFour(UInt_t* data)
{
  bool gotRef = false;
  bool isRising = false;
  bool firstHitOnCh = true;
  UInt_t data_i = JPetUnpackTaskTools::reverseHex(*data);

  int header = (data_i >> 29);

  int epoch;
  int channel;
  int coarse;
  int fine;

  double fullTime;
  double refTime;

  // TDCChannel* new_ch;

  switch (header) {
    // epoch time
    case 3:
      epoch = data_i & 0xfffffff;
      break;

    // time data
    case 4:
      if (channel >= 0 && (unsigned(channel) != ((data_i >> 22) & 0x7f))) {
        firstHitOnCh = true;
      }
      channel = (data_i >> 22) & 0x7f;
      coarse = (data_i & 0x7ff);
      fine = ((data_i >> 12) & 0x3ff);
      isRising = ((data_i >> 11) & 0x1);


      if (useTDCcorrection == true && TDCcorrections[channel + channelOffset] != nullptr) {
        fine = TDCcorrections[channel + channelOffset]->GetBinContent(fine + 1);
      } else {
        fine = fine * 10;
      }

      if (fine != 0x3ff) {
        fullTime = (double) ( ((epoch << 11) * 5.0) );
        fullTime += (((coarse * 5000.) - fine) / 1000.);

        if (channel == 0) {
          refTime = fullTime;
          gotRef = true;
        } else {
          if (gotRef == true) {
            if (firstHitOnCh == true) {
              new_ch = eventIII->AddTDCChannel(channel + channelOffset);
            }
            fullTime = fullTime - refTime;
            if (isRising == false) {
              fullTime -= TOTcalibHist->GetBinContent(channel + channelOffset + 1);
              new_ch->AddTrail(fullTime);
            } else {
              new_ch->AddLead(fullTime);
            }
            firstHitOnCh = false;
          }
        }
      }
      break;
    default:
      break;
  }

  data++;
  is--;

  return true;
}









// void JPetUnpackTask::unpackFile(
//   const std::string& filename,
//   long long nevents, const std::string& configfile = "",
//   const std::string& calibfile = "")
// {
//   JPetUnpackTask unpacker;
//   if (nevents > 0) {
//     unpacker.setParams(filename, nevents, configfile, totCalibFile, tdcCalibFile);
//     WARNING(std::string("Even though the range of events was set, only the first ")
//             + JPetCommonTools::intToString(nevents)
//             + std::string(" will be unpacked by the unpacker. \n The unpacker always starts from the beginning of the file.")
//            );
//   } else {
//     unpacker.setParams(filename, 100000000, configfile, totCalibFile, tdcCalibFile);
//   }
//   return unpacker.exec();
// }
