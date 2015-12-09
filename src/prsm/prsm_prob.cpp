#include "base/file_util.hpp"
#include "prsm/prsm_reader.hpp"
#include "prsm/prsm_prob.hpp"

namespace prot {

PrsmProb::PrsmProb(const std::string &db_file_name, 
                   const std::string &spec_file_name, 
                   const ModPtrVec &fix_mod_ptr_vec,
                   const std::string &in_file_ext,
                   const std::string &out_file_ext, 
                   double K1, double K2, double pref, double inte): 
    db_file_name_(db_file_name),
    spec_file_name_(spec_file_name),
    fix_mod_ptr_vec_(fix_mod_ptr_vec),
    input_file_ext_(in_file_ext),
    output_file_ext_(out_file_ext),
    K1_(K1),
    K2_ (K2),
    pref_(pref),
    inte_(inte) {
    }

void PrsmProb::process() {

  std::string input_file_name = FileUtil::basename(spec_file_name_)+ "." + input_file_ext_;
  FastaIndexReaderPtr seq_reader(new FastaIndexReader(db_file_name_));
  PrsmReader prsm_reader(input_file_name);
  PrsmPtr prsm_ptr = prsm_reader.readOnePrsm(seq_reader, fix_mod_ptr_vec_);

  PrsmXmlWriter all_writer(FileUtil::basename(spec_file_name_)+"."+output_file_ext_);

  while(prsm_ptr != nullptr) {
    int shift_num = prsm_ptr->getProteoformPtr()->getChangeNum(ChangeType::UNEXPECTED);
    AlignTypePtr type_ptr = prsm_ptr->getProteoformPtr()->getAlignType();
    ExtremeValuePtr prob_ptr = prsm_ptr->getExtremeValuePtr();
    if (shift_num == 1) {
      prob_ptr->setOneProtProb(prob_ptr->getOneProtProb() * K1_);
    }
    if (shift_num == 2) {
      prob_ptr->setOneProtProb(prob_ptr->getOneProtProb() * K2_);
    }
    if (type_ptr == AlignType::PREFIX || type_ptr == AlignType::SUFFIX) {
      prob_ptr->setOneProtProb(prob_ptr->getOneProtProb() * pref_);
    }

    if (type_ptr == AlignType::INTERNAL) {
      prob_ptr->setOneProtProb(prob_ptr->getOneProtProb() * inte_);
    }
    all_writer.write(prsm_ptr);
    prsm_ptr = prsm_reader.readOnePrsm(seq_reader, fix_mod_ptr_vec_);
  }

  prsm_reader.close();
  all_writer.close();
}

} /* namespace prot */
