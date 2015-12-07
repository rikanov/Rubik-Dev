#ifndef AUXILIARY_CLASSES
#define AUXILIARY_CLASSES

#include"cube_topology.h"
#include<list>

namespace auxiliary 
{
  class Sidemarks: public std::string
  {
    static const char PositiveGroup[][4];
    int Index;
    mutable int eigenvalue;
    
  public:
    Sidemarks()=default;
    Sidemarks(const int & In); 
    Sidemarks(const std::string & S);
    Sidemarks(const char * C);
    Sidemarks & operator = (const std::string & S);
    operator int() const {return Index;}
    const int& setEigenvalue();
    const int & getEigenvalue() const {return eigenvalue;}
    
  friend std::ostream & operator<<(std::ostream & os, const Sidemarks & sm);
  };
  //============================================================================
  
  class Line: public std::list<String>
  {
    static const std::string DefaultSeparators;
    Line & fill_up(const std::string & Read, const String & Separator);
    
  public:
    Line()=default;
    Line(const std::string & Read, const String & Separator=DefaultSeparators);
    
  friend std::ostream & operator<<(std::ostream & os, const Line & line);
  friend std::istream & operator>>(std::istream & is, Line & line);
  };
  std::istream & operator>>(std::istream & is, Line & line);
  
  //============================================================================
  
  struct t_state
  {
    static int order;
    int * State;
    int LastSide;
    String Path;
    
    t_state(const t_state& T);
    t_state(const int * S, const int & L, const String & P);
    ~t_state();
    t_state& operator= (const t_state& T);
  };
  
}
#endif
