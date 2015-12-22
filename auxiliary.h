#ifndef AUXILIARY_CLASSES
#define AUXILIARY_CLASSES

#include"cube_topology.h"
#include<list>

namespace auxiliary 
{
  class Sidemarks: public std::string
  {
    int Index;
    int eigenvalue;
    
  public:
    Sidemarks()=default;
    Sidemarks(const int & In); 
    Sidemarks(const std::string & S);
    Sidemarks(const char * C);
    Sidemarks & operator = (const Sidemarks & S)=default;
    Sidemarks & operator = (const std::string & S);
    Sidemarks & operator = (const int & In);
    Sidemarks operator + (const String & S) const;
    Sidemarks & operator << (const String & S);
    bool valid() const;
    int type() const;
    operator int() const {return Index;}
    const int & getEigenvalue() const {return eigenvalue;}
 };
 std::ostream & operator<<(std::ostream & os, const Sidemarks & sm);
 std::istream & operator>>(std::istream& is, auxiliary::Sidemarks& sm);
  //============================================================================
  
  class Line: public std::list<String>
  {
    static const std::string DefaultSeparators;
    Line & fill_up(const std::string & Read, const String & Separator);
    
  public:
    Line()=default;
    Line(std::istream& IS);
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
  
  bool checkSimplePath(const String & A);
  String mergeSimplePaths(const std::string& wA, const std::string& wB);
  int * nestedLoop(int * Array,int depth, const int & UpperBound, const int & LowerBound=0);
  void imbueStream(Stream& IS, const String & V);
  String& regSimplifier(const String& R, String& S);
  bool regExp(const char *R, const char *C);
  String putInString(const String& arg, const char& sig,const String& text);
  String putInString(const String& arg1, const char& sig1, const String& arg2, const char& sig2, const String& text);

}

#endif
