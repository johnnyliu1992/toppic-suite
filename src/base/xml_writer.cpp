/*
 * xml_writer.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: xunlikun
 */

#include "base/xml_writer.hpp"

namespace prot {
XmlWriter::XmlWriter(std::string file_name,std::string root){
  file_.open(file_name.c_str());
  root_ = root;
  LOG_DEBUG("file_name " << file_name);
  file_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  if(root_.compare("")!=0){
  file_ << ("<"+root_+">");
  }
  XmlDOMImpl* impl = XmlDOMImplFactory::getXmlDOMImplInstance();
  doc_ = new XmlDOMDocument(impl->createDoc(root_.compare("")!=0?root_:"ROOT"));
  serializer_ = impl->createSerializer();
}
XmlWriter::~XmlWriter(){
  if(root_.compare("")!=0){
  file_ << "</"+root_+">" << std::endl;
  }
  file_.close();
  serializer_->release();
  delete doc_;
}
void XmlWriter::write(xercesc::DOMElement* element){
  std::string str = writeToString(serializer_, element);
  writeToStreamByRemovingDoubleLF(file_, str);
  element->release();
}
} /* namespace prot */