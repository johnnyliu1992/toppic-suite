/*
 * cleavage.hpp
 *
 *  Created on: Feb 24, 2014
 *      Author: xunlikun
 */

#ifndef PROT_CLEAVAGE_HPP_
#define PROT_CLEAVAGE_HPP_

#include "base/proteoform.hpp"
#include "spec/extend_peak.hpp"
#include "prsm/peak_ion_pair.hpp"

namespace prot {

class Cleavage {
 public:
  Cleavage(int pos);
  void setPairs(PeakIonPairPtrVec pairs){
    pairs_ = pairs;
  }
  void setExistNIon(bool n){exist_n_ion_ = n;};
  void setExistCIon(bool c){exist_c_ion_ = c;};
  void appendXml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent);
 private:
  int pos_;
  bool exist_n_ion_;
  bool exist_c_ion_;
  PeakIonPairPtrVec pairs_;
};

typedef std::shared_ptr<Cleavage> CleavagePtr;
typedef std::vector<CleavagePtr> CleavagePtrVec;

CleavagePtrVec getProteoCleavage(ProteoformPtr prot,
                                 ExtendMsPtr ms_three,
                                 double min_mass);
} /* namespace prot */

#endif /* CLEAVAGE_HPP_ */