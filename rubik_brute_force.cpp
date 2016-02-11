#include"rubik.h"

Rubik_BF::Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth):
RubikBase(R)
{
  const int SizeS=auxiliary::countWords(IS);
  initStates(SizeS);
  setConditions(IS);
  const String allowed_sides= (AS=="ALL" || AS=="All" || AS=="all") ? "FURBDL" : AS;
  initTrace();
}

void Rubik_BF::initStates(const int& SizeS)
{
  SolvedState =  new int [NumberOfSideMarks];
  InitialState = new int [NumberOfSideMarks];
  memset(SolvedState,0,NumberOfSideMarks);
  memset(InitialState,0,NumberOfSideMarks);
}

void Rubik_BF::initTrace()
{
  best_choice=0;
}

void Rubik_BF::setConditions(std::stringstream& IS)
{
  int *sp=SolvedState, s_counter=0;
  String s;
  while(IS>>s)
  {
    if(s=="*")
    {
      *sp=s_counter;
      s_counter=0;
    } 
    else if(atoi(s.c_str())>0)
    {
      *sp=-atoi(s.c_str());
      s_counter=0;
    }
    else
    {
      *sp=Sidemarks(s);
      ++s_counter;
    }
    ++sp;
  }
  if(s_counter)
  {
    *(sp++)=-s_counter;
  }
  *sp=-256;
  FOR_FUNC(i)
  {
    InitialState[i]=RubikBase->whatIs(i);
  }
}

int Rubik_BF::checkConditions(const int * Foresight, const int * Trail)
{
  foundBetter=false;
  int Result=0;
  int cond=0;
  bool found=false;
  for(const int *c=SolvedState; *c!=-256; ++c)
  {
    if(*c<0)
    {
      ++Result;
      if(cond>=-(*c))
      {
	found=true;
	cond=0;
	break;
      }
    }
    else
    {
      cond+=(InitialState[*c]==Trail[Foresight[*c]]);
    }
  }
  if(cond>best_choice)
  {
    best_choice=cond; 
    foundBetter=true;
  }
  return found ? Result : 0;
}

std::pair< int, std::string > Rubik_BF::start()
{
  const Topology::t_state* Trace=Topology::getTrace();
  int result=0;
  best_choice=0;
  String Result;
  const int step=Topology::TraceSize/30;
  int bar=0;
  for(const Topology::t_state* T=Topology::getTrace();result==0 && T->state!=nullptr;++T)
  {
    if((bar++)%step==0)
    {
      auxiliary::drawBarLine(bar/step,30);
    }
    for(const Topology::t_state * trail=Topology::getExtender((T->Op)&7)+1;trail->state!=nullptr;++trail)
    { 
      result=checkConditions(T->state,trail->state); 
      if(result||foundBetter)
      {
	Result=auxiliary::mergeSimplePaths(T->path(),trail->path());
      }
      if(result)
      {
	break; 
      }
    }
  }
  OUT_("Result:  "<<Result<<(result ? " result: " : " best choice: ")<<(result?result:best_choice));
  return std::pair<int,String> (result,Result);
}

Rubik_BF::~Rubik_BF()
{
}