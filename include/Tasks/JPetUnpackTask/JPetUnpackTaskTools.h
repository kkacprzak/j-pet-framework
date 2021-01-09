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
 *  @file JPetUnpackTaskTools.h
 */

#ifndef JPETUNPACKTASKTOOLS_H
#define JPETUNPACKTASKTOOLS_H

// #include <map>

class JPetUnpackTaskTools
{
public:
  struct Header
  {
    UInt_t fullSize;
    UInt_t decoding;
    UInt_t id;
    UInt_t seqNr;
    UInt_t date;
    UInt_t time;
    UInt_t runNr;
    UInt_t pad;
  } header;

  struct Subheader
  {
    UInt_t size;
    UInt_t decoding;
    UInt_t hubAddress;
    UInt_t trgNr;
  } subheader;

  TH1F* loadTOTCalibHistogram(std::string totCalibFileName);
  std::map<int, TH1F*> loadTDCCalibHistos(std::string tdcCalibFileName, int channelNum);
  bool areBytesToBeInverted(string fileName);
  size_t reverseHex(size_t n) const;
  size_t getDataSize(UInt_t* pSubHeader, bool invertBytes) const;
  std::string getHubAddress(UInt_t* pSubHeader, bool invertBytes) const;


};

#endif /* !JPETUNPACKTASKTOOLS_H */
