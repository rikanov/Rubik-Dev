#ifndef AUXILIARY_CLASSES
#define AUXILIARY_CLASSES

#include"cube_topology.h"

namespace auxiliary 
{
  class Sidemarks: protected std::string
  {
    int Index;
    
  public:
    Sidemarks()=default;
    Sidemarks(const int & In); 
    Sidemarks(const std::string & S);
    Sidemarks(const char * C);
    Sidemarks & operator = (const std::string & S);
    operator int() const {return Index;}
    
  friend std::ostream & operator<<(std::ostream & os, const Sidemarks & sm);
  };
  
  std::ostream & operator<<(std::ostream & os, const Sidemarks & sm);
}
#endif
