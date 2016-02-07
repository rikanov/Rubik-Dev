#include"rubik.h"

Rubik_BF::Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth):
Trace(nullptr),
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
  const int maximumNumberOfNodes=200; // having improved Trace lenght by this overestimated vault, we mustn't check overflow
				     // in each iteration step. (preformance issue)
  Trace=new t_state[seekerSize+maximumNumberOfNodes];
  trace_start=Trace;
  trace_end=trace_start+1;
  trace_start->alloc()->copy(InitialState);
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

int Rubik_BF::checkConditions(const int * Foresight)
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
    cond+=(Foresight[trace_start->state[*c]]==SolvedState[*c]);
  }
  return found ? Result : 0;
}

std::pair< int, std::string > Rubik_BF::start()
{
  Solution=trace_start;
  int trace_length=1;  
  int result=0;
  String Result;
  int bar=0;
  for(foundBetter=false; trace_length<seekerSize;++trace_start)
  {
    result=checkConditions();
    if(result)
    {
      Solution=trace_start;  
      Result=Solution->path();
      break;
    }
    else
    {
      if(foundBetter)
      {
	Solution=trace_start;
	Result=Solution->path();
      }
      extendNode(trace_length);
    }
  }
  if(result==0)
  {
    Solution=new t_state;
    while(++trace_start!=trace_end)
    { 
#ifndef SILENT
      if(--trace_length % seekerStep ==0)
      {
	auxiliary::drawBarLine(bar++, barLength);
      }
#elif
      --trace_length;
#endif
      result=examineNode();
      if(result||foundBetter)
      {
	Result=Solution->path();
      }
      if(result)
      {	
	break;
      }
    }  
    delete Solution;
    Solution=nullptr;
  }
  OUT_("  "<<Result);
  return std::pair<int,String> (result,Result);
}

void Rubik_BF::extendNode(int & trace_length)
{
  for(const int *a=AllowedSides; *a!=-1; ++a)
  {
    const int A=(*a)-1;
    if(A==((trace_start->Op)&7))
    {
      continue;
    }
    trace_end->alloc();
    Topology::operateOnRestrictedSpace(trace_end->state,trace_start->state,A);
    trace_end->parent=trace_start;
    trace_end->Op=A+Topology::sideGroup(1);
    ++trace_end; ++trace_length;
    for(int mode=2;mode<4;++mode)
    {
      trace_end->alloc();
      Topology::operateOnRestrictedSpace(trace_end->state,(trace_end-1)->state,A);
      trace_end->parent=trace_start;
      trace_end->Op=A+Topology::sideGroup(mode);
      ++trace_end; ++trace_length;
    }
  }
}

int Rubik_BF::examineNode()
{
  int result=checkConditions();
  if(result||foundBetter)
  {
    Solution->parent=trace_start->parent;
    Solution->Op=trace_start->Op;
  }
  for(const int *a=AllowedSides; *a!=-1 && result==0; ++a)
  {
    const int A=(*a)-1;
    if(A==((trace_start->Op)&7))
    {
      continue;
    }
    for(int rot=1;rot<4;++rot)
    {
      const int SideRot=A+Topology::sideGroup(rot);
      result=checkConditions(Topology::rotation(SideRot));
      if(result||foundBetter)
      {
	Solution->parent=trace_start;
	Solution->Op=SideRot;
      }
      if(result)
      {
	break;
      }
    }
  }
  return result;
}

Rubik_BF::~Rubik_BF()
{
  for(trace_start=Trace; trace_start!=trace_end;++trace_start)
  {
    trace_start->dealloc();
  }
  delete[] Trace;
  trace_start=nullptr;
  trace_end=nullptr;
  Solution=nullptr;
  Trace=nullptr;
}