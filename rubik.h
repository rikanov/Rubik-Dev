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
  int * C_map; 	// indeces of those only side marks we want to care of.
		// By default, it is an identical mapping what we can
		// restrict for some reason.

public:
  
  Rubik();
  ~Rubik();
  
  Sidemarks whatIs(const Sidemarks & S) const;
  Sidemarks locationOf(const Sidemarks& S) const;

  Rubik & operator << (const std::string & Rot);
};
#endif