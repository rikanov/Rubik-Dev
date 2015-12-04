#include"rubik.h"

String Rubik::bruteForce(const std::string& S, const std::string& AS)
{
  auxiliary::Line Solutions(S);
  const int SizeS=Solutions.size();
  int SolutionIdices[SizeS+1];
  int *sp=SolutionIdices;
  int SolvedState[NumberOfSideMarks],sp_end=0;
  int InitialState[NumberOfSideMarks];
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
      InitialState[index]=A_map[Solved];
      SolvedState[index]=Solved;
      ++sp_end; 
    }
  } 
  *sp=-2;  // the end sign of testing
  InitialState[sp_end]=-1; // end of inner state
  auxiliary::t_state::order=sp_end+1;
  std::list<t_state> Seeking;
  Seeking.push_back(t_state(InitialState,-1,""));
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  int allowed[allowed_sides.length()+1];
  for(int i=0; i<allowed_sides.length();++i)
  {
    allowed[i]=Topology::sideDigit(allowed_sides[i])-1;
  }
  allowed[allowed_sides.length()]=-1;
  std::pair<int,String> Result=seeker(Seeking,SolvedState,SolutionIdices,allowed);
#ifndef SILENT
  if(Result.first)
  {  
    OUT_( Result.second << " combination has been found for condition "+char('0'+Result.first) );
  }
  else
  {
    OUT_("Not found." );
  }
#endif
  return Result.second;
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
	  Topology::operateOnRestrictedSpace(nstate,Trace.front().State,*a,false);
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