#include"rubik.h"

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
