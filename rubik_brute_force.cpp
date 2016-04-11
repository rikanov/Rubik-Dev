#include"rubik.h"


Rubik_BF::Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth):
RubikBase(R)
{
  const int SizeS=auxiliary::countWords(IS);
  initStates(SizeS);
  setConditions(IS);
  if(AS=="*")
  {
    seekerDepth=6;
    Engine=&Rubik_BF::fastestCheck;
  }
  else if(AS.length()==2 && AS[0]=='+')
  {
    seekerDepth=AS[1]-'0';
    if(seekerDepth<0 || seekerDepth>6)
    {
      seekerDepth=0;
    }
    Engine=&Rubik_BF::checkConditions;
  }
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
    InitialState[i]=RubikBase->locationOf(i);
  }
}

int Rubik_BF::checkConditions(const int * Foresight, const int * Trail)
{
  foundBetter=false;
  int Result=0;
  int cond=0;
  int counter=0;
  bool found=false;
  for(const int *c=SolvedState; *c!=-256; ++c)
  {
    if(*c<0)
    {
      ++Result;
      if(cond>=-(*c))
      {
	found=true; 
	break;
      }
    }
    else
    {
      const int add=Trail[Foresight[InitialState[*c]]]==*c; 
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

int Rubik_BF::fastestCheck(const int* Foresight, const int* Trail)
{
  for(const int *c=SolvedState; *c>=0; ++c)
  {
    if(Trail[Foresight[InitialState[*c]]]!=*c)
    {
      return 0;
    }
  }
  return 1;
}
std::pair< int, std::string > Rubik_BF::start()
{
  const Topology::t_state* Trace=Topology::getTrace();
  int result=0;
  foundBetter=false;
  best_choice=0;
  String Result;
  const int step=Topology::TraceSize/30;
  int bar=0;
  int bar_length=0;
  const Topology::t_state* T_short=Topology::getTrace();
  const Topology::t_state* checkPoints[6];
  const Topology::t_state* trail[6];
  for(int i=0;i<6;++i)
  {
    trail[i]=checkPoints[i]=Topology::getTrace(i);
  }
  while(result==0 && T_short->length<5)
  {
    result=(this->*Engine)(T_short->state,IdentityMap);
    if(result||foundBetter)
    {
      Result=T_short->path();
    }
    if(result)
    {
      break; 
    }
    ++T_short; 
  } 
  
  for(int depth=0;depth<=seekerDepth; ++depth)
  {
    for(const Topology::t_state* T=T_short;result==0 && T->state!=nullptr;++T)
    {
      if((bar++)%step==0)
      {
	auxiliary::drawBarLine((bar_length++)%30,30);OUT(" depth: "<<(5+depth)<<" best found: "<<Result<<"   ");
      }
      for(int side=0;side<6;++side)
      {  
	if(T->last==side)
	{
	  continue;
	}
	for(trail[side]=checkPoints[side];trail[side]->length<depth;++trail[side])
	{ 
	  result=(this->*Engine)(T->state,trail[side]->state);
	  if(result||foundBetter)
	  {
	    Result=auxiliary::mergeSimplePaths(T->path(),trail[side]->path());
	  }
	  if(result)
	  {
	    depth=seekerDepth+1; side=6; // to escape from the outer loops too
	    break; 
	  }
	} 
      }
    }
    for(int i=0;i<6;++i)
    {
      checkPoints[i]=trail[i];
    }
  }
  auxiliary::drawBarLine((bar_length++)%30,30);
  OUT_(" Result:  "<<Result<<(result ? " is a solution for condition: " : " best choice to solve ")<<(result?result:best_choice));
  return std::pair<int,String> (result,Result);
}

Rubik_BF::~Rubik_BF()
{
}