//Copyright (c) 2014 - 2017, The Trustees of Indiana University.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.


#ifndef PROT_FEATURE_MATCH_ENVELOPE_HPP_
#define PROT_FEATURE_MATCH_ENVELOPE_HPP_

#include <memory>
#include <vector>

#include "spec/ms_header.hpp"
#include "feature/feature_mng.hpp" 
#include "feature/envelope.hpp" 
#include "feature/real_env.hpp" 

namespace prot {

class MatchEnv;

typedef std::shared_ptr<MatchEnv> MatchEnvPtr;

class MatchEnv {
 public:
  MatchEnv(int mass_group, EnvelopePtr theo_env_ptr, RealEnvPtr real_env_ptr);

  void compScr(FeatureMngPtr mng_ptr);

  static bool cmpScoreDec(const MatchEnvPtr &a, const MatchEnvPtr &b) { 
    return a->getScore() > b->getScore();
  }

  double calcPeakScr(int id_x, double inte_sum, double tolerance);

  int getId() {return id_;}

  int getMassGroup() {return mass_group_;}

  RealEnvPtr getRealEnvPtr() {return real_env_ptr_;}

  EnvelopePtr getTheoEnvPtr() {return theo_env_ptr_;}

  double getScore() {return score_;}

  void setScore(double score) {score_ = score;}

  void setId(int id) {id_ = id;}

  void setTheoEnvPtr(EnvelopePtr theo_env_ptr) {theo_env_ptr_ = theo_env_ptr;}

  void appendXml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent);

  static std::string getXmlElementName() {return "match_env";}

 private:
  int id_;
  // we divide envelopes into several groups based on monoisotopic  masses  
  int mass_group_;
  double score_;
  EnvelopePtr theo_env_ptr_;
  RealEnvPtr real_env_ptr_;

  double calcShareInteAccu(int id_x, double inte_sum);

  double calcMzFactor(int id_x, double shift, double tolerance);

  double calcIntensityFactor(double theo_inte, double real_inte);

  double calcIntensityFactor(int id_x, double ratio);

  double findBestShift(FeatureMngPtr mng_ptr);

  double findBestRatio(FeatureMngPtr mng_ptr);

  double calcScrWithSftRatio(double shift, double ratio, double tolerance);
};

typedef std::vector<MatchEnvPtr> MatchEnvPtrVec;
typedef std::vector<MatchEnvPtrVec> MatchEnvPtr2D;

/*
namespace MsalignWriter {

void write(std::ofstream &file, MatchEnvPtrVec &envs, MsHeaderPtr header_ptr);

}  // namespace MsalignWriter
*/

}  // namespace prot

#endif
