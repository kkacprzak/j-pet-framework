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
 *  @file JPetUnpackTaskTools.cpp
 */

#include "JPetUnpackTask/JPetUnpackTaskTools.h"

/**
 * Reading TOT calibration histogram from external ROOT file. If not found,
 * retuning a histogram filled with zeros. Number of bins for creating an empty
 * histogram is a highest ID number of Channels in configuration
 */
TH1F* JPetUnpackTaskTools::loadTOTCalibHistogram(std::string totCalibFileName, int channelNum)
{
  TH1F* totCalibHisto;
  if (totCalibFileName.find(".root") == string::npos) {
    WARNING(Form("TOT calibration file %s not found", totCalibFileName.c_str()));
    returnHisto = new TH1F("stretcher_offsets", "stretcher_offsets", channelNum, 0, channelNum);
    for (int i = 0; i < channelNum; i++) {
      returnHisto->SetBinContent(i + 1, 0.0);
    }
  } else {
    INFO(Form("Loading TOT calibration from file %s", totCalibFileName.c_str()));
    TFile* totCalibFile = new TFile(totCalibFileName.c_str(), "READ");
    TDirectory* dir = gDirectory->GetDirectory("Rint:/");
    TH1F* tmpHisto = dynamic_cast<TH1F*>(totCalibFile->Get("stretcher_offsets"));
    totCalibHisto = dynamic_cast<TH1F*>(tmpHisto->Clone("stretcher_offsets"));
    totCalibHisto->SetDirectory(dir);
    totCalibFile->Close();
  }
  return totCalibHisto;
}

/**
 * Reading TDC calibration of channel nonlinearity from external ROOT file.
 * For a given number of channels, a calibration histogram is read and put
 * into map object. If a file was not found, retuning an empty map.
 */
std::map<int, TH1F*> JPetUnpackTaskTools::loadTDCCalibHistos(std::string tdcCalibFileName, int channelNum)
{
  std::map<int, TH1F*> tdcHistograms;
  TFile* tdcCalibFile = new TFile(tdcCalibFileName, "READ");
  TDirectory* dir = gDirectory->GetDirectory("Rint:/");

  if (tdcCalibFile->IsOpen()) {
    INFO(Form("Loading TDC nonlinearity corrections from file %s", tdcCalibFileName.c_str()));
    for (int i = 0; i < channelNum; ++i) {
      TH1F* tmp = dynamic_cast<TH1F*>(tdcCalibFile->Get(Form("correction%d", i)));
      if (tmp) {
        tdcHistograms[i] = dynamic_cast<TH1F*>(tmp->Clone(tmp->GetName()));
        tdcHistograms[i]->SetDirectory(dir);
      } else {
        tdcHistograms[i] = nullptr;
      }
    }
  } else {
    WARNING(Form("TDC nonlinearity calibration file %s not found, empty map will be used.", tdcCalibFileName.c_str()));
  }

  f->Close();
  delete f;
  return tdcHistograms;
}

/**
 * Method checking ordering of bytes in binary file, indicating if it is necessary
 * to invert bytes in next steps.
 */
bool JPetUnpackTaskTools::areBytesToBeInverted(string fileName)
{
  ifstream* file = new ifstream(fileName.c_str());
  if (file->is_open()) {
    // skip the file header
    file->ignore(32);
    // read out data into Header and SubHeader structure
    (UInt_t*) pHdr = (UInt_t*)&header;
    file->read((char*)(pHdr), sizeof(Header));
    (UInt_t*) pSub = (UInt_t*)&subheader;
    file->read((char*)(pSub), sizeof(Subheader));
    if (((Subheader*)pSub)->decoding == 16777728) {
      return true;
    }
  }
  file->close();
  return false;
}

/**
 * Tool for reversing hexadecimal number
 */
size_t JPetUnpackTaskTools::reverseHex(size_t n) const
{
  size_t a, b, c, d;

  a = n & 0x000000ff;
  b = n & 0x0000ff00;
  c = n & 0x00ff0000;
  d = n & 0xff000000;

  a <<= 24;
  b <<= 8;
  c >>= 8;
  d >>= 24;

  return a | b | c | d;
}

/**
 * Getting a size of subheader data for cases if bytes are to be inverted, or not
 */
size_t JPetUnpackTaskTools::getDataSize(UInt_t* pSubHeader, bool invertBytes) const
{
  if (invertBytes) {
    return (size_t)(ReverseHex(((Subheader*)pSubHeader)->size) - 16);
  } else {
    return (size_t)(((Subheader*)pSubHeader)->size - 16);
  }
  return -1;
}

/**
 * Probably not needed - check later
 */
std::string JPetUnpackTaskTools::getHubAddress(UInt_t* pSubHeader, bool invertBytes) const
{
  std::string s = "0000";
  std::stringstream sstream;
  if (invertBytes) {
    sstream << hex << ReverseHex((UInt_t)((Subheader*)pSubHeader)->hubAddress);
  } else {
    sstream << hex << ((Subheader*)pSubHeader)->hubAddress;
  }
  return s.replace(4 - sstream.str().length(), sstream.str().length(), sstream.str());
}
