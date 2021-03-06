#include"rubik_bf.h"

Rubik_BF::Cluster * Rubik_BF::cluster = new Cluster;
Rubik_BF::Rubik_BF(const Rubik* R, Stream& IS, const String& AS):
RubikBase(R),
useExtendedPath(false),
HeuristicSearch(true)
{
  const int SizeS=auxiliary::countWords(IS);
  initStates(SizeS);
  setConditions(IS);
  if(AS=="*")
  {
    seekerDepth=Topology::cacheLevel();
    Engine=&Rubik_BF::fastestCheck;
    cluster->clusterInit(0,SolvedState);
  }
  else if(AS.length()==2 && AS[0]=='*')
  {
    cluster->clusterInit(AS[1]-'0',SolvedState);
    seekerDepth=Topology::cacheLevel();
    Engine=&Rubik_BF::heuristicSearch;
  }
  else if(AS.length()==2 && AS[0]=='+')
  {
    seekerDepth=AS[1]-'0';
    if(seekerDepth<0 || seekerDepth>5)
    {
      seekerDepth=0;
    }
    Engine=&Rubik_BF::checkConditions;
    HeuristicSearch=false;
  }
}

void Rubik_BF::initStates(const int& SizeS)
{
  SolvedState     = new CubeSlot [NumberOfSideMarks];
  InitialState    = new CubeSlot [NumberOfSideMarks];
  InvInitialState = new CubeSlot [NumberOfSideMarks];
  memset(SolvedState,0,NumberOfSideMarks);
  memset(InitialState,0,NumberOfSideMarks);
  memset(InvInitialState,0,NumberOfSideMarks);
}

void Rubik_BF::setConditions(Stream& IS)
{
  CubeSlot *sp=SolvedState;
  int s_counter=0;
  String s; 
  while(IS>>s)
  {
    if(atoi(s.c_str())>0)
    {
      *sp=-atoi(s.c_str());
      s_counter=0;
    }
    else
    {
      *sp=Sidemarks(s);
      --s_counter;
    }
    ++sp;
  }
  if(s_counter)
  {
    *(sp++)=s_counter;
  }
  *sp=-128;
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
  if(cluster->found)
  {
    Result=auxiliary::mergeSimplePaths(Result,(*cluster->found)->path());
  }
  if(cluster->trail)
  {
    Result=auxiliary::mergeSimplePaths(Result,(*cluster->trail)->path());
  }
  return Result;
}

Rubik_BF::~Rubik_BF()
{
  delete[] InitialState;
  delete[] InvInitialState;
  delete[] SolvedState;
}