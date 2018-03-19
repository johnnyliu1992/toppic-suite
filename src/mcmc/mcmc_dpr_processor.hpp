//Copyright (c) 2014 - 2018, The Trustees of Indiana University.
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

#ifndef PROT_MCMC_DPR_PROCESSOR_HPP_
#define PROT_MCMC_DPR_PROCESSOR_HPP_


#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#include "base/proteoform.hpp"
#include "base/activation.hpp"

#include "spec/deconv_ms.hpp"

#include "prsm/prsm.hpp"

#include "tdgf/count_test_num.hpp"
#include "tdgf/comp_pvalue_lookup_table.hpp"

#include "mcmc/mcmc_mng.hpp"

namespace prot {

class DprProcessor {
 public:
  explicit DprProcessor(MCMCMngPtr mng_ptr):
      mng_ptr_(mng_ptr), mt_(42) {
        init();
      }

  void process();

 private:
  void init();

  void read_mass_table();

  std::vector<std::vector<double> > compPtmComb();

  void processOnePrsm(PrsmPtr prsm_ptr, SpectrumSetPtr spec_set_ptr, double tolerance);

  double compPValueMCMC(PrsmPtr prsm_ptr, ActivationPtr act,
                        const std::vector<int> & ms_masses);

  void simulateDPR(ResiduePtrVec &residues, const std::vector<int> & ms_masses,
                   ActivationPtr act, const PtmPtrVec & ptm_vec,
                   long omega, const std::vector<long long> & mu);

  ResiduePtrVec randomTrans(ResiduePtrVec residues);

  int getMaxScore(const ResiduePtrVec &residues, const std::vector<int> & ms_masses,
                  ActivationPtr act, const PtmPtrVec & ptm_vec);

  int compScore(const std::vector<int> & ms_masses,
                std::vector<double> n_theo_masses, std::vector<double> c_theo_masses,
                const std::vector<size_t> & change_pos, const PtmPtrVec & ptm_vec);

  int compNumMatched(const std::vector<int> &ms_masses, const std::vector<double> &theo_masses);

  MCMCMngPtr mng_ptr_;

  CountTestNumPtr test_num_ptr_;

  CompPValueLookupTablePtr comp_pvalue_table_ptr_;

  std::random_device rd_;

  std::mt19937 mt_;

  SpParaPtr sp_para_ptr_;

  PtmPtrVec ptm_vec_;

  std::vector<std::vector<double> > ptm_mass_vec2d_;

  std::map<PtmPtr, std::vector<ResiduePtr> > ptm_residue_map_;

  std::map<int, std::vector<std::string> > mass_table_;

  std::vector<int> score_vec_;

  int z_;

  double pep_mass_;
};

typedef std::shared_ptr<DprProcessor> DprProcessorPtr;

}  // namespace prot

#endif
