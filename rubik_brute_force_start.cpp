#include "rubik_bf.h" 

static int bar_length=0;
static bool show_bar = false;

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
  if(result)
  {
    return result;
  }
  for(Topology::RotationRange range(0,CONFIG_CACHE_MEMORY_USAGE); range.state(); range.next())
  {
    if(show_bar)
    {
      auxiliary::drawBarLine(bar_length++, 30);
    }
    if
    (
      (level==1 && checkResult(result,Result,range.state(),Topology::getTrace())) ||
      (level==2 && secondStage(result,Result,range.state()))
    )
    {
      break;
    }
  }
  return result;
}

int Rubik_BF::secondStage(int & result, String & Result, const Topology::t_state * T)
{
  static int bline=0;
  if(result)
  {
    return result;
  }
  if(bline++ == 30)
  {
    bline=0;
    auxiliary::drawBarLine(bar_length++,30); // to show the engine stills work
  }
  for(Topology::RotationRange range(CONFIG_CACHE_MEMORY_USAGE-1,CONFIG_CACHE_MEMORY_USAGE); range.state(); range.next())
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
  show_bar=false;
  if(isHeuristicSearch())
  {
    useExtendedPath=false;
    Engine=&Rubik_BF::fastestCheck;
    searchManager(result, Result, 1); // heuristic searches required checking of short solutions
    
    show_bar=true;
    Engine=&Rubik_BF::heuristicSearch; 
    OUT_("\nis the heuristic search needed ? "<<(result ? "no" : "yes\n"))
    searchManager(result, Result, 1);
    
    useExtendedPath=true; 
    OUT("\r                                     ")
    OUT_("\ris the extended search needed ? "<<(result ? "no" : "yes\n"))
    searchManager(result, Result, 1); 
    OUT("\r                                     ")
  }
  else
  {
    show_bar=false;
    searchManager(result, Result, 1);
    searchManager(result, Result, 2);
  }
  OUT("\nResult:  "<<Result<<(result ? " is a solution for condition: " : " best choice to solve ")<<(result?result:best_choice))
  OUT_("                        ");
  return std::pair<int,String> (result,Result);
}