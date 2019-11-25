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
 *  @file JPetUnpackTaskTools.h
 */

 #ifndef JPETUNPACKTASKTOOLS_H
 #define JPETUNPACKTASKTOOLS_H

#include <TH1F.h>



class JPetUnpackTaskTools
{
public:
  static TH1F* getTOTCalibHist(std::string calibFileName);
  static TH1F* getTDCCalibHists(std::string calibFileName);

  struct HDR {
    UInt_t fullSize;
    UInt_t decoding;
    UInt_t id;
    UInt_t seqNr;
    UInt_t date;
    UInt_t time;
    UInt_t runNr;
    UInt_t pad;
  } hdr;

  struct subHDR {
    UInt_t size;
    UInt_t decoding;
    UInt_t hubAddress;
    UInt_t trgNr;
  } subHdr;

  static size_t getDataSize(bool doReverseHex);
  static std::string getHubAddress(bool doReverseHex);

  size_t reverseHex(size_t n);

  size_t getHDRSize() const { return sizeof(HDR); }
  size_t getSubHDRSize() const { return sizeof(subHDR); }
  UInt_t getFullSize() const { return ((HDR*) hdr)->fullSize; }
  size_t align8(const size_t i) const { return 8 * size_t((i - 1) / 8 + 1); }
  size_t getPaddedDataSize(bool doReverseHex) { return align8(getDataSize(doReverseHex)); }
  size_t getPaddedFullSize() { return align8(getFullSize()); }
  size_t getDataLength() const { return ((getFullSize() - getHDRSize()) + 3) / 4; }


  std::map<UInt_t, UInt_t> tdc_offsets;
  int eventsToAnalyze;
  bool useTDCcorrection = false;
  bool areBytesToBeInverted(std::string);
  bool invertBytes;
  bool fullSetup;
  bool debugMode;
  long int fileSize;
  int refChannelOffset;
  TH1F * TOTcalibHist = nullptr;
  int highest_channel_number = -1;
  TH1F ** TDCcorrections = nullptr;

  const static int kMaxAllowedRepetitions = 1;

private:
  static const int kRefChannelsNubmer = 50;
  static const int kRefChannelOffset = 65;
};

 #endif /* !JPETUNPACKTASKTOOLS_H */
