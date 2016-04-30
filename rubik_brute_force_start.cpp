#include "rubik.h" 

std::pair< int, String > Rubik_BF::start()
{
  if(fastestCheck(Topology::getTrace(), Topology::getTrace()))
  {
    return std::pair< int, String >(1,NIL);
  }
  const char boringMark[]="-\\|/";
  int boringPhase=0;
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
  while(result==0 && T_short->length<CONFIG_CACHE_MEMORY_USAGE-1)
  {
    if(++bar%1000==0)
    {
      OUT('\r'<<boringMark[(boringPhase++)%4]<<" depth: "<<T_short->length+CONFIG_CACHE_MEMORY_USAGE);
    }
    result=(this->*Engine)(T_short,Topology::getTrace());
    if(result||foundBetter)
    {
      Result=resolver(T_short,Topology::getTrace());
    }
    if(result)
    {
      break; 
    }
    ++T_short; 
  } 
  bar_length=1;
  bar=0;
  for(int depth=0;result==0 && depth<=seekerDepth; ++depth)
  {
    for(const Topology::t_state* T=T_short;result==0 && T->state!=nullptr;++T)
    {
      result=(this->*Engine)(T,Topology::getTrace());
      if(++bar%1000==0)
      {
	OUT('\r'<<boringMark[(boringPhase++)%4]);
      }
      if(bar%step==0)
      {
	auxiliary::drawBarLine((bar_length++)%30,30);
	OUT(" best found: "<<Result<<"   ");
      }
      if(result||foundBetter)
      {
	Result=resolver(T,Topology::getTrace());
      }
      if(result)
      {
	break; 
      }
      for(int side=0;result==0 && side<6;++side)
      {  
	if(T->last==side)
	{
	  continue;
	}
	for(trail[side]=checkPoints[side];trail[side]->length<depth;++trail[side])
	{ 	  
	  result=(this->*Engine)(T,trail[side]);
	  if(result||foundBetter)
	  {
	    Result=resolver(T,trail[side]);
	  }
	  if(result)
	  {
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
 // auxiliary::drawBarLine((bar_length++)%30,30);
  OUT("\r Result:  "<<Result<<(result ? " is a solution for condition: " : " best choice to solve ")<<(result?result:best_choice))
  OUT_("                        ");
  return std::pair<int,String> (result,Result);
}
