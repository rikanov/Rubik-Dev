#include"rubik_bf.h"

int Rubik_BF::checkConditions(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence)
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
      const int add=second_sequence->state[first_sequence->state[InitialState[*c]]]==*c; 
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

int Rubik_BF::fastestCheck(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence)
{
  for(const CubeSlot *c=SolvedState; *c>=0; ++c)
  {
    if(second_sequence->state[first_sequence->state[InitialState[*c]]]!=*c)
    {
      return 0;
    }
  }
  return 1;
}

int Rubik_BF::fastestCheck(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence, const Topology::t_state* third_sequence)
{
  for(const CubeSlot *c=SolvedState; *c>=0; ++c)
  {
    if(third_sequence->state[second_sequence->state[first_sequence->state[InitialState[*c]]]]!=*c)
    {
      return 0;
    }
  }
  return 1;
}

int Rubik_BF::heuristicSearch(const Topology::t_state* rot_sequence, const Topology::t_state* )
{
  for(const CubeSlot *c=SolvedState; *c>=0; ++c)
  {
    if(rot_sequence->state[InitialState[*c]]!=*c)
    {
      return useCache(rot_sequence);
    }
  }
  cluster.found=nullptr;
  cluster.trail=nullptr;
  return 1;
}

int Rubik_BF::useCache(const Topology::t_state* rot_sequence)
{
  int result=0;
  int index=cluster.indexOf(rot_sequence->state,InitialState); 
  cluster.found = cluster.solutions(index);
  for(int depth=0;depth<cluster.dimensions(index);++depth,++cluster.found)
  {
    const CubeSlot *c =SolvedState+cluster.Dim; 
    while(*c>=0)
    {
      if((*cluster.found)->state[rot_sequence->state[InitialState[*c]]]!=*c)
      {
	if(useExtendedPath)
	{
	  result=useExtendedCache(*cluster.found, rot_sequence);
	}
	break;
      }
      ++c;
    }
    if(*c<0 || result)
    {
      return 1;
    }
  }
  cluster.found=nullptr;
  return 0;
}

int Rubik_BF::useExtendedCache(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence)
{
  const int index=cluster.subIndexOf(first_sequence, second_sequence);
  cluster.trail = cluster.sideSolutions(index);
  for(int depth=0;depth<cluster.sideDimensions(index);++depth,++cluster.trail)
  {
    const CubeSlot *c =SolvedState+cluster.Dim+2; 
    while(*c>=0)
    {
      if((*cluster.trail)->state[first_sequence->state[second_sequence->state[InitialState[*c]]]]!=*c)
      {
	break;
      }
      ++c;
    }
    if(*c>0)
    {
      return 1;
    }
  }
  cluster.trail=nullptr;
  return 0;
}