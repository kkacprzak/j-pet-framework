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
 *  @file JPetUnpackTaskTools.cpp
 */

 #include "./JPetUnpackTask/JPetUnpackTaskTools.h"
 #include "./Core/JPetLoggerInclude.h"
 #include <TFile.h>

using namespace std;

TH1F* JPetUnpackTaskTools::getTOTCalibHist(string calibFileName)
{
  TH1F* calibHisto;
  TFile* calibFile = new TFile(calibFileName.c_str(), "READ");
  if(calibFile->IsOpen()){
    auto directory = gDirectory->GetDirectory("Rint:/");
    calibHisto = dynamic_cast<TH1F*>(calibFile->Get("stretcher_offsets")->Clone("stretcher_offsets"));
    calibHisto->SetDirectory(directory);
    calibFile->Close();
  } else {
    ERROR(Form("Unable to open TOT calibration file %s, returning histogram filled with zeros.", calibFileName.c_str()));
    calibHisto = new TH1F("stretcher_offsets", "stretcher_offsets",
      kRefChannelsNubmer*kRefChannelOffset, 0, kRefChannelsNubmer*kRefChannelOffset
    );
    for (int i = 0; i < kRefChannelsNubmer * kRefChannelOffset; i++) {
      calibHisto->SetBinContent(i + 1, 0);
    }
  }
  return calibHisto;
}

TH1F* JPetUnpackTaskTools::getTDCCalibHists(string calibFileName)
{
  TFile * f = new TFile(calibFile, "READ");
    TDirectory* dir = gDirectory->GetDirectory("Rint:/");

    if(f->IsOpen()){

      TDCcorrections = new TH1F*[highest_channel_number];
      for(int i=0;i<highest_channel_number; ++i){

        TH1F * tmp = dynamic_cast<TH1F*>(f->Get(Form("correction%d", i)));

        if(tmp){
          TDCcorrections[i] = dynamic_cast<TH1F*>(tmp->Clone(tmp->GetName()));
          TDCcorrections[i]->SetDirectory(dir);
        }else{
          TDCcorrections[i] = nullptr;
        }

      }

      if(debugMode){
        cerr << "Loaded TDC nonlinearity corrections." << endl;
      }

    }else{
      if(debugMode){
        cerr << "The TDC calibration file " << calibFile << " could not be properly opened." << endl;
        cerr << "TDC nonlinearity correction will not be used!" << endl;
      }

      f->Close();
      delete f;

      return false;
    }

    f->Close();
    delete f;

    return true;s







  TH1F* calibHisto;
  auto calibFile = new TFile(calibFileName, "READ");
  auto directory = gDirectory->GetDirectory("Rint:/");

  if(calibFile->IsOpen()){

    calibHisto = new TH1F*[highest_channel_number];
    for(int i=0;i<highest_channel_number; ++i){

      TH1F * tmp = dynamic_cast<TH1F*>(f->Get(Form("correction%d", i)));

      if(tmp){
        TDCcorrections[i] = dynamic_cast<TH1F*>(tmp->Clone(tmp->GetName()));
        TDCcorrections[i]->SetDirectory(dir);
      }

    }
  //
  //   if(debugMode){
  //     cerr << "Loaded TDC nonlinearity corrections." << endl;
  //   }
  //
  //
  // } else {
  //   ERROR("Unable to open TDC calibration file %s, returning histogram filled with zeros.", calibFileName);
  //
  //   calibHisto = new TH1F("stretcher_offsets", "stretcher_offsets",
  //     kRefChannelsNubmer*kRefChannelOffset, 0, kRefChannelsNubmer*kRefChannelOffset
  //   );
  //   for (int i = 0; i < kRefChannelsNubmer * kRefChannelOffset; i++) {
  //     calibHisto->SetBinContent(i + 1, 0);
  //   }
  // }
  //
  //
  // TFile * f = new TFile(calibFile, "READ");
  // TDirectory* dir = gDirectory->GetDirectory("Rint:/");
  //
  //   if(f->IsOpen()){
  //
  //     TDCcorrections = new TH1F*[highest_channel_number];
  //     for(int i=0;i<highest_channel_number; ++i){
  //
  //       TH1F * tmp = dynamic_cast<TH1F*>(f->Get(Form("correction%d", i)));
  //
  //       if(tmp){
  //         TDCcorrections[i] = dynamic_cast<TH1F*>(tmp->Clone(tmp->GetName()));
  //         TDCcorrections[i]->SetDirectory(dir);
  //       }
  //
  //     }
  //
  //     if(debugMode){
  //       cerr << "Loaded TDC nonlinearity corrections." << endl;
  //     }
  //
  //   }else{
  //     if(debugMode){
  //       cerr << "The TDC calibration file " << calibFile << " could not be properly opened." << endl;
  //       cerr << "TDC nonlinearity correction will not be used!" << endl;
  //     }
  //
  //     f->Close();
  //     delete f;
  //
  //     return false;
  //   }
  //
  //   f->Close();
  //   delete f;
  //
  //   return true;
  return calibHisto;
}

size_t JPetUnpackTaskTools::reverseHex(UInt_t n) {

  int a, b, c, d;

  a = n & 0x000000ff;
  b = n & 0x0000ff00;
  c = n & 0x00ff0000;
  d = n & 0xff000000;

  a <<= 24;
  b <<= 8;
  c >>= 8;
  d >>= 24 ;

  return a|b|c|d;
}


size_t JPetUnpackTaskTools::getDataSize(bool doReverseHex) {
  if(doReverseHex){
    return (size_t) (reverseHex(((subHDR*) subHdr)->size) - 16);
  } else {
    return (size_t) ((((subHDR*) subHdr))->size - 16);
  }
  return -1;
}

std::string JPetUnpackTaskTools::getHubAddress(bool doReverseHex) {
  std::string address = "0000";
  stringstream sstream;
  if (doReverseHex) {
    sstream << hex << reverseHex((UInt_t) ((subHDR*) subHdr)->hubAddress);
  } else {
    sstream << hex << ((subHDR*) subHdr)->hubAddress;
  }
  return address.replace(4 - sstream.str().length(), sstream.str().length(), sstream.str());
}
