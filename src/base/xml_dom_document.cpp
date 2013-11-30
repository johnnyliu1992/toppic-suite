#include <stdlib.h>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "xml_dom_document.hpp"

 
namespace prot {

XmlDOMDocument::XmlDOMDocument(XmlDOMParser* parser, 
                               const char* xml_file) : doc_(NULL) {
  doc_ = parser->parse(xml_file);
}

XmlDOMDocument::~XmlDOMDocument() {
  if (doc_) {
    doc_->release();
  }
}

xercesc::DOMElement* XmlDOMDocument::getElement(const char* tag, int index) { 
  XMLCh* temp = xercesc::XMLString::transcode(tag);
  xercesc::DOMNodeList* list = doc_->getElementsByTagName(temp);
  xercesc::XMLString::release(&temp);
  xercesc::DOMElement* element = 
      dynamic_cast<xercesc::DOMElement*>(list->item(index));
  return element;

}

std::string XmlDOMDocument::getChildValue(const char* parent_tag, 
                                          int parent_index, 
                                          const char* child_tag) {

  XMLCh* temp = xercesc::XMLString::transcode(parent_tag);
  xercesc::DOMNodeList* list = doc_->getElementsByTagName(temp);
  xercesc::XMLString::release(&temp);

  xercesc::DOMElement* parent = 
      dynamic_cast<xercesc::DOMElement*>(list->item(parent_index));
  xercesc::DOMElement* child = 
      dynamic_cast<xercesc::DOMElement*>(parent->getElementsByTagName(
              xercesc::XMLString::transcode(child_tag))->item(0));
  std::string value;
  if (child) {
    char* temp2 = xercesc::XMLString::transcode(child->getTextContent());
    value = temp2;
    xercesc::XMLString::release(&temp2);
  }
  else {
    value = "";
  }
  return value;
}

std::string XmlDOMDocument::getAttributeValue(const char* element_tag, 
                                              int element_index, 
                                              const char* attribute_tag) {
  XMLCh* temp = xercesc::XMLString::transcode(element_tag);
  xercesc::DOMNodeList* list = doc_->getElementsByTagName(temp);
  xercesc::XMLString::release(&temp);

  xercesc::DOMElement* element = 
      dynamic_cast<xercesc::DOMElement*>(list->item(element_index));
  temp = xercesc::XMLString::transcode(attribute_tag);
  char* temp2 = xercesc::XMLString::transcode(element->getAttribute(temp));

  std::string value = temp2;
  xercesc::XMLString::release(&temp);
  xercesc::XMLString::release(&temp2);
  return value;
}

int XmlDOMDocument::getChildCount(const char* parent_tag, 
                                  int parent_index, 
                                  const char* child_tag) {
  XMLCh* temp = xercesc::XMLString::transcode(parent_tag);
  xercesc::DOMNodeList* list = doc_->getElementsByTagName(temp);
  xercesc::XMLString::release(&temp);

  xercesc::DOMElement* parent = dynamic_cast<xercesc::DOMElement*>(list->item(parent_index));
  xercesc::DOMNodeList* childList = parent->getElementsByTagName(xercesc::XMLString::transcode(child_tag));
  return (int)childList->getLength();
}

/**
 * Add an element 
 **/
xercesc::DOMElement* XmlDOMDocument::createElement(const char* tag) {
  XMLCh* temp = xercesc::XMLString::transcode(tag);
  xercesc::DOMElement* element = doc_->createElement(temp);
  xercesc::XMLString::release(&temp);
  return element;
}

xercesc::DOMText* XmlDOMDocument::createTextNode(const char* text) {
  XMLCh* temp = xercesc::XMLString::transcode(text);
  xercesc::DOMText* text_node = doc_->createTextNode(temp);
  xercesc::XMLString::release(&temp);
  return text_node;
}

void XmlDOMDocument::addElement(xercesc::DOMElement* element, 
                const char* tag, const char* value) {
  xercesc::DOMElement* child = createElement(tag);
  element->appendChild(child);
  xercesc::DOMText* text_node = createTextNode(value);
  child->appendChild(text_node);
}

std::string getChildValue(xercesc::DOMElement* parent,  
                          const char* child_tag) {
  xercesc::DOMElement* child = 
      dynamic_cast<xercesc::DOMElement*>(parent->getElementsByTagName(
              xercesc::XMLString::transcode(child_tag))->item(0));
  std::string value;
  if (child) {
    char* temp2 = xercesc::XMLString::transcode(child->getTextContent());
    value = temp2;
    xercesc::XMLString::release(&temp2);
  }
  else {
    value = "";
  }
  return value;
}

double getDoubleChildValue(xercesc::DOMElement* parent,  
                           const char* child_tag) {
  std::string value = getChildValue(parent, child_tag);
  return atof(value.c_str());
}

int getIntChildValue(xercesc::DOMElement* parent,  
                     const char* child_tag) {
  std::string value = getChildValue(parent, child_tag);
  return atoi(value.c_str());
}

bool getBoolChildValue(xercesc::DOMElement* parent,
                       const char* child_tag) {
  std::string value = getChildValue(parent, child_tag);
  boost::to_lower(value);
  if(value == "true") {
    return true;
  }
  return false;
}

std::string convertToString(double value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}

std::string convertToString(bool value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}

}
