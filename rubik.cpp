#include"rubik.h"
#include"cube_topology.h"
#define NEW (new int[NumberOfSideMarks])

Rubik::Rubik(): A_map NEW, B_map NEW, Sup_map NEW, Sup_inv NEW
{
  Topology::singleton();
  CPY_FUNC(A_map,IdentityMap)
  CPY_FUNC(B_map,IdentityMap)
  noSuppose();
}

Rubik::Rubik(Rubik* R): A_map NEW, B_map NEW, Sup_map NEW, Sup_inv NEW
{
  CPY_FUNC(A_map,R->A_map)
  CPY_FUNC(B_map,R->B_map)
  noSuppose();
}

bool Rubik::is_solved(const int* Cubes, const int & Limit) const
{ 
  const int * b=B_map;
  C_EACH_FUNC(Cubes,c, index)
  {
    if(index==Limit)
    {
      return true;
    }
    if(*c!=*(b++))
    {
      return false;
    }
  }
  return true;
}

void Rubik::noSuppose()
{
  CPY_FUNC(Sup_map,IdentityMap)
  CPY_FUNC(Sup_inv,IdentityMap)
}

void Rubik::suppose(const Sidemarks& S)
{
  noSuppose();
  String SMap(bruteForce(S,"all"));
  Topology::defOperation(Sup_inv,SMap,Topology::AllCubes,Topology::NotDefined);
  Topology::inverse(Sup_inv,Sup_map);
}


Sidemarks Rubik::whatIs(const Sidemarks& S) const
{
  return Sup_map[B_map[S]];
}

Sidemarks Rubik::locationOf(const Sidemarks& S) const
{
  return A_map[Sup_inv[S]];
}

Rubik& Rubik::operator<<(const String & Rot)
{
  EMPTY(Act)
  Topology::defOperation(Act,Rot);
  Topology::actOn(A_map,Act);
  Topology::inverse(A_map,B_map); 
  if(Topology::defOperation(Act,Rot,Topology::AllCubes, Topology::Middle))
  {
    Topology::actOn(B_map,Act);
    Topology::inverse(B_map,A_map);
  }
  return *this;
}

Rubik::~Rubik()
{
  delete[] A_map, B_map, Sup_map, Sup_inv;
}