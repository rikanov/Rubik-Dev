#ifndef AUXILIARY_CLASSES
#define AUXILIARY_CLASSES

#include"cube_topology.h"
#include<list>

namespace auxiliary 
{
  struct CubeletColors
  {
    int
    front,
    up,
    right,
    left,
    down,
    back;
    
    CubeletColors(): front(0),up(0),right(0),left(0),down(0),back(0) {}
  };
  
  class Sidemarks: public String
  {
    int Index;
    int eigenvalue;
    int pivotNumber;
    
  public:
    Sidemarks()=default;
    Sidemarks(const int & In); 
    Sidemarks(const String & S);
    Sidemarks(const char * C);
    Sidemarks & operator = (const Sidemarks & S)=default;
    Sidemarks & operator = (const String & S);
    Sidemarks & operator = (const int & In);
    Sidemarks operator + (const String & S) const;
    Sidemarks & operator << (const String & S);
    bool valid() const;
    int type() const;
    operator int() const {return Index;}
    const int & getEigenvalue() const {return eigenvalue;}
    const int & getPivot() const {return pivotNumber;}
    bool is_acting(const char& C, const bool& middle) const;
    static bool sameCubes(const auxiliary::Sidemarks& A, const auxiliary::Sidemarks& B);
    CubeletColors getColors(const auxiliary::Sidemarks& Cubelet) const;
 };
 std::ostream & operator<<(std::ostream & os, const Sidemarks & sm);
 std::istream & operator>>(std::istream& is, auxiliary::Sidemarks& sm);
  //============================================================================
  
  bool checkSimplePath(const String & A);
  String mergeSimplePaths(const String& wA, const String& wB);
  String findPath(const Sidemarks& From, const Sidemarks& To, const bool& AllowMiddle);
  int * nestedLoop(int * Array,int depth, const int & UpperBound, const int & LowerBound=0);
  void imbueStream(Stream& IS, const String & V);
  String& regSimplifier(const String& R, String& S);
  bool regExp(const char *R, const char *C);
  String putInString(const String& arg, const char& sig,const String& text);
  String putInString(const String& arg1, const char& sig1, const String& arg2, const char& sig2, const String& text);
  String putInString(const String& arg, const String& token, const String& text);
  unsigned int countWords(Stream& IS);
  unsigned int countWords(const String& S);
  String permute(String P);
  void cryptText(const String& Key1, const String& Key2, String & Text);
  String mirror(const String & a, const char & c);
  void drawBarLine(const int & bar, const int & barLength);
  void intro();
}

#endif
