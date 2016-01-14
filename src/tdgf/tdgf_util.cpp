
#include "base/residue_util.hpp"
#include "tdgf/tdgf_util.hpp"

namespace prot {

void TdgfUtil::updateResidueCounts(const ResiduePtrVec &residue_list, 
                                   std::vector<double> &counts,
                                   ProteoformPtr prot_ptr) {
  ResSeqPtr seq_ptr = prot_ptr->getResSeqPtr();    
  for (int i = 0; i < seq_ptr->getLen(); i++) {
    ResiduePtr res_ptr = seq_ptr->getResiduePtr(i);
    int pos = ResidueUtil::findResidue(residue_list, res_ptr);
    if (pos >= 0) {
      // found 
      counts[pos] = counts[pos]+1;
    }
  }
}

ResFreqPtrVec TdgfUtil::compResidueFreq(const ResiduePtrVec &residue_list, 
                                        const std::vector<double> &counts) {
  double sum = 0;
  for (size_t i = 0; i < counts.size(); i++) {
    sum = sum + counts[i];
  }
  ResFreqPtrVec res_freq_list;
  for (size_t i = 0; i < residue_list.size(); i++) {
    ResFreqPtr res_freq_ptr(new ResidueFreq(residue_list[i]->getAcidPtr(), 
                                            residue_list[i]->getPtmPtr(),
                                            counts[i]/sum));
    res_freq_list.push_back(res_freq_ptr);
  }
  return res_freq_list;
}

void TdgfUtil::updateNTermResidueCounts(ResiduePtrVec &residue_list, std::vector<double> &counts,
                                        const ProteoformPtrVec &mod_proteo_ptrs) {
  for (size_t i = 0; i < mod_proteo_ptrs.size(); i++) {
    ResSeqPtr seq_ptr = mod_proteo_ptrs[i]->getResSeqPtr();    
    if (seq_ptr->getLen() >= 1) {
      ResiduePtr res_ptr = seq_ptr->getResiduePtr(0);
      int pos = ResidueUtil::findResidue(residue_list, res_ptr);
      if (pos >= 0) {
        // found 
        counts[pos] = counts[pos]+1;
      }
      else {
        residue_list.push_back(res_ptr);
        counts.push_back(1);
      }
    }
  }
}

int TdgfUtil::computeAvgLength(const ResFreqPtrVec &residue_ptrs, double convert_ratio) {
  double mass_sum = 0;
  double freq_sum = 0;
  for (size_t i = 0; i < residue_ptrs.size(); i++) {
    freq_sum = freq_sum + residue_ptrs[i]->getFreq();
    mass_sum = mass_sum + residue_ptrs[i]->getFreq() * residue_ptrs[i]->getMass();
  }
  return (int)std::round(mass_sum/freq_sum * convert_ratio);
}

}