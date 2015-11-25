#include"rubik.h"
#define NEW (new int[NumberOfSideMarks])

Rubik::Rubik(): A_map NEW, B_map NEW, C_map(nullptr)
{
  Topology::singleton();
  FUNCTOR(temp)
  CPY_FUNC(A_map,temp)
  CPY_FUNC(B_map,temp)
}

Sidemarks Rubik::whatIs(const Sidemarks& S) const
{
  return B_map[S];
}

Sidemarks Rubik::locationOf(const Sidemarks& S) const
{
  return A_map[S];
}

Rubik& Rubik::operator<<(const String & Rot)
{
  EMPTY(Act)
  Topology::defOperation(Act,Rot);
  Topology::actOn(A_map,Act);
  Topology::inverse(A_map,B_map); 
  Topology::defOperation(Act,Rot,Topology::AllCubes, Topology::Middle);
  Topology::actOn(B_map,Act);
  Topology::inverse(B_map,A_map);
  return *this;
}

Rubik::~Rubik()
{
  delete[] A_map, B_map, C_map;
}