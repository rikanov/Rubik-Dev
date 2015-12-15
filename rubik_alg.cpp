#include"rubik.h"

String Rubik::bruteForce(const auxiliary::Line & Solutions, const String & AS) const
{
  const int SizeS=Solutions.size();
  int SolutionIdices[SizeS+1];
  int SolvedState[NumberOfSideMarks];
  int InitialState[NumberOfSideMarks];
  std::list<t_state> Seeking;
  setConditions(SolutionIdices,SolvedState,InitialState,Solutions,Seeking);
  
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  int allowed[allowed_sides.length()+1];
  for(int i=0; i<allowed_sides.length();++i)
  {
    allowed[i]=Topology::sideDigit(allowed_sides[i])-1;
  }
  allowed[allowed_sides.length()]=-1;
  std::pair<int,String> Result=seeker(Seeking,SolvedState,SolutionIdices,allowed);
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
  return Result.second;
}

void Rubik::setConditions(int * SolutionIdices, int * SolvedState, int * InitialState, const Line& Solutions, std::list<t_state> & Seeking) const
{
  int *sp=SolutionIdices,sp_end=0;
  for(String s: Solutions)
  {
    if(s=="*")
    {
      *(sp++)=-1;
      continue;
    }
    const int Solved=Topology::getIndex(s);
    int index=0;
    for(; index<sp_end;++index)
    {
      if(SolvedState[index]==Solved)
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
  *sp=-2;  // the end sign of testing
  InitialState[sp_end]=-1; // end of inner state
  auxiliary::t_state::order=sp_end+1;
  Seeking.push_back(t_state(InitialState,-1,""));
}

int Rubik::checkConditions(const int *State, const int * SolvedState, const int * Conditions) const
{
  int Result=1;
  bool cond=true;
  for(const int *c=Conditions; *c!=-2; ++c)
  {
    if(*c==-1)
    {
      if(cond)
      {
	break;
      }
      else
      {
	cond=true;
	++Result;
      }
    }
    cond&=(State[*c]==SolvedState[*c]);
  }
  return cond;
}

std::pair<int,String> Rubik::seeker(std::list<t_state> & Trace, const int * SolvedState, 
				    const int * Conditions, const int * AllowedSides) const			       
{
  const int pre_check=checkConditions(Trace.front().State,SolvedState,Conditions);
  if(pre_check)
  {
    return std::pair<int,String>(pre_check,"");
  }
  const int step=2000;
  String A="",B="----------------------------";
  const char mode_sign[]={'x','\'','2'};
  int nstate[auxiliary::t_state::order];
  while(true) // the exit condition what will be never happened :-)
  {
    for(const int *a=AllowedSides; *a!=-1; ++a)
    {
      if(*a==Trace.front().LastSide)
      {
	continue;
      }
      for(int mode=0; mode<3;++mode)
      {
	Topology::operateOnRestrictedSpace(nstate,Trace.front().State,*a,mode==1);
	if(mode==2)
	{
	  EMPTY(double_move);
	  CPY_FUNC(double_move,nstate);
	  Topology::operateOnRestrictedSpace(nstate,double_move,*a,false);
	}
	String npath=Trace.front().Path+Topology::sideMarksOf(*a);
	if(mode)
	{
	  npath.push_back(mode_sign[mode]);
	}
	const int result=checkConditions(nstate,SolvedState,Conditions);
	if(result)
	{
	  if(Trace.size()>=step)
	  {
	    NL_
	  }
	  return std::pair<int,String>(result,npath);
	}
	Trace.push_back(t_state(nstate,*a,npath));
	if(Trace.size()%step==0)
	{
	  if(B.empty())
	  {
	    NL_
	    return std::pair<int,String>(0,"");
	  }
	  A.push_back('=');
	  B.pop_back();
	  std::cout<<"\r["<<A<<B<<"]"<<std::flush;
	}
      }
    }
      Trace.pop_front();
  }
  return std::pair<int,String>(0,"");
}

String Rubik::findPath(const Sidemarks& From, const Sidemarks& To)
{
  if(From==To  || From.type()!= To.type() || From.getEigenvalue()!=To.getEigenvalue())
  {
    return "";
  }
  typedef std::pair<Sidemarks,String>  state;
  std::list<state> Trace;
  Trace.push_back(state(From,"E")); // E means: Empty. It will be deleted before return
  while(true)
  {
    Sidemarks next_pos=Trace.front().first;
    String path=Trace.front().second;
    const char last_sign=path.back();
    const char last_move= isletter(last_sign) ? last_sign : path[path.length()-2];
    const String sides(next_pos);
    C_FOR_STR(sides,s)
    {
      if(*s==last_move)
      {
	continue;
      }
      for(int mode=0;mode<3;++mode)
      {
	String op;
	op.push_back(*s);
	switch(mode)
	{
	  case 1:  // inverse
	    op.push_back('\'');
	    break;
	  case 2:  // double
	    op.push_back('2');
	    break;
	  default:
	    SKIP
	}
	Sidemarks next=next_pos+op;
	if(next==To)
	{
	  path.erase(path.begin());
	  return path+op;
	}
	Trace.push_back(state(next,path+op));
      }
    }
    Trace.pop_front();
  }
}