// Copyright (c) 2014 - 2016, The Trustees of Indiana University.
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


#include <cmath>

#include "base/logger.hpp"
#include "base/fasta_reader.hpp"
#include "base/mod_util.hpp"
#include "base/residue_base.hpp"
#include "base/residue_util.hpp"
#include "base/proteoform_factory.hpp"
#include "tdgf/tdgf_util.hpp"
#include "tdgf/count_test_num.hpp"
#include "tdgf/comp_prob_value.hpp"

namespace prot {

CountTestNum::CountTestNum(TdgfMngPtr mng_ptr) {
  convert_ratio_ = mng_ptr->convert_ratio_;
  max_ptm_mass_ = mng_ptr->max_ptm_mass_;
  max_sp_len_ = (int)std::round(mng_ptr->max_prec_mass_ * convert_ratio_);
  init(mng_ptr->prsm_para_ptr_);
  LOG_DEBUG("count numbers initialized");
}

CountTestNum::~CountTestNum() {
  if (comp_mass_cnts_ != nullptr) {
    delete comp_mass_cnts_;
  }
  if (pref_mass_cnts_ != nullptr) {
    delete pref_mass_cnts_;
  }
  if (suff_mass_cnts_ != nullptr) {
    delete suff_mass_cnts_;
  }
  if (internal_mass_cnts_ != nullptr) {
    delete internal_mass_cnts_;
  }
}

inline int CountTestNum::convertMass(double m) {
  int n = (int) std::round(m * convert_ratio_);
  if (n < 0) {
    LOG_WARN("Negative mass value: " << m);
    return 0;
  }
  if (n >= max_sp_len_) {
    n = max_sp_len_ - 1;
  }
  return n;
}


void CountTestNum::init(PrsmParaPtr para_ptr) {
  std::string db_file_name = para_ptr->getSearchDbFileName();
  comp_mass_cnts_ = new double[max_sp_len_](); 
  pref_mass_cnts_ = new double[max_sp_len_]();
  suff_mass_cnts_ = new double[max_sp_len_]();

  ResiduePtrVec non_ptm_residue_list = ResidueBase::getBaseNonePtmResiduePtrVec();
  ModPtrVec fix_mod_list = para_ptr->getFixModPtrVec();
  ResiduePtrVec residue_list = ModUtil::geneResidueListWithMod(non_ptm_residue_list, fix_mod_list);
  
  std::vector<double> residue_counts(residue_list.size(), 0.0);

  ResiduePtrVec n_term_residue_list;
  std::vector<double> n_term_residue_counts;

  ProtModPtrVec prot_mods = para_ptr->getProtModPtrVec();
  FastaReader reader(db_file_name);
  FastaSeqPtr seq_ptr = reader.getNextSeq();
  
  while (seq_ptr != nullptr) {
    ProteoformPtr proteo_ptr = ProteoformFactory::geneDbProteoformPtr(seq_ptr, fix_mod_list);
    ProteoformPtrVec mod_proteo_ptrs = ProteoformFactory::geneProtModProteoform(proteo_ptr, prot_mods);
    for (size_t i = 0; i < mod_proteo_ptrs.size(); i++) {
      // complete
      double m = mod_proteo_ptrs[i]->getResSeqPtr()->getResMassSum();
      comp_mass_cnts_[convertMass(m)] += 1.0;
      // prefix
      std::vector<double> prm_masses = mod_proteo_ptrs[i]->getBpSpecPtr()->getPrmMasses();
      for (size_t j = 1; j < prm_masses.size() - 1; j++) {
        pref_mass_cnts_[convertMass(prm_masses[j])] += 1.0;
      }
    }
    //suffix
    BreakPointPtrVec break_points = proteo_ptr->getBpSpecPtr()->getBreakPointPtrVec();
    for (size_t i = 1; i < break_points.size() - 1; i++) {
      suff_mass_cnts_[convertMass(break_points[i]->getSrm())] += 1.0;
    }

    // length
    raw_proteo_lens_.push_back(proteo_ptr->getLen());
    for (size_t i = 0; i < mod_proteo_ptrs.size(); i++) {
      mod_proteo_lens_.push_back(mod_proteo_ptrs[i]->getLen());
    }

    // update residue counts 
    TdgfUtil::updateResidueCounts(residue_list,residue_counts, proteo_ptr);

    // update n terminal residue counts
    TdgfUtil::updateNTermResidueCounts(n_term_residue_list, n_term_residue_counts, mod_proteo_ptrs);

    // next protein 
    seq_ptr = reader.getNextSeq();
  }
  // compute residue freq;
  residue_ptrs_ =  TdgfUtil::compResidueFreq(residue_list, residue_counts);

  // compute residue average length
  residue_avg_len_ = TdgfUtil::computeAvgLength(residue_ptrs_, convert_ratio_);

  // compute n term residue freq;
  prot_n_term_residue_ptrs_ =  TdgfUtil::compResidueFreq(n_term_residue_list, n_term_residue_counts);

  // internal 
  initInternalMassCnt();
}

inline void CountTestNum::initInternalMassCnt() {
  // init to 0
  internal_mass_cnts_ = new double[max_sp_len_]();
  // middle
  double norm_count = 0;
  // use approxiation to speed up
  LOG_DEBUG("residue_avg_len_ " << residue_avg_len_);
  for (int i = max_sp_len_ - 1; i >= 0; i--) {
    norm_count += suff_mass_cnts_[i];
    internal_mass_cnts_[i] = norm_count/ residue_avg_len_;
  }
}

double CountTestNum::compCandNum(AlignTypePtr type_ptr, int index, 
                                 double ori_mass, double ori_tolerance) {
    double cand_num = 0;
    if (index == 0) {
        cand_num = compNonPtmCandNum(type_ptr, ori_mass, ori_tolerance);
    } else if (index >= 1){ // with shifts
        /*
        if (max_ptm_mass_ >=10000) {
            // max shift mass is larger than 10k, we treat it as no limitation 
            cand_num = compPtmCandNum(type_ptr, ori_mass);
        }
        else {
        }
        */
        cand_num = compPtmRestrictCandNum(type_ptr, index, ori_mass);
        // multiple adjustment 
        if (type_ptr == AlignType::PREFIX || type_ptr == AlignType::SUFFIX) {
            cand_num = cand_num * PREFIX_SUFFIX_ADJUST();
        }
        else if (type_ptr == AlignType::INTERNAL) {
            cand_num = cand_num * INTERNAL_ADJUST();
        }
    }

    if (cand_num == 0.0) {
        LOG_WARN("candidate number is ZERO");
    }
    return cand_num;
}

double CountTestNum::compNonPtmCandNum(AlignTypePtr type_ptr, 
                                       double ori_mass, double ori_tolerance) {
  int low = std::floor((ori_mass - ori_tolerance) * convert_ratio_);
  int high = std::ceil((ori_mass + ori_tolerance) * convert_ratio_);
  double cand_num = compSeqNum(type_ptr, low, high);
  
  //if (type_ptr == SemiAlignTypeFactory::getCompletePtr()) {
  //LOG_DEBUG("low " << low << " high " << high << " cand num " << cand_num);
  //}
  
  return cand_num;
}

double CountTestNum::compPtmCandNum (AlignTypePtr type_ptr, double ori_mass) {
  double cand_num = 0;
  if (type_ptr == AlignType::COMPLETE) {
    cand_num = mod_proteo_lens_.size();
  } else if (type_ptr == AlignType::PREFIX) {
    for (size_t i = 0; i < raw_proteo_lens_.size(); i++) {
      cand_num += mod_proteo_lens_[i];
    }
  } else if (type_ptr == AlignType::SUFFIX) {
    for (size_t i = 0; i < raw_proteo_lens_.size(); i++) {
      cand_num += raw_proteo_lens_[i];
    }
  } else if (type_ptr == AlignType::INTERNAL) {
    for (size_t i = 0; i < raw_proteo_lens_.size(); i++) {
      cand_num = cand_num + raw_proteo_lens_[i] * raw_proteo_lens_[i];
    }
  }
  return cand_num;
}

double CountTestNum::compPtmRestrictCandNum (AlignTypePtr type_ptr, 
                                             int shift_num, double ori_mass) {
  double shift = max_ptm_mass_ * shift_num;
  int low = std::floor((ori_mass - shift) * convert_ratio_);
  int high = std::ceil((ori_mass + shift) * convert_ratio_);
  double cand_num = compSeqNum(type_ptr, low, high);
  return cand_num;
}

double CountTestNum::compSeqNum(AlignTypePtr type_ptr, int low, int high) {
  double candNum = 0;
  if (type_ptr == AlignType::COMPLETE) {
    candNum = compMassNum(comp_mass_cnts_, low, high);
  } else if (type_ptr == AlignType::PREFIX) {
    candNum = compMassNum(pref_mass_cnts_, low, high);
  } else if (type_ptr == AlignType::SUFFIX) {
    candNum = compMassNum(suff_mass_cnts_, low, high);
  } else if (type_ptr == AlignType::INTERNAL) {
    candNum = compMassNum(internal_mass_cnts_, low, high);
  }
  return candNum;
}

double CountTestNum::compMassNum(double *cnts, int low, int high) {
  double cnt = 0;
  if (high >= max_sp_len_) {
    high = max_sp_len_ - 1;
  }
  if (low < 0) {
    low = 0;
  }
  if (low > high) {
    low = high;
  }
  for (int i = low; i <= high; i++) {
    cnt += cnts[i];
  }
  return cnt;
}

}
