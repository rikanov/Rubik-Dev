#include"rubik.h"

String Rubik::random()
{
  String Result;
  Result.push_back(Topology::SideMarks[1+rand()%6]);
  if(rand()%2)
  {
    Result.push_back('\'');
  }
  return Result;
}

String Rubik::bruteForce(Stream& IS, const String & AS) const
{
  const int SizeS=auxiliary::countWords(IS);
  int SolutionIndices[SizeS+1]={};
  int SolvedState[NumberOfSideMarks]={};
  int InitialState[NumberOfSideMarks]={};
  setConditions(SolutionIndices,SolvedState,InitialState,IS);
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  int allowed[allowed_sides.length()+1];
  for(int i=0; i<allowed_sides.length();++i)
  {
    allowed[i]=Topology::sideDigit(allowed_sides[i]);
  }
  allowed[allowed_sides.length()]=-1;
  Description desc;
  desc.AllowedSides=allowed;
  desc.State=InitialState;
  desc.SolvedState=SolvedState;
  desc.SolutionIdices=SolutionIndices;
  std::pair<int,String> Result=seeker(desc);
#ifndef SILENT
  NL_
  if(Result.first)
  {  
    if(Result.second=="")
    {
      OUT_("The condition "<<Result.first<<" is already satisfied. \nNothing to do.")
    }
    else
    {
      OUT_("The "<<Result.second << " path has been found for condition "<<Result.first );
    }
  }
  else
  {
    OUT_("Not found." );
  }
#endif
  BF_SUCCESS=Result.first ? std::to_string(Result.first) : "NIL";
  BF_RESULT =Result.second;
  return Result.second;
}

void Rubik::setConditions(int * SolutionIdices, int * SolvedState, int * InitialState, Stream& IS) const
{
  int *sp=SolutionIdices,sp_end=0;
  int seq_counter=0;
  String s; 
  while(IS>>s)
  {
    if(s=="*")
    {
      if(seq_counter)
      {
	*(sp++)=-seq_counter;
	seq_counter=0;
      }
      continue;
    }
    if(isdigit(s.front()))
    {
      if(seq_counter)
      {
	*(sp++)=-stoi(s);
	seq_counter=0;
      }
      continue;
    }
    ++seq_counter;
    const int Solved=Topology::getIndex(s);
    // it works like a set. If its index is not found add the new unique member (Solved) into SolvedState
    int index=0;
    for(; index<sp_end;++index)
    {
      if(Sidemarks::sameCubes(SolvedState[index],Solved))
      {
	break;
      }
    }
    *(sp++)=index;
    if(index==sp_end)
    {
      InitialState[index]=locationOf(Solved);
      SolvedState[index]=Solved;
      ++sp_end; 
    }
  } 
  if(*(sp-1)>=0)
  {
    *(sp++)=-seq_counter; // auto-finish
  }
  *sp=-256;  // the end sign of testing
  InitialState[sp_end]=-1; // end of inner state
}

int Rubik::checkConditions(const Description & desc, int & best_choice, bool & foundBetter) const
{
  foundBetter=false;
  int Result=1;
  int cond=0;
  bool found=false;
  for(const int *c=desc.SolutionIdices; *c!=-256; ++c)
  {
    if(*c<0)
    {
      if(cond>=-(*c))
      {
	found=true;
	break;
      }
      else
      {	
	if(cond>best_choice)
	{
	  best_choice=cond;
	  foundBetter=true;
	}
	cond=0;
	++Result;
	continue;
      }
    }
    cond+=(desc.State[*c]==desc.SolvedState[*c]);
  }
  return found ? Result : 0;
}

std::pair<int,String> Rubik::seeker(const Description & desc) const			       
{ 
  int length=1;
  for(const int * I=desc.State; *I!=-1; ++I,++length);
  t_state::order=length;
  const int barLength=60;
  const int seekerStep=BF_WIDTH;
  const int seekerSize=seekerStep*barLength;
  const int state_size=t_state::order*sizeof(int);
  int nstate[t_state::order];
  t_state * ExtendedSpace=new t_state[seekerSize];
  t_state *trace_start=ExtendedSpace;
  t_state *trace_end=trace_start+1;
  t_state *Solution=nullptr;
  trace_start->state=desc.State;
  int result=0, best_choice=0;
  int trace_length=1;
  for(bool foundBetter=false; trace_length<seekerSize;++trace_start)
  {
    desc.State=trace_start->state;
    result=checkConditions(desc,best_choice, foundBetter);
    if(result)
    {
      Solution=trace_start;
      break;
    }
    else
    {
      if(foundBetter)
      {
	Solution=trace_start;
      }
      trace_length+=15;
      extendNode(trace_start,trace_end,desc.AllowedSides, nstate, state_size);
    }
  }
  t_state tempSolution;
  for(int bar=0, restLine=2*barLength/3-2; !result && trace_start!=trace_end; ++trace_start)
  {
    if((trace_length--)%seekerStep==0)
    {
      auxiliary::drawBarLine(bar++,restLine);
    }
    result=examineNode(desc,trace_start, tempSolution, best_choice, nstate, state_size);
    if(result)
    {
      break;
    }
  }
  if(tempSolution.parent)
  {
    *Solution=tempSolution;
  }
  NL_
  String Result=Solution->parent!=nullptr ? Solution->path() : NIL;
  ExtendedSpace->state=nullptr;
  delete[] ExtendedSpace;
  return std::pair<int,String> (result, Result);
}
void Rubik::extendNode(t_state *& trace_start, t_state *& trace_end, const int * AllowedSides,int * nstate, const int & state_size) const
{
  for(const int *a=AllowedSides; *a!=-1; ++a)
  {
    if(*a==trace_start->Op&7)
    {
      continue;
    }
    const int A=*a-1;
    for(int mode=0;mode<2;++mode)
    {
      Topology::operateOnRestrictedSpace(nstate,trace_start->state,A,mode==1);
      if(mode==2)
      {
	int double_move[t_state::order];
	memcpy(double_move,nstate,state_size);
	Topology::operateOnRestrictedSpace(nstate,double_move,A,false);
      }
      trace_end->parent=trace_start;
      trace_end->Op=(*a)|(mode<<3);
      trace_end->state=new int[t_state::order];
      memcpy(trace_end->state,nstate,state_size);
      ++trace_end;
    }
  }
}

int Rubik::examineNode(const Description & desc, t_state * trace_start, t_state & betterState, int & best_choice,  int * nstate, const int & state_size) const
{
  int result=0;
  bool foundBetter=false;
  for(const int *a=desc.AllowedSides; *a!=-1; ++a)
  {
    if(*a==trace_start->Op&7)
    {
      continue;
    }
    const int A=*a-1;
    for(int mode=0;mode<2;++mode)
    {
      Topology::operateOnRestrictedSpace(nstate,trace_start->state,A,mode==1);
      if(mode==2)
      {
	int double_move[t_state::order];
	memcpy(double_move,nstate,state_size);
	Topology::operateOnRestrictedSpace(nstate,double_move,A,false);
      }
      desc.State=nstate;
      result=checkConditions(desc,best_choice,foundBetter);
      if(result || foundBetter)
      {
	betterState.parent=trace_start;
	betterState.Op=(*a)|(mode<<3);
	memcpy(betterState.state,nstate,state_size);
	if(result)
	{
	  break;
	}
      }
    }
  }
  return result;
}