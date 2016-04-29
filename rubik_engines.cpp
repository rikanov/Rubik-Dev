#include"rubik.h"

int Rubik_BF::checkConditions(const Topology::t_state * Foresight, const Topology::t_state * Trail)
{
  foundBetter=false;
  int Result=0;
  int cond=0;
  int counter=0;
  bool found=false;
  for(const CubeSlot *c=SolvedState; *c!=-128; ++c)
  {
    if(*c<0) 
    {
      ++Result;
      if(cond>=-(*c))
      {
	found=true; 
	break;
      }
      cond=0;
    }
    else
    {
      const int add=Trail->state[Foresight->state[InitialState[*c]]]==*c; 
      cond+=add;
      counter+=add;
    }
  }
  if(counter>best_choice)
  {
    best_choice=counter; 
    foundBetter=true;
  }
  return found ? Result : 0;
}

int Rubik_BF::fastestCheck(const Topology::t_state* Foresight, const Topology::t_state* Trail)
{
  for(const CubeSlot *c=SolvedState; *c>=0; ++c)
  {
    if(Trail->state[Foresight->state[InitialState[*c]]]!=*c)
    {
      return 0;
    }
  }
  return 1;
}

int Rubik_BF::heuristicalSearch(const Topology::t_state* Foresight, const Topology::t_state* Trail)
{
  for(const CubeSlot *c=SolvedState; *c>=0; ++c)
  {
    if(Trail->state[Foresight->state[InitialState[*c]]]!=*c)
    {
      int multiplier=1;
      int index=0; 
      for(int i=0; i<cluster.Dim; ++i, multiplier*=24)
      {
	//const Sidemarks sm(InvInitialState[Foresight->i_state[Trail->i_state[cluster.HeuristicIndices[i]]]]);
	const Sidemarks sm(Trail->state[Foresight->state[InitialState[cluster.HeuristicIndices[i]]]]);
	index+=multiplier*sm.getPivot();
      }
      cluster.found = cluster.solutions(index);
      for(int depth=0;depth<cluster.dimensions(index);++depth,++cluster.found)
      {
	const CubeSlot *c=SolvedState+cluster.Dim; 
	while(*c>=0)
	{
	  if((*cluster.found)->state[Trail->state[Foresight->state[InitialState[*c]]]]!=*c)
	  {
	    break;
	  }
	  ++c;
	}
	if(*c<0)
	{
	  return 1;
	}
      }
      cluster.found=nullptr; 
      return 0;
    }
  } OUT_("found ")
  cluster.found=nullptr;
  return 1;
}