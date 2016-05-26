#ifndef GLOBALS_MACROS__
#define GLOBALS_MACROS__

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <regex>
#include <map>
#include <time.h>
#include <algorithm> 
#include <ncurses.h>

const int CURRENT_VERSION_NUMBERS[]={1,0,0};

typedef std::string String;
typedef std::stringstream Stream;
typedef std::regex Regex;
typedef signed char CubeSlot;
#define REPLACE(X,R,A) std::regex_replace(X,R,A);
#define FIND(X,R,n) std::regex_replace(X,R,n,std::regex_constants::format_no_copy)
#define MATCH(X,R) std::regex_match(X,R)
#define STR_END std::string::npos
#define FOR_STR(S,it) for(std::string::iterator it=S.begin();it!=S.end();++it)
#define C_FOR_STR(S,it) for(std::string::const_iterator it=S.begin();it!=S.end();++it)

#define EMPTY(x) CubeSlot x[NumberOfSideMarks]={}; 
#define CPY_FUNC(x,y) memcpy(x,y, NumberOfSideMarks); // DON'T use sizeof y or x !!!!!!!!
#define FOR_FUNC(x) for(int x=0;x<NumberOfSideMarks;++x)
#define EACH_FUNC(x,y,i) for(CubeSlot * y=x,i=0;i<NumberOfSideMarks;++y,++i)
#define PRINT_FUNC(x) for(CubeSlot * y=x,i=0;i<NumberOfSideMarks;++y,++i) std::cout<<i<<':'<<Sidemarks(i)<<"->"<<Sidemarks(*y)<<' ';
#define C_EACH_FUNC(x,y,i) int i=0;for(const CubeSlot * y=x;i<NumberOfSideMarks;++y,++i)
#define OUT_(x) std::cout<<x<<std::endl;
#define OUT(x) std::cout<<x<<std::flush;
#define NL std::endl
#define NL_ std::cout<<std::endl;

extern const char * NIL;
extern const char * L_TRUE;
extern const char * MACRO_SYNTAX;
const int CompressedArraySize=240;
const int NumberOfSideMarks=78;	// = 8*3! + 12*2! 
const CubeSlot IdentityMap[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,\
			40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77};

void inline TRIM_END(String & S)
{
  while(!S.empty() && (S.back()==';' || S.back()==' ' || S.back()=='\t'))
  {
    S.pop_back();
  }
}

void inline TRIM(String & S)
{
  TRIM_END(S);
  while(!S.empty() && (S.front()==' ' || S.front()=='\t')) 
  {
    S=S.substr(1);
  }
}

void inline NONILL(String & X)
{
  if(X=="NIL") 
  {
    X="";
    
  }
}

bool inline isletter(const char& x)
{
  return ('A'<=((x)&95) && ((x)&95)<='Z');
}

int inline OPPOSITE(const int& x) 
{
  return (5-x);
}

bool inline IS_FACET(const char& X)
{
  return (X=='F'||X=='U'||X=='R'||X=='B'||X=='D'||X=='L');
}

char inline UPCASE(const char & x)
{
  return x & 95;
}

bool inline IS_LOWCASE(const char & x)
{
  return (x&32);
}

#endif