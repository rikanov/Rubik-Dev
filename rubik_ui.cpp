#include"rubik.h"

// TODO

void Rubik::REPL()
{
  OUT_(NL<<"To log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems")
  String In;
  OUT("REPL > ")
  do
  {
    const int request=parser(In);
    switch (request)
    {
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
      default:
	continue; // do nothing
    }
    OUT("REPL > ")
  }while( std::cin >> In );
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
      
  return 0;
}

void Rubik::print(String & C) const
{
  OUT("\nquery: "<<C<<NL<<"--------")
  FOR_STR(C,it)
  { 
    *it=UPCASE(*it);
    OUT('-')
  }
  OUT(NL<<'\t')
  for(int i=2; i<7 ;++i)
    for(int j=1; j<i ;++j)
    {
      if(C.find(SideMarks[i])==STR_END && C.find(SideMarks[j])==STR_END)
      {
	continue;
      }
      const int index= Topology::getIndex(0,i,j);
      if (index==0)
      {
	continue;
      }
      const Sidemarks Sm1=Sidemarks(index);
      const Sidemarks Sm2=Sidemarks(B_map[index]);
      OUT(Sm1<<(index<10?" ":"")<<" -> "<<(B_map[index]<10?" ":"")<<Sm2<<"  ")
    }
  OUT(NL<<'\t')
  for(int i=3;i<7;++i)
    for(int j=2;j<i;++j)
      for(int k=1;k<j;++k)
      {
	if(C.find(SideMarks[i])==STR_END && 
	    C.find(SideMarks[j])==STR_END && 
	      C.find(SideMarks[k])==STR_END)
	{
	  continue;
	}
	const int index= Topology::getIndex(i,j,k);
	if(index==0)
	{
	  continue;
	}
	const Sidemarks Sm1=Sidemarks(index);
	const Sidemarks Sm2=Sidemarks(B_map[index]);
	OUT(Sm1<<"->"<<Sm2<<"  ")
      }
  OUT(NL)
}
