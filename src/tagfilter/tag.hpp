
#ifndef PROT_TAG_HPP
#define PROT_TAG_HPP

#include <string>

#include "base/logger.hpp"

namespace prot {

class SpecTag {
 public:
  SpecTag (const std::string & x, const std::string & y,
       double mass, bool ordered): acidX(x), acidY(y), 
    mass(mass), tolerance(200), ordered(ordered){}

  double getMinMass() {return mass - tolerance;}

  double getMaxMass() {return mass + tolerance;}

  bool isOrdered() {return ordered;}

  std::string getSeq() {return acidX + acidY;}

 private:
  std::string acidX, acidY;
  double mass, tolerance;
  bool ordered;

};

}

#endif