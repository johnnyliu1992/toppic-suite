#ifndef PROT_PRSM_PRSM_READER_HPP_
#define PROT_PRSM_PRSM_READER_HPP_

#include <iostream>
#include <fstream>

#include "base/xml_dom_document.hpp"
#include "prsm/prsm.hpp"
#include "prsm/prsm_str.hpp"

namespace prot {

class PrsmReader {
 public:
  PrsmReader(const std::string &file_name);

  std::vector<std::string> readOnePrsmLines();

  PrsmStrPtr readOnePrsmStr();
  
  PrsmPtr readOnePrsm(FastaIndexReaderPtr reader_ptr, 
                      const ModPtrVec fix_mod_list);

  void close();

 private:
  std::ifstream input_;
};

typedef std::shared_ptr<PrsmReader> PrsmReaderPtr;
typedef std::vector<PrsmReaderPtr> PrsmReaderPtrVec;

PrsmStrPtrVec readAllPrsmStrs(const std::string &input_file_name);

PrsmPtrVec readAllPrsms(const std::string &prsm_file_name, 
                        const std::string &db_file_name,
                        const ModPtrVec  &fix_mod_list);
} /* namespace prot */

#endif /* PROT_PRSM_READER_HPP_ */
