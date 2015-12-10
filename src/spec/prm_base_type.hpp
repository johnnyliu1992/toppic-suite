#ifndef PROT_BASE_PRM_BASE_TYPE_HPP_
#define PROT_BASE_PRM_BASE_TYPE_HPP_

#include <memory>
#include <string>

namespace prot {

class PrmBaseType;
typedef std::shared_ptr<PrmBaseType> PrmBaseTypePtr;

class PrmBaseType {
 public:
  static const PrmBaseTypePtr ORIGINAL;
  static const PrmBaseTypePtr REVERSED;

  PrmBaseType(std::string name) {name_=name;}

  std::string getName() {return name_;}

 private:
  std::string name_;
};


}

#endif

