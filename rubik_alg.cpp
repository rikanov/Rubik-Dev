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
  t_state InitSeeking=t_state(InitialState,-1,"");
  
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  int allowed[allowed_sides.length()+1];
  for(int i=0; i<allowed_sides.length();++i)
  {
    allowed[i]=Topology::sideDigit(allowed_sides[i])-1;
  }
  allowed[allowed_sides.length()]=-1;
  std::pair<int,String> Result=seeker(InitSeeking,SolvedState,SolutionIndices,allowed);
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
  auxiliary::t_state::order=sp_end+1;
}

int Rubik::checkConditions(const int *State, const int * SolvedState, const int * Conditions, int & best_choice) const
{
  int Result=1;
  int cond=0;
  bool found=false;
  for(const int *c=Conditions; *c!=-256; ++c)
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
	}
	cond=0;
	++Result;
	continue;
      }
    }
    cond+=(State[*c]==SolvedState[*c]);
  }
  return found ? Result : 0;
}

std::pair<int,String> Rubik::seeker(t_state & InitTrace, const int * SolvedState, 
				    const int * Conditions, const int * AllowedSides) const			       
{
  static const char mode_sign[]={'x','\'','2'};
  
  int best_choice=0;
  String part_solution;
  const int pre_check=checkConditions(InitTrace.State,SolvedState,Conditions, best_choice);
  if(pre_check)
  {
    return std::pair<int,String>(pre_check,"");
  }
  const int step=40000;
  String A="",B="---------------------------------";
  t_state * Trace=new t_state [step*(B.length()+3)];
  t_state * trace_start=Trace;
  t_state * trace_end=trace_start+1;
  int trace_length=1;
  *trace_start=InitTrace;
  int nstate[auxiliary::t_state::order];
  int Result=0;
  bool seek_for=true;
  while(seek_for) // the exit condition what will be never happened :-)
  {
    for(const int *a=AllowedSides; seek_for && *a!=-1; ++a)
    {
      if(*a==trace_start->LastSide)
      {
	continue;
      }
      for(int mode=0; mode<3;++mode)
      {
	Topology::operateOnRestrictedSpace(nstate,trace_start->State,*a,mode==1);
	if(mode==2)
	{
	  EMPTY(double_move);
	  CPY_FUNC(double_move,nstate);
	  Topology::operateOnRestrictedSpace(nstate,double_move,*a,false);
	}
	String npath=trace_start->Path+Topology::sideMarksOf(*a);
	if(mode)
	{
	  npath.push_back(mode_sign[mode]);
	}
	int test=best_choice;
	const int result=checkConditions(nstate,SolvedState,Conditions,test);
	if(test>best_choice)
	{
	  best_choice=test;
	  part_solution=npath;
	}
	if(result)
	{
	  if(trace_length>=step)
	  {
	    NL_
	  }
	  Result=result;
	  part_solution=npath;
	  seek_for=false;
	  break;
	  //return std::pair<int,String>(result,npath);
	}
	(trace_end++)->set(nstate,*a,npath);
	++trace_length;
	if(trace_length%step==0)
	{
	  if(B.empty())
	  {
	    NL_
	    Result=0;
	    seek_for=false;
	    break;
	    //return std::pair<int,String>(0,"");
	  }
	  A.push_back('=');
	  B.pop_back();
	  std::cout<<"\r["<<A<<B<<"]"<<std::flush;
	}
      }
    }
      ++trace_start;
  }
  delete[] Trace; 
  return std::pair<int,String>(Result,part_solution);
}