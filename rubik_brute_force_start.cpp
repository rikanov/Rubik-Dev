#include "rubik.h"


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
	  result=(this->*Engine)(T,trail[side]);
	  if(result||foundBetter)
	  {
	    Result=resolver(T,trail[side]);
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
