#include "base/logger.hpp"
#include "base/acid_base.hpp"
#include "base/ptm_base.hpp"
#include "base/residue_base.hpp"
#include "base/residue_util.hpp"

namespace prot {

ResiduePtrVec ResidueUtil::convertStrToResiduePtrVec(const std::string &seq) {
  ResiduePtrVec residue_ptr_vec;
  for (size_t i = 0; i < seq.length(); i++) {
    AcidPtr acid_ptr = AcidBase::getAcidPtrByOneLetter(seq.substr(i, 1));
    PtmPtr ptm_ptr = PtmBase::getEmptyPtmPtr();
    ResiduePtr residue_ptr = ResidueBase::getBaseResiduePtr(acid_ptr, ptm_ptr);
    residue_ptr_vec.push_back(residue_ptr);
  }
  return residue_ptr_vec;
}

ResiduePtrVec ResidueUtil::convertStrToResiduePtrVec(const std::string &seq, 
                                                     ModPtrVec &fix_mod_ptr_vec) {
  ResiduePtrVec residue_ptrs = ResidueUtil::convertStrToResiduePtrVec(seq);
  for (size_t i = 0; i < residue_ptrs.size(); i++) {
    for (size_t j = 0; j < fix_mod_ptr_vec.size(); j++) {
      if (residue_ptrs[i] == fix_mod_ptr_vec[j]->getOriResiduePtr()) {
        residue_ptrs[i] = fix_mod_ptr_vec[j]->getModResiduePtr();
        break;
      }
    }
  }
  return residue_ptrs;
}

int ResidueUtil::findResidue(const ResiduePtrVec &residue_list, 
                             ResiduePtr residue_ptr) {
  for (size_t i = 0; i < residue_list.size(); i++) {
    if (residue_list[i] == residue_ptr) {
      return i;
    }
  }
  return -1;
}

double ResidueUtil::compResiduePtrVecMass(const ResiduePtrVec &ptr_vec) {
  double mass = 0;
  for (size_t i = 0; i < ptr_vec.size(); i++) {
    mass += ptr_vec[i]->getMass();
  }
  return mass;
}

}
