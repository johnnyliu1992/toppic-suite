/*
 * prsm_combine.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: xunlikun
 */

#include "base/file_util.hpp"
#include "prsm/prsm_combine.hpp"

namespace prot {

PrsmCombine::PrsmCombine(std::string db_file,
                         std::string spec_file,
                         std::vector<std::string> &in_file_exts,
                         std::string out_file) {
  input_file_exts_ = in_file_exts;
  output_files_ = out_file;
  spec_file_ = spec_file;
  db_file_ = db_file;
}

PrsmCombine::PrsmCombine(std::map<std::string, std::string> arguments,
                         std::vector<std::string> &in_file_exts,
                         std::string out_file) {
  input_file_exts_ = in_file_exts;
  output_files_ = out_file;
  spec_file_ = arguments["spectrumFileName"];
  db_file_ = arguments["databaseFileName"];
}

PrsmCombine::~PrsmCombine() {
}

void PrsmCombine::process() {
  ProteoformPtrVec proteoforms = prot::readFastaToProteoform(db_file_,ResidueFactory::getBaseResiduePtrVec());
  PrsmPtrVec prsms;
  for(unsigned int i=0;i<input_file_exts_.size();i++){
    PrsmPtrVec temps = prot::readPrsm(basename(spec_file_)+"."+input_file_exts_[i],proteoforms);
    for(unsigned int j=0;j<temps.size();j++){
//      std::cout<<temps[j]->getSpectrumId()<<std::endl;
      prsms.push_back(temps[j]);
    }
  }
  std::sort(prsms.begin(),prsms.end(),prsmSpectrumIdUpMatchFragUp);
  PrsmWriter all_writer(basename(spec_file_)+"."+output_files_);
  all_writer.writeVector(prsms);

  //because the prsm_writer ~PrsmWriter changed and the fileclosing is an independant function
  all_writer.close();
}

} /* namespace prot */
