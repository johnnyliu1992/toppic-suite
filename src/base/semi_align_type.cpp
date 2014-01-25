#include "base/semi_align_type.hpp"

namespace prot {

SemiAlignTypePtr SemiAlignTypeFactory::semi_align_type_complete_ 
    = SemiAlignTypePtr(new SemiAlignType("COMPLETE", 0));

SemiAlignTypePtr SemiAlignTypeFactory::semi_align_type_prefix_
    = SemiAlignTypePtr(new SemiAlignType("PREFIX", 1));
SemiAlignTypePtr SemiAlignTypeFactory::semi_align_type_suffix_  
    = SemiAlignTypePtr(new SemiAlignType("SUFFIX", 2));
SemiAlignTypePtr SemiAlignTypeFactory::semi_align_type_internal_
    = SemiAlignTypePtr(new SemiAlignType("INTERNAL", 3));

std::string SemiAlignTypeFactory::convertSemiAlignTypeToString (int i) {
  if (i==1){
    return "PREFIX";
  }
  if (i==2){
    return "SUFFIX";
  }
  if(i==3){
    return "INTERNAL";
  }
  return "COMPLETE";
}

}