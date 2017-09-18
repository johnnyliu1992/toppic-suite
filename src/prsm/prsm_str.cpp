// Copyright (c) 2014 - 2017, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <limits>
#include <string>
#include <vector>
#include <algorithm>

#include "base/logger.hpp"
#include "prsm/prsm_util.hpp"
#include "prsm/prsm_str.hpp"

namespace prot {

bool ChangeStr::cmpPosInc(const std::shared_ptr<ChangeStr> &a,
                          const std::shared_ptr<ChangeStr> &b) {
  if (a->left_pos_ < b->left_pos_) {
    return true;
  } else if (a->left_pos_ > b->left_pos_) {
    return false;
  } else {
    return a->right_pos_ < b->right_pos_;
  }
}

PrsmStr::PrsmStr(const std::vector<std::string> &str_vec) {
  str_vec_ = str_vec;
  std::string line = PrsmUtil::getXmlLine(str_vec_, "<spectrum_id>");
  spectrum_id_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<precursor_id>");
  precursor_id_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<precursor_feature_id>");
  precursor_feature_id_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<precursor_feature_inte>");
  precursor_feature_inte_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<adjusted_prec_mass>");
  adjusted_prec_mass_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<seq_name>");
  seq_name_ = PrsmUtil::getValueStr(line);
  line = PrsmUtil::getXmlLine(str_vec_, "<seq_desc>");
  seq_desc_ = PrsmUtil::getValueStr(line);
  line = PrsmUtil::getXmlLine(str_vec_, "<match_fragment_num>");
  match_frag_num_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<norm_match_fragment_num>");
  norm_match_frag_num_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<e_value>");
  if (line == "") {
    e_value_ = 0.0;
  } else {
    e_value_ = std::stod(PrsmUtil::getValueStr(line));
  }
  line = PrsmUtil::getXmlLine(str_vec_, "<fdr>");
  fdr_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<proteoform_fdr>");
  proteoform_fdr_ = std::stod(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<start_pos>");
  proteoform_start_pos_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<end_pos>");
  proteoform_end_pos_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<species_id>");
  species_id_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<prot_id>");
  prot_id_ = std::stoi(PrsmUtil::getValueStr(line));
  line = PrsmUtil::getXmlLine(str_vec_, "<unexpected_ptm_num>");
  unexpected_ptm_num_ = std::stoi(PrsmUtil::getValueStr(line));

  std::vector<std::string> mass_lines = PrsmUtil::getXmlLineVec(str_vec_, "<mass_shift>");
  std::vector<std::string> left_pos_lines = PrsmUtil::getXmlLineVec(str_vec_, "<left_bp_pos>");
  std::vector<std::string> right_pos_lines = PrsmUtil::getXmlLineVec(str_vec_, "<right_bp_pos>");

  for (size_t i = 0; i < mass_lines.size(); i++) {
    change_vec_.push_back(std::make_shared<ChangeStr>(std::stod(PrsmUtil::getValueStr(mass_lines[i])),
                                                      std::stoi(PrsmUtil::getValueStr(left_pos_lines[i])),
                                                      std::stoi(PrsmUtil::getValueStr(right_pos_lines[i]))));
  }
}

int getXmlLineIndex(const std::vector<std::string> &str_vec,
                    const std::string &property) {
  for (size_t i = 0; i < str_vec.size(); i++) {
    size_t found = str_vec[i].find(property);
    if (found != std::string::npos) {
      return i;
    }
  }
  return -1;
}

void PrsmStr::setFdr(double fdr) {
  int i = getXmlLineIndex(str_vec_, "fdr");
  str_vec_[i] = "<fdr>" + std::to_string(fdr) + "</fdr>";
  fdr_ = fdr;
}

void PrsmStr::setProteoformFdr(double proteoform_fdr) {
  int i = getXmlLineIndex(str_vec_, "proteoform_fdr");
  str_vec_[i] = "<proteoform_fdr>" + std::to_string(proteoform_fdr) + "</proteoform_fdr>";
  proteoform_fdr_ = proteoform_fdr;
}

void PrsmStr::setSpectrumId(int id) {
  int i = getXmlLineIndex(str_vec_, "spectrum_id");
  str_vec_[i] = "<spectrum_id>" + std::to_string(id) + "</spectrum_id>";
  spectrum_id_ = id;
}

void PrsmStr::setPrecFeatureId(int id) {
  int i = getXmlLineIndex(str_vec_, "precursor_feature_id");
  str_vec_[i] = "<precursor_feature_id>" + std::to_string(id) + "</precursor_feature_id>";
  precursor_feature_id_ = id;
}

void PrsmStr::setPrecFeatureInte(double inte) {
  int i = getXmlLineIndex(str_vec_, "precursor_feature_inte");
  str_vec_[i] = "<precursor_feature_inte>" + std::to_string(inte) + "</precursor_feature_inte>";
  precursor_feature_inte_ = inte;
}

void PrsmStr::setSpeciesId(int id) {
  int i = getXmlLineIndex(str_vec_, "species_id");
  str_vec_[i] = "<species_id>" + std::to_string(id) + "</species_id>";
  species_id_ = id;
}

void PrsmStr::setProtId(int id) {
  int i = getXmlLineIndex(str_vec_, "prot_id");
  str_vec_[i] = "<prot_id>" + std::to_string(id) + "</prot_id>";
  prot_id_ = id;
}

bool PrsmStr::isSameSeqAndMass(const PrsmStrPtr &a, const PrsmStrPtr &b, double ppo) {
  if (a->getSeqName() != b->getSeqName()) {
    return false;
  }
  if (a->getProteoformStartPos() != b->getProteoformStartPos()) {
    return false;
  }
  if (a->getProteoformEndPos() != b->getProteoformEndPos()) {
    return false;
  }
  double thresh = a->getAdjustedPrecMass() * ppo;
  if (std::abs(a->getAdjustedPrecMass() - b->getAdjustedPrecMass()) > thresh) {
    return false;
  }
  return true;
}

bool PrsmStr::isStrictCompatiablePtmSpecies(const PrsmStrPtr & a, const PrsmStrPtr & b, double ppo) {
  if (!isSameSeqAndMass(a, b, ppo)) {
    return false;
  }

  if (a->getChangeStrVec().size() != b->getChangeStrVec().size()) {
    return false;
  }

  double shift_tolerance = a->getAdjustedPrecMass() * ppo;
  std::vector<std::shared_ptr<ChangeStr> > a_change_vec = a->getChangeStrVec();
  std::vector<std::shared_ptr<ChangeStr> > b_change_vec = b->getChangeStrVec();
  std::sort(a_change_vec.begin(), a_change_vec.end(), ChangeStr::cmpPosInc);
  std::sort(b_change_vec.begin(), b_change_vec.end(), ChangeStr::cmpPosInc);
  for (size_t i = 0; i < a->getChangeStrVec().size(); i++) {
    ChangeStrPtr ac = a_change_vec[i];
    ChangeStrPtr bc = b_change_vec[i];
    if (ac->right_pos_ <= bc->left_pos_ || bc->right_pos_ <= ac->left_pos_) {
      return false;
    }
    if (std::abs(ac->mass_shift_ - bc->mass_shift_) > shift_tolerance) {
      return false;
    }
  }
  return true;
}

}  // namespace prot
