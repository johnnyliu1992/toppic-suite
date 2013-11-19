#include "acid_vec.hpp"
#include "xml_dom.hpp"
#include "xml_dom_document.hpp"

namespace proteomics {
      

const std::vector<AcidPtr> AcidVec::getInstance(const char* file_name) {
  std::vector<AcidPtr> acid_ptrs;
  proteomics::XmlDOMParser* parser = proteomics::getXmlDOMInstance();
  if (parser) {
    proteomics::XmlDOMDocument* doc = new proteomics::XmlDOMDocument(parser, file_name);
    if (doc) {
      int acid_num = doc->getChildCount("amino_acid_list", 0, "amino_acid");
      for (int i = 0; i < acid_num; i++) {
        xercesc::DOMElement* element = doc->getElement("amino_acid", i);
        acid_ptrs.push_back(AcidPtr(new Acid(element)));

      }
      delete doc;
    }
    delete parser;
  }
  return acid_ptrs;
}

/**
 * Returns an amino acid based on the the name. Returns null if the amino
 * acid name does not exist.
 */
AcidPtr AcidVec::getAcidByName(std::vector<AcidPtr> &acid_ptrs, const std::string &name) {
    for (unsigned int i = 0; i < acid_ptrs.size(); i++) {
        std::string n = acid_ptrs[i]->getName();
        if (n == name) {
            return acid_ptrs[i];
        }
    }
    return AcidPtr(nullptr);
}

/**
 * Returns an amino acid based on the one letter representation. Returns
 * null if the one letter representation does not exist.
 */
AcidPtr AcidVec::getAcidByOneLetter(std::vector<AcidPtr> &acid_ptrs, const std::string &one_letter) {
    for (unsigned int i = 0; i < acid_ptrs.size(); i++) {
        std::string l = acid_ptrs[i]->getOneLetter();
        if (l == one_letter) {
            return acid_ptrs[i];
        }
    }
    //logger.debug("Acid not found " + one_letter);
    return AcidPtr(nullptr);
}

/**
 * Returns an amino acid based on the three letter representation. Returns
 * null if the three letter representation does not exist.
 */
AcidPtr AcidVec::getAcidByThreeLetter(std::vector<AcidPtr> &acid_ptrs, const std::string &three_letter) {
    for (unsigned int i = 0; i < acid_ptrs.size(); i++) {
        std::string l = acid_ptrs[i]->getThreeLetter();
        if (l == three_letter) {
            return acid_ptrs[i];
        }
    }
    //logger.debug("Acid not found " + three_letter);
    return AcidPtr(nullptr);
}

/**
 * Checks if the list contains an amino acid with the specific name.
 */
bool AcidVec::containsName(std::vector<AcidPtr> &acid_ptrs, const std::string &name) {
    if (getAcidByName(acid_ptrs, name).get() == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

/**
 * Checks if the list contains an amino acid with the specific one letter
 * representation.
 */
bool AcidVec::containsOneLetter(std::vector<AcidPtr> &acid_ptrs, const std::string &one_letter) {
    if (getAcidByOneLetter(acid_ptrs, one_letter).get() == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

/**
 * Checks if the list contains an amino acid with the specific three letter
 * representation.
 */
bool AcidVec::containsThreeLetter(std::vector<AcidPtr> &acid_vec, const std::string &three_letter) {
    if (getAcidByThreeLetter(acid_vec, three_letter).get() == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

/**
 * Converts a protein sequence (with one letter representation of amino
 * acids) to an amino acid array.
 */
std::vector<AcidPtr> AcidVec::convert(std::vector<AcidPtr> &acid_ptrs, const std::string &seq) {
    std::vector<AcidPtr> acid_ptr_seq;
    if (seq.length() == 0) {
        return acid_ptr_seq;
    } else {
        for (unsigned int i = 0; i < seq.length(); i++) {
            acid_ptr_seq.push_back(AcidVec::getAcidByOneLetter(acid_ptrs, seq.substr(i, 1)));
        }
        return acid_ptr_seq;
    }
}
	
};


