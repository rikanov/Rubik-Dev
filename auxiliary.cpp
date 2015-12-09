#include "auxiliary.h"

const char auxiliary::Sidemarks::PositiveGroup[][4]={"FUR","FRD","FDL","FLU","UBR","ULB","RBD","BLD"};
  
auxiliary::Sidemarks::Sidemarks(const int& In): Index(In), std::string(Topology::sideMarksOf(In))
{
  
}

auxiliary::Sidemarks::Sidemarks(const char* C): Index(Topology::getIndex(C)), std::string(C)
{
  
}

auxiliary::Sidemarks::Sidemarks(const std::string& S): Index(Topology::getIndex(S)),std::string(S)
{
  
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const std::string& S)
{
  std::string::operator=(S);
  Index=Topology::getIndex(S);
  return *this;
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const int& In)
{
  Index=In;
  std::string::operator=(Topology::sideMarksOf(Index));
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator<<(const std::string& S)
{
  const bool real_moves= S.front()=='@';
  C_FOR_STR(S,s)
  {
    const int side=Topology::sideDigit(*s)-1;
    if(side==-1)
    {
      continue;
    }
    if(!real_moves || Topology::onTheSide(Index,side))
    {
      bool Invert= (s+1!=S.end() && *(s+1)=='\'');
      Index=Topology::rotation(Index,side,Invert);
      if(s+1!=S.end() && *(s+1)=='2')
      {
	Index=Topology::rotation(Index,side,false); 
      }
    }
  }
  return operator=(Index);
}

auxiliary::Sidemarks auxiliary::Sidemarks::operator+(const String & S) const
{
  Sidemarks clone(*this);
  return clone << S;
}

const int & auxiliary::Sidemarks::setEigenvalue() const
{
  if(std::string::length()<3)
  {
    return eigenvalue=0;
  }
  else
  {
    char tester[4];
    strcpy(tester,std::string::c_str());
    for(int j=0;j<8;++j)
    {
      if(strcmp(tester,PositiveGroup[j])==0)
      {
	return eigenvalue=1;
      }
    }
    for(int i=0;i<2;++i)
    {
      const char t=tester[0];
      tester[0]=tester[1];
      tester[1]=tester[2];
      tester[2]=t; // cyclic rotation <<
      for(int j=0;j<8;++j)
      {
	if(strcmp(tester,PositiveGroup[j])==0)
	{
	  return eigenvalue=1;
	}
      }
    }
    return eigenvalue=-1;
  }
}

std::ostream& auxiliary::operator<<(std::ostream& os, const auxiliary::Sidemarks& sm)
{
  os << (std::string)sm <<'('<<(int)sm<<')';
  return os;
}

std::istream& auxiliary::operator>>(std::istream& is, auxiliary::Sidemarks& sm)
{
  String In;
  is >> In;
  sm=In;
  return is;
}

///=====================================================================


const std::string auxiliary::Line::DefaultSeparators=" \t\n;";

auxiliary::Line& auxiliary::Line::fill_up(const String& Read, const String & Separator)
{
  push_back("");
  for(char a: Read)
  {
    if(Separator.find(a)!=String::npos)
    {
      if(back()!="")
      {
	push_back("");
      }
      continue;
    }
    back().push_back(a);
  }
  if(back()=="")
  {
    pop_back();
  }
  return *this;
}

auxiliary::Line::Line(std::istream& IS)
{
  String A;
  while(IS>>A)
  {
    push_back(A);
  }
}

auxiliary::Line::Line(const String & Read, const String & Separator)
{
  fill_up(Read,Separator);
}

std::ostream& auxiliary::operator<<(std::ostream& os, const auxiliary::Line& line)
{
  for(const std::string S : line)
    os << S<<'('<<S.length()<<')';
  return os;
}

std::istream & auxiliary::operator>>(std::istream & is, auxiliary::Line & line)
{
  line.clear();
  std::string Read; is >> Read;
  line.push_back(Read);
  //line.fill_up(Read, Line::DefaultSeparators);
  return is;
}

///=================================================================================

int auxiliary::t_state::order=1;
auxiliary::t_state::t_state(const int* S, const int& L, const std::string& P): 
  State(new int[order]), 
  LastSide(L), 
  Path(P)
{
  memcpy(State,S,order*sizeof(int));
}

auxiliary::t_state::t_state(const auxiliary::t_state& T): 
  State(new int[order]), 
  LastSide(T.LastSide), 
  Path(T.Path) 
{
  memcpy(State,T.State,order*sizeof(int)); 
}

auxiliary::t_state& auxiliary::t_state::operator=(const auxiliary::t_state& T)
{
  if(this==&T)
  {
    return *this;
  }
  State=new int[order]; 
  LastSide=T.LastSide; 
  Path=T.Path;
  memcpy(State,T.State,order*sizeof(int)); 
}

auxiliary::t_state::~t_state()
{
  delete State;
  State=nullptr;
}
