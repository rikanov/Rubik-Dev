#ifndef RUBIK_CLASS
#define RUBIK_CLASS

#include"auxiliary.h"
#include<map>
  using namespace auxiliary;

class Rubik 
{
		// Let I the index of a certain side mark in the current state 
		// of the Rubik's Cube. Then, let:
  int * A_map; 	// indices of side marks what we need here, 
  int * B_map;	// indices of side marks what ARE actually here

  int * Sup_map;  // to find partial solutions we need supposations
  int * Sup_inv;  // by default they both are identical mappings. (no supposation)
  
  std::map<String, String> Var_space;
  
  bool is_solved(const int* Cubes, const int& Limit) const;
  void setConditions(int* SolutionIdices, int* SolvedState, int* InitialState, const Line& Solutions, std::list< t_state >& Seeking) const;
  int checkConditions(const int *State, const int * SolvedState, const int * Conditions) const;
  std::pair<int,String> seeker(std::list< t_state >& Trace, const int* SolvedState, const int* Conditions, const int* AllowedSides) const;
  
     //==========================//
    //  *** User Interface ***  //
   //==========================//
  String parser(Stream& IS);
  String list(Stream& IS);
  String defvar(Stream& IS,const String& fName);
  String variableEquality(Stream& IS);
  String parsingEquality(Stream& IS);
  String mapcar(Stream& IS);
  String echo(Stream& IS);
  String assoc(Stream& IS);
  String select(Stream& IS, bool Inv);
  String pathFinder(Stream& IS);
  String merge(Stream& IS);
  String cube(Stream& IS);
  String variable(Stream& IS,const String& R);
  String functionResolver(Stream& IS, const String& R);
  String callBruteForce(Stream& IS);
  String setAlign(Stream& IS);
  void printSidemarks(Stream& IS);
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