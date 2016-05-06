#include"rubik.h"

int Rubik_BF::checkConditions(const Topology::t_state* Foresight, const Topology::t_state* Trail)
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
      return useCache(Foresight, Trail);
    }
  }
  cluster.found=nullptr;
  return 1;
}

int Rubik_BF::useCache(const Topology::t_state* Foresight, const Topology::t_state* Trail)
{
  int index=cluster.indexOf(Trail->state, Foresight->state,InitialState); 
  cluster.found = cluster.solutions(index);
  for(int depth=0;depth<cluster.dimensions(index);++depth,++cluster.found)
  {
    const CubeSlot *c =SolvedState+cluster.Dim; 
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
      OUT_(NL<<(*cluster.found)->path())
      return 1;
    }
  }
  cluster.found=nullptr; 
  return 0;
}
