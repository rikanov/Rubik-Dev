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

  bool is_solved(const int* Cubes, const int& Limit) const;
  int checkConditions(const int *State, const int * SolvedState, const int * Conditions) const;
  std::pair<int,String> seeker(std::list< t_state >& Trace, const int* SolvedState, const int* Conditions, const int* AllowedSides) const;
public:
  
  Rubik();
  Rubik(Rubik * R);
  ~Rubik();
  
  Sidemarks whatIs(const Sidemarks & S) const;
  Sidemarks locationOf(const Sidemarks& S) const;
  
  int parser(const String & In) const;
  void print(const String & C ) const;

  String bruteForce(const std::string& S, const std::string& AS="*");
  
  Rubik & operator << (const String & Rot);
  
  void REPL();
};
#endif