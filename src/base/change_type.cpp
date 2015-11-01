#include "base/change_type.hpp"

namespace prot {

const ChangeTypePtr ChangeType::INPUT = ChangeTypePtr(new ChangeType("Input"));
const ChangeTypePtr ChangeType::FIXED = ChangeTypePtr(new ChangeType("Fixed"));
const ChangeTypePtr ChangeType::PROTEIN_VARIABLE = ChangeTypePtr(new ChangeType("Protein variable"));
const ChangeTypePtr ChangeType::VARIABLE = ChangeTypePtr(new ChangeType("Variable"));
const ChangeTypePtr ChangeType::UNEXPECTED = ChangeTypePtr(new ChangeType("Unexpected"));

void ChangeType::appendXml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent){
  std::string element_name = ChangeType::getXmlElementName();
  xercesc::DOMElement* element = xml_doc->createElement(element_name.c_str());
  xml_doc->addElement(element, "name", name_.c_str());
  parent->appendChild(element);
}

ChangeTypePtr ChangeType::getChangeTypePtrFromXml(xercesc::DOMElement * element) {
  std::string name = getChildValue(element, "name", 0);
  if (name == ChangeType::INPUT->getName()) {
    return ChangeType::INPUT;
  }
  if (name == ChangeType::FIXED->getName()) {
    return ChangeType::FIXED;
  }
  if (name == ChangeType::PROTEIN_VARIABLE->getName()) {
    return ChangeType::PROTEIN_VARIABLE;
  }
  if (name == ChangeType::VARIABLE->getName()) {
    return ChangeType::VARIABLE;
  }
  if (name == ChangeType::UNEXPECTED->getName()) {
    return ChangeType::UNEXPECTED;
  }
  return nullptr;
}

}
