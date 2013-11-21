#ifndef PROT_RESIDUE_UTIL_H_
#define PROT_RESIDUE_UTIL_H_

#include <vector>

#include "residue.hpp"

namespace prot {

/**
 * Returns the first residue based on the acid and ptm. 
 */
ResiduePtr getResiduePtrByAcidPtm(ResiduePtrVec &residue_ptr_vec,
                                  AcidPtr acid_ptr, PtmPtr ptm_ptr);

ResiduePtrVec getResiduePtrVecInstance(AcidPtrVec &acid_ptr_vec, 
                                       PtmPtrVec &ptm_ptr_vec,
                                       const char* file_name); 

ResiduePtr addResidue(ResiduePtrVec &residue_ptr_vec, AcidPtr acid_ptr,
                      PtmPtr ptm_ptr);
}
#endif
