#ifndef RUBIK_CLASS
#define RUBIK_CLASS

#include"auxiliary.h"
  
  using namespace auxiliary;

class Rubik 
{
		// Let I the index of a certain side mark in the current state 
		// of the Rubik's Cube. Then, let:
  int * A_map; 	// indices of side marks what we need here, 
  int * B_map;	// indices of side marks what ARE actually here

  int * Sup_map;  // to find partial solutions we need supposations
  int * Sup_inv;  // by default they both are identical mappings. (no supposation)
  
  bool is_solved(const int* Cubes, const int& Limit) const;
  void setConditions(int* SolutionIdices, int* SolvedState, int* InitialState, const Line& Solutions, std::list< t_state >& Seeking) const;
  int checkConditions(const int *State, const int * SolvedState, const int * Conditions) const;
  std::pair<int,String> seeker(std::list< t_state >& Trace, const int* SolvedState, const int* Conditions, const int* AllowedSides) const;
  String parser(std::stringstream& IS);
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
};
#endif