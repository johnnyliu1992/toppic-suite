#include "base/algorithm.hpp"
#include "spec/theo_peak.hpp"
#include "prsm/peak_ion_pair.hpp"

namespace prot {

void PeakIonPair::appendPeakToXml(XmlDOMDocument* xml_doc, 
                                  xercesc::DOMElement* parent) {

  xercesc::DOMElement* element = xml_doc->createElement("matched_peak");
  std::string str = theo_peak_ptr_->getIonPtr()->getIonTypePtr()->getName();
  xml_doc->addElement(element, "ion_name", str.c_str());
  str = convertToString(real_peak_ptr_->getBasePeakPtr()->getId());
  xml_doc->addElement(element, "peak_id", str.c_str());
  str = convertToString(real_peak_ptr_->getBasePeakPtr()->getCharge());
  xml_doc->addElement(element, "peak_charge", str.c_str());
  parent->appendChild(element);
}

void PeakIonPair::appendIonToXml(XmlDOMDocument* xml_doc, 
                                 xercesc::DOMElement* parent) {
  xercesc::DOMElement* element = xml_doc->createElement("matched_ion");
  std::string str = theo_peak_ptr_->getIonPtr()->getIonTypePtr()->getName().substr(0,1);
  xml_doc->addElement(element, "type", str.c_str());
  str = convertToString(theo_peak_ptr_->getShift());
  xml_doc->addElement(element, "match_shift", str.c_str()); 
  str = convertToString(real_peak_ptr_->getMonoMass());
  xml_doc->addElement(element, "adjusted_mass", str.c_str()); 
  str = convertToString(theo_peak_ptr_->getIonPtr()->getDisplayPos());
  xml_doc->addElement(element, "ion_display_posistion", str.c_str()); 
  str = convertToString(theo_peak_ptr_->getIonPtr()->getPos());
  xml_doc->addElement(element, "ion_left_posistion", str.c_str()); 
  double error = real_peak_ptr_->getMonoMass() - theo_peak_ptr_->getModMass();
  str = convertToString(error);
  xml_doc->addElement(element, "mass_error", str.c_str()); 
  str = convertToString(error * 1000000 / real_peak_ptr_->getMonoMass());
  xml_doc->addElement(element, "ppm", str.c_str()); 
  parent->appendChild(element);
}

void getMatchedPairs(PeakIonPairPtrVec &pairs, int peak_id, 
                     PeakIonPairPtrVec &selected_pairs) {
  for (unsigned int i = 0; i < pairs.size(); i++) {
    if (pairs[i]->getRealPeakPtr()->getBasePeakPtr()->getId() == peak_id) {
      selected_pairs.push_back(pairs[i]);
    }
  }
}

void findPairs(ExtendMsPtr ms_three_ptr, TheoPeakPtrVec &theo_peaks, 
               int bgn, int end, PeakIonPairPtrVec &pairs) {
  std::sort(theo_peaks.begin(), theo_peaks.end(), theo_peak_up);
  std::vector<double> ms_masses;
  getExtendMassVec(ms_three_ptr, ms_masses);
  std::vector<double> theo_masses; 
  getTheoMassVec(theo_peaks, theo_masses);

  unsigned int i = 0;
  unsigned int j = 0;
  while (i < ms_masses.size() && j < theo_masses.size()) {
    double deviation = ms_masses[i] - theo_masses[j];
    IonPtr ion_ptr = theo_peaks[j]->getIonPtr();
    double err = ms_three_ptr->getPeakPtr(i)->getOrigTolerance();
    if (ion_ptr->getPos() >= bgn && ion_ptr->getPos() <= end) {
      if (abs(deviation) <= err) {
        PeakIonPairPtr pair_ptr 
            = PeakIonPairPtr(new PeakIonPair(ms_three_ptr->getPeakPtr(i), theo_peaks[j]));
      }
    }
    if (increaseIJ(i, j, deviation, err, ms_masses, theo_masses)) {
      i++;
    } else {
      j++;
    }
  }
}

void getPeakIonPairs (ProteoformPtr proteoform_ptr, ExtendMsPtr ms_three_ptr, 
                      double min_mass, PeakIonPairPtrVec &pairs) {
  ActivationPtr activation_ptr = ms_three_ptr->getHeaderPtr()->getActivationPtr();

  TheoPeakPtrVec theo_peaks = getProteoformTheoPeak(proteoform_ptr, 
                                                    activation_ptr, 
                                                    min_mass);

  findPairs(ms_three_ptr, theo_peaks, 0, proteoform_ptr->getLen(), pairs);
}

}
