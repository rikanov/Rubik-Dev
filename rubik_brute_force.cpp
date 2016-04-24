#include"rubik.h"


Rubik_BF::Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth):
RubikBase(R)
{
  const int SizeS=auxiliary::countWords(IS);
  initStates(SizeS);
  setConditions(IS);
  if(AS=="*")
  {
    seekerDepth=5;
    Engine=&Rubik_BF::fastestCheck;
  }
  else if(AS.length()==2 && AS[0]=='*')
  {
    cluster.clusterInit(AS[1]-'0',SolvedState);
    seekerDepth=5;
    Engine=&Rubik_BF::deepestCheck;
  }
  else if(AS.length()==2 && AS[0]=='+')
  {
    seekerDepth=AS[1]-'0';
    if(seekerDepth<0 || seekerDepth>5)
    {
      seekerDepth=0;
    }
    Engine=&Rubik_BF::checkConditions;
  }
  initTrace();
}

void Rubik_BF::initStates(const int& SizeS)
{
  SolvedState     = new int [NumberOfSideMarks];
  InitialState    = new int [NumberOfSideMarks];
  InvInitialState = new int [NumberOfSideMarks];
  memset(SolvedState,0,NumberOfSideMarks);
  memset(InitialState,0,NumberOfSideMarks);
  memset(InvInitialState,0,NumberOfSideMarks);
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
  Topology::inverse(InitialState,InvInitialState);
}

String Rubik_BF::resolver(const Topology::t_state* A,const Topology::t_state* B)
{
  String Result;
  Result=auxiliary::mergeSimplePaths(A->path(),B->path());
  if(cluster.found)
  {
    Result=auxiliary::mergeSimplePaths(Result,(*cluster.found)->path());
  }
  return Result;
}

Rubik_BF::~Rubik_BF()
{
  delete[] InitialState;
  delete[] SolvedState;
}