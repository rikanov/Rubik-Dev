#ifndef GLOBALS_MACROS__
#define GLOBALS_MACROS__

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <regex>
#include <map>

//#define SILENT

#define SKIP ;
#define String std::string
#define Stream std::stringstream
#define Regex std::regex
#define REPLACE(X,R,A) std::regex_replace(X,R,A);
#define MATCH(X,R) std::regex_match(X,R)
#define STR_END std::string::npos
#define FOR_STR(S,it) for(std::string::iterator it=S.begin();it!=S.end();++it)
#define C_FOR_STR(S,it) for(std::string::const_iterator it=S.begin();it!=S.end();++it)
#define OPPOSITE(x) (5-(x))
#define isletter(x) ('A'<=((x)&95)<='Z')
#define IS_FACET(X) (X=='F'||X=='U'||X=='R'||X=='B'||X=='D'||X=='L')
#define UPCASE(x) ((x)&95)
#define IS_UPCASE(x) ((x)&32)
#define FUNCTOR(x) int x[NumberOfSideMarks]; \
		    memcpy(x,IdentityMap,sizeof IdentityMap);
#define EMPTY(x) int x[NumberOfSideMarks]; 
#define CPY_FUNC(x,y) memcpy(x,y, NumberOfSideMarks*sizeof(int)); // DON'T use sizeof y or x !!!!!!!!
#define FOR_FUNC(x) for(int x=0;x<NumberOfSideMarks;++x)
#define EACH_FUNC(x,y,i) for(int* y=x,i=0;i<NumberOfSideMarks;++y,++i)
#define PRINT_FUNC(x) for(int* y=x,i=0;i<NumberOfSideMarks;++y,++i) std::cout<<i<<':'<<*y<<' ';
#define C_EACH_FUNC(x,y,i) int i=0;for(const int* y=x;i<NumberOfSideMarks;++y,++i)
#define OUT_(x) std::cout<<x<<std::endl;
#define OUT(x) std::cout<<x;
#define NL std::endl
#define NL_ std::cout<<std::endl;
#define CUT_END(S) while(S.back()==' '||S.back()==9) S.pop_back();
#define NIL "NIL"
#define TRUE "T"
const int CompressedArraySize=240;
const int NumberOfSideMarks=78;	// = 8*3! + 12*2! 
const int IdentityMap[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,\
			40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77};

#endif