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
  *sp=-256;
  FOR_FUNC(i)
  {
    InitialState[i]=RubikBase->locationOf(i);
  }
  for(int *s=SolvedState;*s!=-256;++s)
  {
    OUT((*s)<<' ')
  }NL_
}

int Rubik_BF::checkConditions(const int * Foresight)
{
  foundBetter=false;
  int Result=1;
  int cond=0;
  bool found=false;
  for(const int *c=SolvedState; *c!=-256; ++c)
  {
    if(*c<0)
    {
      if(cond>=-(*c))
      {
	found=true; OUT_("found best")
	break;
      }
      else
      {	
	if(cond>best_choice)
	{
	  best_choice=cond; OUT_("found better")
	  foundBetter=true;
	}
	cond=0;
	++Result;
	continue;
      }
    }
    else
    {
      cond+=(Foresight[InitialState[*c]]==*c);
    }
  }
  return found ? Result : 0;
}

std::pair< int, std::string > Rubik_BF::start()
{
  int result=0;
  best_choice=0;
  String Result;
  int bar=0;
  for(const Topology::t_state* T=Topology::getTrace();T!=nullptr;++T)
  {
    if((bar++)%50000==0)
    {
      auxiliary::drawBarLine(bar/50000,19);
    }
    result=checkConditions(T->state); 
    if(result||foundBetter)
    {
      Result=T->path();
    }
  }
  OUT_("Result:  "<<Result);
  return std::pair<int,String> (result,Result);
}

Rubik_BF::~Rubik_BF()
{
}