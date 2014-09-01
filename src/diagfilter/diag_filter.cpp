#include <algorithm>
#include <iostream>
#include "diagfilter/diag_filter.hpp"

namespace prot {

DiagFilter::DiagFilter(const ProteoformPtrVec &proteo_ptrs,
                       DiagFilterMngPtr mng_ptr){
  mng_ptr_ = mng_ptr;
  proteo_ptrs_ = proteo_ptrs;
  index_ptr_ = CompShiftPtr(new CompShift(proteo_ptrs, mng_ptr));
}

SimplePrsmPtrVec DiagFilter::getBestMatch(PrmMsPtr ms_ptr){
  SimplePrsmPtrVec match_ptrs = compute(ms_ptr);
  SimplePrsmPtrVec unique_match_ptrs = getUniqueMatches(match_ptrs);
  std::sort(unique_match_ptrs.begin(), unique_match_ptrs.end(),simplePrsmDown);
  size_t num = mng_ptr_->ptm_fast_filter_result_num_;
  if(num > unique_match_ptrs.size()){
    num = unique_match_ptrs.size();
  }
  SimplePrsmPtrVec result_ptrs;
  for(size_t i=0;i<num;i++){
    SimplePrsmPtr match_ptr = unique_match_ptrs[i];
    if(match_ptr->getScore() > 0.0){
      result_ptrs.push_back(match_ptr);
    }
    else{
      break;
    }
  }
  return result_ptrs;
}

inline SimplePrsmPtrVec DiagFilter::compute(PrmMsPtr ms_ptr){
  std::pair<std::vector<int>, std::vector<int>> mass_errors 
      = getIntMassErrorList(ms_ptr, mng_ptr_->ptm_fast_filter_scale_,true,false);
  SimplePrsmPtrVec match_ptrs;
  for(size_t i=0;i<mass_errors.first.size();i++){
    std::vector<std::pair<int,int>> results 
        =index_ptr_->compConvolution(mass_errors.first, mass_errors.second, i,
                                     mng_ptr_->ptm_fast_filter_result_num_);
    for(size_t j =0;j <results.size();j++){
      match_ptrs.push_back(
          SimplePrsmPtr(new SimplePrsm(ms_ptr->getHeaderPtr(),
                                       proteo_ptrs_[results[j].first],
                                       results[j].second)));
    }
  }
  return match_ptrs;
}

} /* namespace prot */
