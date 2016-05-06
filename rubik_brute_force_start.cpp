#include "rubik.h" 

static int bar_length=0;

bool Rubik_BF::checkResult(int & result, String & Result, const Topology::t_state * S1, const Topology::t_state * S2)
{
  result=(this->*Engine)(S1,S2);
  if(result||foundBetter)
  {
    Result=resolver(S1,S2);
  }
  return result;
}

int Rubik_BF::searchManager(int& result, String& Result, const int & level)
{
  SearchEngine stored=Engine;
  if(level==0)
  {
    Engine=&Rubik_BF::fastestCheck;
  }
  for(Topology::RotationRange range(0,CONFIG_CACHE_MEMORY_USAGE); result==0 && range.state(); range.next())
  {
    if
    (
      (level<2  && checkResult(result,Result,range.state(),Topology::getTrace())) ||
      (level==2 && secondStage(result,Result,range.state()))
    )
    {
      break;
    }
  }
  if(level==0)
  {
    Engine=stored;
  }
  return result;
}

int Rubik_BF::secondStage(int & result, String & Result, const Topology::t_state * T)
{
  auxiliary::drawBarLine(bar_length++,30); // to show the engine stills work
  for(Topology::RotationRange range(CONFIG_CACHE_MEMORY_USAGE-1,CONFIG_CACHE_MEMORY_USAGE); result==0 && range.state(); range.next())
  {
    if(checkResult(result,Result,T,range.state()))
    {
      break;
    }
  }
  return result;
}

std::pair< int, String > Rubik_BF::start()
{
  if(fastestCheck(Topology::getTrace(), Topology::getTrace()))
  {
    return std::pair< int, String >(1,NIL);
  }
  foundBetter=false;
  best_choice=0;
  String Result;
  int result=0;
  int bar=0;
  int bar_length=0;
  if(cluster.initialized())
  {
    searchManager(result, Result, 0); // heuristic searches required checking of short solutions
  }
  searchManager(result, Result, 1);
  searchManager(result, Result, 2);
  OUT("\r Result:  "<<Result<<(result ? " is a solution for condition: " : " best choice to solve ")<<(result?result:best_choice))
  OUT_("                        ");
  return std::pair<int,String> (result,Result);
}