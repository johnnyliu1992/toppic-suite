#ifndef PROT_PRSM_HPP_
#define PROT_PRSM_HPP_

#include <string>

#include "base/extreme_value.hpp"
#include "base/proteoform.hpp"
#include "base/anno_residue.hpp"
#include "spec/deconv_peak.hpp"
#include "spec/extend_peak.hpp"
#include "spec/sp_para.hpp"
#include "prsm/cleavage.hpp"

namespace prot {

class PrSM {
 public:
  PrSM(ProteoformPtr proteoform_ptr, DeconvMsPtr deconv_ms_ptr, 
       double adjusted_prec_mass, double calibration, SpParaPtr sp_para_ptr);
  PrSM(xercesc::DOMElement* element,ProteoformPtrVec proteoforms);
    double getAdjustedPrecMass() {return adjusted_prec_mass_;}

    ProteoformPtr getProteoformPtr() {return proteoform_ptr_;}

    void setProteoformPtr(ProteoformPtr proteoform) {proteoform_ptr_=proteoform;}

    double getCalibration() {return calibration_;}

    DeconvMsPtr getDeconvMsPtr() {return deconv_ms_ptr_;}

    void setDeconvMsPtr(DeconvMsPtr ms) {deconv_ms_ptr_=ms;}

    double getMatchPeakNum() {return match_peak_num_;}

    double getEValue() {return prob_ptr_->getEValue();}

    double getFdr() {return fdr_;}

    int getId() {return prsm_id_;}

    double getPValue() {return prob_ptr_->getPValue();}

    double getMatchFragNum() {return match_fragment_num_;}

    int getSpectrumId() {return spectrum_id_;}

  std::string getSpectrumScan() {return spectrum_scan_;}

    double getOriPrecMass() {return ori_prec_mass_;}

    int getPrecurorId() {return precursor_id_;}

    ExtendMsPtr getRefineMs() {return refine_ms_three_;}

    void setRefineMs(ExtendMsPtr refine_ms_three){refine_ms_three_ = refine_ms_three;}

    ExtremeValuePtr getProbPtr() {return prob_ptr_;} 

    void setProbPtr(ExtremeValuePtr prob_ptr) {prob_ptr_ = prob_ptr;}

    void setFdr(double fdr) {fdr_ = fdr;}

    void setId(int id) {prsm_id_ = id;}

    void setSpectrumId(int spectrum_id) {spectrum_id_ = spectrum_id;}

    void setSpectrumScan(std::string spectrum_scan) {spectrum_scan_ = spectrum_scan;}

    void setOriPrecMass(double prec_mass) {ori_prec_mass_ = prec_mass;}

  void setPrecurorId(int precursor_id) {precursor_id_ = precursor_id;}

  int getMinMass(){return sp_para_ptr_->getMinMass();}

  xercesc::DOMElement* toXmlElement(XmlDOMDocument* xml_doc);

  void appendXml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent);

 private:
  int prsm_id_ = -1;
  /* spectrum information */
  int spectrum_id_;

  std::string spectrum_scan_;

  int precursor_id_;

  double ori_prec_mass_;
  /* adjusted precursor mass */
  double adjusted_prec_mass_;
  /* calibration */
  double calibration_;

  /* protein sequence */
  ProteoformPtr proteoform_ptr_;

  ExtremeValuePtr prob_ptr_;

  double fdr_ = -1;

  /* The following are not saved in xml */
  DeconvMsPtr deconv_ms_ptr_;
  /* adjusted extended msThree, used for matching ions and peaks */
  ExtendMsPtr refine_ms_three_;

  double match_peak_num_ = 0;
  double match_fragment_num_ = 0;

  SpParaPtr sp_para_ptr_;

  void init(SpParaPtr sp_para_ptr);
  void initScores(SpParaPtr sp_para_ptr);
};

typedef std::shared_ptr<PrSM> PrSMPtr;
typedef std::vector<PrSMPtr> PrSMPtrVec;
typedef std::vector<PrSMPtrVec> PrSMPtrVec2D;
typedef std::vector<PrSMPtrVec2D> PrSMPtrVec3D;

inline bool prsmProteoformIdUp(PrSMPtr p1, PrSMPtr p2) {
  return p1->getProteoformPtr()->getDbResSeqPtr()->getId()
      < p2->getProteoformPtr()->getDbResSeqPtr()->getId();
}

inline bool prsmMatchFragmentDown(PrSMPtr p1, PrSMPtr p2) {
  return p1->getMatchFragNum() > p2->getMatchFragNum();
}
//compare two complete prsm, first match fragment number, second start pos 
inline bool prsmCompPreMatchFragmentDown(PrSMPtr p1, PrSMPtr p2) {
 if(p1->getMatchFragNum() > p2->getMatchFragNum()){
   return true;
 }
 else if(p1->getMatchFragNum() == p2->getMatchFragNum()){
   return p1->getProteoformPtr()->getStartPos()<p2->getProteoformPtr()->getStartPos();
 }
 return false;
}

inline bool prsmEValueUp(PrSMPtr p1, PrSMPtr p2) {
  return p1->getEValue() < p2->getEValue();
}

inline bool prsmEValueDown(PrSMPtr p1, PrSMPtr p2) {
  return p1->getEValue() > p2->getEValue();
}

inline bool prsmSpectrumIdUpPrecursorIdUp(PrSMPtr p1, PrSMPtr p2){
    if(p1->getSpectrumId() < p2->getSpectrumId()){
        return false;
    }
    else if(p1->getSpectrumId() > p2->getSpectrumId()){
        return true;
    }
    else{
        if(p1->getPrecurorId()>p2->getPrecurorId()){
            return true;
        }
        return false;
    }
}

inline bool prsmSpectrumIdUpMatchFragUp(PrSMPtr p1, PrSMPtr p2){
    if(p1->getSpectrumId() < p2->getSpectrumId()){
        return true;
    }
    else if(p1->getSpectrumId() > p2->getSpectrumId()){
        return false;
    }
    else{
        if(p1->getMatchFragNum()>p2->getMatchFragNum()){
            return true;
        }
        return false;
    }
}

PrSMPtrVec readPrsm(std::string file_name,ProteoformPtrVec proteoforms);

bool isMatch(PrSMPtr prsm_ptr, MsHeaderPtr header_ptr);
void filterPrsms(PrSMPtrVec &prsms, MsHeaderPtr header_ptr, PrSMPtrVec &sele_prsms); 
}
#endif

