#include"rubik.h"

Rubik_BF::Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth):
RubikBase(R),
seekerStep(bfWidth),
barLength(60)
{
  const int SizeS=auxiliary::countWords(IS);
  initStates(SizeS);
  setConditions(IS);
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  AllowedSides=new int [allowed_sides.length()+1];
  for(int i=0; i<allowed_sides.length();++i)
  {
    AllowedSides[i]=Topology::sideDigit(allowed_sides[i]);
  }
  AllowedSides[allowed_sides.length()]=-1;
  initTrace();
}

void Rubik_BF::initStates(const int& SizeS)
{
  SolutionIndices = new int [SizeS+1];
  SolvedState =  new int [NumberOfSideMarks];
  InitialState = new int [NumberOfSideMarks];
  memset(SolutionIndices,0,SizeS+1);
  memset(SolvedState,0,NumberOfSideMarks);
  memset(InitialState,0,NumberOfSideMarks);
}

void Rubik_BF::initTrace()
{
  int length=1;
  for(const int * I=InitialState; *I!=-1; ++I,++length);
  t_state::order=length;
  seekerSize=seekerStep*barLength;
  Trace=new t_state[seekerSize];
  trace_start=Trace;
  trace_end=trace_start+1;
  trace_start->state=InitialState;
  best_choice=0;
}

void Rubik_BF::setConditions(std::stringstream& IS)
{
  int *sp=SolutionIndices,sp_end=0;
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
      InitialState[index]=RubikBase->locationOf(Solved);
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

int Rubik_BF::checkConditions()
{
  foundBetter=false;
  int Result=1;
  int cond=0;
  bool found=false;
  for(const int *c=SolutionIndices; *c!=-256; ++c)
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
    cond+=(toTest->state[*c]==SolvedState[*c]);
  }
  return found ? Result : 0;
}

std::pair< int, std::string > Rubik_BF::start()
{
  int trace_length=1; 
  int result=0;
  for(foundBetter=false; trace_length<seekerSize;++trace_start)
  {
    toTest=trace_start;
    result=checkConditions();
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
      extendNode();
    }
  }
  int bar=0;
  while(++trace_start!=trace_end)
  {
    if(--trace_length % seekerStep ==0)
    {
      auxiliary::drawBarLine(bar++, barLength);
    }
    toTest=trace_start;
    result=checkConditions();
    if(result)
    {
      Solution=trace_start;
      break;
    }
    else if(foundBetter)
    {
      Solution=trace_start;
    }
  }
  return std::pair<int,String> (result,Solution->path());
}

void Rubik_BF::extendNode()
{
  for(const int *a=AllowedSides; *a!=-1; ++a)
  {
    if(*a==((trace_start->Op)&7))
    {
      continue;
    }
    const int A=*a-1;
    trace_end->state=new int[t_state::order];
    Topology::operateOnRestrictedSpace(trace_end->state,trace_start->state,A);
    trace_end->parent=trace_start;
    trace_end->Op=*a;
    ++trace_end;
    for(int mode=1;mode<3;++mode)
    {
      trace_end->state=new int[t_state::order];
      Topology::operateOnRestrictedSpace(trace_end->state,(trace_end-1)->state,A);
      trace_end->parent=trace_start;
      trace_end->Op=(*a)|(mode<<3);
      ++trace_end;
    }
  }
}

int Rubik_BF::examineNode()
{
 
}