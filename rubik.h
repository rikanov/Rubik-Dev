#ifndef RUBIK_CLASS
#define RUBIK_CLASS

#include"auxiliary.h"
#include<map>

#define UI_func(X) String X(Stream& IS);
#define UI_rfunc(X) String Rubik::X(Stream& IS)
#define GET(A) String A; IS>>A;
#define GET2(A,B) String A,B; IS>>A; IS>>B; 
#define PARSER(A) String A=parser(IS);
#define PARSER2(A,B) PARSER(A) PARSER(B)

using namespace auxiliary;

class Rubik 
{
		// Let I the index of a certain side mark in the current state 
		// of the Rubik's Cube. Then, let:
  int * A_map; 	// indices of side marks what we need here, 
  int * B_map;	// indices of side marks what ARE actually here

  int * Sup_map;  // to find partial solutions we need supposations
  int * Sup_inv;  // by default they both are identical mappings. (no supposation)  

  static const int MaximumStackDepth=256;
  int Stack[MaximumStackDepth][NumberOfSideMarks];
  int stack_pointer=0;

  std::map<String, String> Var_space;
  
  bool is_solved(const int* Cubes, const int& Limit) const;
  void setConditions(int* SolutionIdices, int* SolvedState, int* InitialState, const Line& Solutions, std::list< t_state >& Seeking) const;
  int checkConditions(const int *State, const int * SolvedState, const int * Conditions) const;
  std::pair<int,String> seeker(std::list< t_state >& Trace, const int* SolvedState, const int* Conditions, const int* AllowedSides) const;
  bool variableEquality(String& A, const String& B) const;
  void variable(Stream& IS, String& R);
  String functionResolver(Stream& IS, const String& R);
  void select(Stream& IS, String& Result, const bool & Inv);
  
     //==========================//
    //  *** User Interface ***  //
   //==========================//
  UI_func(parser)
  
  UI_func(progn)
  UI_func(list)
  UI_func(defvar)
  UI_func(defun)
  UI_func(nilEquality)
  UI_func(variableEquality)
  UI_func(parsingEquality)
  UI_func(logicalOr)
  UI_func(logicalXor)
  UI_func(logicalAnd)
  
  UI_func(assoc)
  UI_func(select)
  UI_func(deselect)
  UI_func(mapcar)
  UI_func(conc)
  UI_func(regExp)
  UI_func(echo)
    
  UI_func(merge)
  UI_func(pathFinder)
  UI_func(callBruteForce)
  
  UI_func(whatIs)
  UI_func(whereIs)
  UI_func(cube)
  UI_func(solvedp)
  UI_func(doRotations)
  UI_func(store)
  UI_func(revert)
  UI_func(setAlign)
  UI_func(printSmarks)
  
public:
  
  Rubik();
  Rubik(Rubik * R);
  ~Rubik();
  
  static String findPath(const Sidemarks& From, const Sidemarks& To);
  
  void noSuppose();
  void suppose(const Sidemarks & S);
  void align(const Sidemarks & A, const Sidemarks & B);
  Sidemarks whatIs(const Sidemarks & S) const;
  Sidemarks locationOf(const Sidemarks& S) const;
  
  void print(const String & C ) const;

  String bruteForce(const Line& Solutions, const std::string& AS) const;
  Rubik & operator << (const String & Rot);
  
  void REPL(std::istream & IS, std::ostream & OS);
  String file_open(Stream& IS);
  String file_open(const char * F);
};
#endif