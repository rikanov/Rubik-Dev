#include"globals.h"
#include"rubik.h"

void Rubik::REPL()
{
  OUT_(NL<<"To log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems")
  String In;
  OUT("REPL > ")
  while( std::cin >> In )
  {
    const int request=parser(In);
    switch (request)
    {
      case 0:
	OUT_(NL<<(In=="" ? "REPL >":"Unrecognised command. To read manual, type H or Help..."))
	break;
      case 1:
	std::cin >> In;
	print(In);
	break;
      case 2:
	std::cin >> In;
	*this << In;
	break;
      case 3:
	std::cin>>In;
	OUT_(whatIs(In))
	break;
      case 4:
	std::cin>>In;
	OUT_(locationOf(In))
	break;
      case 5:
	OUT_("It is"<<(is_solved(IdentityMap,78) ? " solved." : " not solved."))
	break;
      case 6:
	print(Topology::SideMarks);
	break;
      case 7:
      {
	std::cin>>In;
	String Rest;
	getline(std::cin,Rest);
	bruteForce(Rest,In);
	break;
      }
      case 8:
	std::cin>>In;
	suppose(In);
	break;
      case 9:
	noSuppose();
	break;
      default:
	continue; // do nothing
    }
    OUT("REPL > ")
  };
  OUT_(NL<<"REPL mode has been closed...")
}

int Rubik::parser(const String & In) const
{
  if(In=="print")
    return 1;
  if(In=="do")
    return 2;
  if(In=="what_is")
    return 3;
  if(In=="find")
    return 4;
  if(In=="where_is")
    return 4;
  if(In=="location_of")
    return 4;
  if(In=="is_solved")
    return 5;
  if(In=="list")
    return 6;
  if(In=="brute_force")
    return 7;
  if(In=="!")
    return 7;
  if(In=="suppose")
    return 8;
  if(In=="no_suppose")
    return 9;
  return 0;
}

void Rubik::print(const String & C) const
{
  OUT("\nquery: "<<C<<NL<<"--------")
  C_FOR_STR(C,it)
  { 
    OUT('-')
  }
  OUT(NL<<'\t')
  int nl=0;
  for(int i=2; i<7 ;++i)
    for(int j=1; j<i ;++j)
    {
      if(C.find(Topology::SideMarks[i])==STR_END && C.find(Topology::SideMarks[j])==STR_END)
      {
	continue;
      }
      const int index= Topology::getIndex(0,i,j);
      if (index==0)
      {
	continue;
      }
      const Sidemarks Sm1=Sidemarks(index);
      const Sidemarks Sm2=whatIs(index);
      OUT(Sm1<<(index<10?" ":"")<<" -> "<<(B_map[index]<10?" ":"")<<Sm2<<"  ")
      if(++nl%4==0)
      {
	OUT(NL<<'\t')
      }
    }
  if(nl%4)
  {
    OUT(NL<<'\t')
  }
  nl=0;
  for(int i=3;i<7;++i)
    for(int j=2;j<i;++j)
      for(int k=1;k<j;++k)
      {
	if(C.find(Topology::SideMarks[i])==STR_END && 
	    C.find(Topology::SideMarks[j])==STR_END && 
	      C.find(Topology::SideMarks[k])==STR_END)
	{
	  continue;
	}
	const int index= Topology::getIndex(i,j,k);
	if(index==0)
	{
	  continue;
	}
	const Sidemarks Sm1=Sidemarks(index);
	const Sidemarks Sm2=whatIs(index);
	OUT(Sm1<<"->"<<Sm2<<"  ")
	if(++nl%4==0)
	{
	  OUT(NL<<'\t')
	}
      }
  OUT(NL)
}
