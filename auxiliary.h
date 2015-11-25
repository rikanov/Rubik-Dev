#ifndef AUXILIARY_CLASSES
#define AUXILIARY_CLASSES

#include"cube_topology.h"
#include<list>

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
  
  class Line: public std::list<std::string>
  {
    static const std::string DefaultSeparators;
    Line & fill_up(const std::string & Read, const std::string & Separator);
    
  public:
    Line()=default;
    Line(const std::string & Read, const std::string & Separator=DefaultSeparators);
    
  friend std::ostream & operator<<(std::ostream & os, const Line & line);
  friend std::istream & operator>>(std::istream & is, Line & line);
  };
  std::istream & operator>>(std::istream & is, Line & line);
  
  //===========================================================================
}
#endif
