#ifndef PROT_BASE_PROT_MOD_UTIL_HPP_
#define PROT_BASE_PROT_MOD_UTIL_HPP_

#include "base/prot_mod.hpp"
#include "base/residue.hpp"

namespace prot {

class ProtModUtil {
 public:
  static bool allowMod(ProtModPtr prot_mod_ptr, const ResiduePtrVec &residues); 

  static ProtModPtrVec readProtMod(const std::string &file_name);

  static ProtModPtr findNME_Acetylation(const ProtModPtrVec &prot_mod_ptrs, const ResiduePtrVec &residues);

};

}
#endif
