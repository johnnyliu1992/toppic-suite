// Copyright (c) 2014 - 2017, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <string>

#include "base/change_type.hpp"
#include "base/xml_dom_util.hpp"

namespace prot {

const ChangeTypePtr ChangeType::INPUT = ChangeTypePtr(new ChangeType(1, "Input"));
const ChangeTypePtr ChangeType::FIXED = ChangeTypePtr(new ChangeType(2, "Fixed"));
const ChangeTypePtr ChangeType::PROTEIN_VARIABLE
    = ChangeTypePtr(new ChangeType(3, "Protein variable"));
const ChangeTypePtr ChangeType::VARIABLE = ChangeTypePtr(new ChangeType(4, "Variable"));
const ChangeTypePtr ChangeType::UNEXPECTED = ChangeTypePtr(new ChangeType(5, "Unexpected"));

void ChangeType::appendXml(XmlDOMDocument* xml_doc, xercesc::DOMElement* parent) {
  std::string element_name = ChangeType::getXmlElementName();
  xercesc::DOMElement* element = xml_doc->createElement(element_name.c_str());
  xml_doc->addElement(element, "name", name_.c_str());
  parent->appendChild(element);
}

ChangeTypePtr ChangeType::getChangeTypePtrFromXml(xercesc::DOMElement * element) {
  std::string name = XmlDomUtil::getChildValue(element, "name", 0);
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

}  // namespace prot
