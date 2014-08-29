#ifndef ANNO_VIEW_HPP_
#define ANNO_VIEW_HPP_

#include <map>

#include "base/extreme_value.hpp"
#include "base/proteoform.hpp"
#include "base/anno_residue.hpp"
#include "spec/deconv_peak.hpp"
#include "spec/extend_peak.hpp"
#include "spec/sp_para.hpp"
#include "prsm/cleavage.hpp"
#include "prsm/prsm.hpp"

#include "xpp/view_mng.hpp"

namespace prot{
class AnnoView {
 public:
  std::vector<std::vector<std::string>> file_list_;
  xercesc::DOMElement* geneFileList(XmlDOMDocument* xml_doc);
};

typedef std::shared_ptr<AnnoView> AnnoViewPtr;

std::vector<std::vector<std::string>> readViewXmlFiles(const std::string &file_name);

xercesc::DOMElement* genePrsmView(XmlDOMDocument* xml_doc, PrsmPtr prsm_ptr, ViewMngPtr mng_ptr);

xercesc::DOMElement* geneProteinView(XmlDOMDocument* xml_doc,ProteoformPtr proteo_ptr,
                                     ExtendMsPtr ms_three_ptr, ViewMngPtr mng_ptr);

xercesc::DOMElement* speciesToXml(XmlDOMDocument* xml_doc, const PrsmPtrVec &prsm_ptrs, 
                                  ViewMngPtr mng_ptr);

xercesc::DOMElement* proteinToXml(XmlDOMDocument* xml_doc,
                                  const PrsmPtrVec &prsm_ptrs,
                                  ProteoformPtr proteo_ptr,
                                  const std::vector<int> &species_ids,
                                  ViewMngPtr mng_ptr);


xercesc::DOMElement* allProteinToXml(XmlDOMDocument* xml_doc,
                                     const PrsmPtrVec &prsm_ptrs,
                                     const ProteoformPtrVec &proteo_ptrs,
                                     ViewMngPtr mng_ptr);
}

#endif /* ANNO_VIEW_HPP_ */
