#include <base/logger.hpp>

#include "base/acid_base.hpp"
#include "base/ptm_base.hpp"
#include "base/residue.hpp"
#include "base/file_util.hpp"
#include "base/string_util.hpp"
#include "base/xml_dom_util.hpp"

namespace prot {

Residue::Residue(AcidPtr acid_ptr, PtmPtr ptm_ptr):
    acid_ptr_(acid_ptr),
    ptm_ptr_(ptm_ptr) {
      mass_ = acid_ptr_->getMonoMass() + ptm_ptr_->getMonoMass();
    }

Residue::Residue(const std::string &acid_name, 
                 const std::string &ptm_abbr_name) {
  acid_ptr_ = AcidBase::getAcidPtrByName(acid_name);
  ptm_ptr_ = PtmBase::getPtmPtrByAbbrName(ptm_abbr_name);
  mass_ = acid_ptr_->getMonoMass() + ptm_ptr_->getMonoMass();
}

Residue::Residue(xercesc::DOMElement* element) { 
  std::string acid_element_name = Acid::getXmlElementName();
  xercesc::DOMElement* acid_element 
      = XmlDomUtil::getChildElement(element, acid_element_name.c_str(), 0);
  acid_ptr_ = AcidBase::getAcidPtrFromXml(acid_element);
  std::string ptm_element_name = Ptm::getXmlElementName();
  xercesc::DOMElement* ptm_element 
      = XmlDomUtil::getChildElement(element, ptm_element_name.c_str(), 0);
  ptm_ptr_ = PtmBase::getPtmPtrFromXml(ptm_element);  
  mass_ = acid_ptr_->getMonoMass() + ptm_ptr_->getMonoMass();
}

std::string Residue::toString(const std::string &delim_bgn, 
                              const std::string &delim_end) {
  if (PtmBase::isEmptyPtmPtr(ptm_ptr_)) {
    return acid_ptr_->getOneLetter();
  } else {
    return acid_ptr_->getOneLetter() + delim_bgn + ptm_ptr_->getAbbrName()
        + delim_end;
  }
}

void Residue::appendXml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent){
  std::string element_name = Residue::getXmlElementName();
  xercesc::DOMElement* element = xml_doc->createElement(element_name.c_str());
  std::string str = StringUtil::convertToString(mass_);
  xml_doc->addElement(element, "mass", str.c_str());
  acid_ptr_->appendNameToXml(xml_doc,element);
  ptm_ptr_->appendAbbrNameToXml(xml_doc,element);
  parent->appendChild(element);
}

}
