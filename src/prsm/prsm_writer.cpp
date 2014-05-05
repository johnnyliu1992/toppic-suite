/*
 * prsm_writer.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: xunlikun
 */
#include "base/logger.hpp"
#include "prsm/prsm_writer.hpp"

namespace prot {

PrsmWriter::PrsmWriter(std::string file_name) {
  file_.open(file_name.c_str());
  LOG_DEBUG("file_name " << file_name);
  file_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  file_ << "<prsm_list>";
  XmlDOMImpl* impl = XmlDOMImplFactory::getXmlDOMImplInstance();
  doc_ = new XmlDOMDocument(impl->createDoc("prsm_list"));
  serializer_ = impl->createSerializer();
}

void PrsmWriter::close(){
  file_ << "</prsm_list>" << std::endl;
  file_.close();
}

PrsmWriter::~PrsmWriter() {
//  file_ << "</prsm_list>" << std::endl;
//  file_.close();
  serializer_->release();
  delete doc_;
}

void PrsmWriter::write(PrsmPtr prsm_ptr) {
  if(prsm_ptr!=nullptr){
    xercesc::DOMElement* element = prsm_ptr->toXmlElement(doc_);
    //LOG_DEBUG("Element generated");
    std::string str = writeToString(serializer_, element);
    //LOG_DEBUG("String generated");
    writeToStreamByRemovingDoubleLF(file_, str);
    element->release();
  }
}

void PrsmWriter::writeVector(PrsmPtrVec &prsms) {
  for (unsigned i = 0; i < prsms.size(); i++) {
    write(prsms[i]);
  }
}

void PrsmWriter::writeVector2D(PrsmPtrVec2D &prsms){
    for (unsigned i = 0; i < prsms.size(); i++) {
        writeVector(prsms[i]);
    }
}
void PrsmWriter::writeVector3D(PrsmPtrVec3D &prsms){
    for (unsigned i = 0; i < prsms.size(); i++) {
        writeVector2D(prsms[i]);
    }
}

} /* namespace prot */
