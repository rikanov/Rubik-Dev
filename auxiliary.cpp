#include "auxiliary.h"

auxiliary::Sidemarks::Sidemarks(const int& In): 
  Index(In), 
  std::string(Topology::sideMarksOf(In)),
  eigenvalue(Topology::getEigenvalue(In))
{
  
}

auxiliary::Sidemarks::Sidemarks(const char* C): 
  Index(Topology::getIndex(C)), 
  std::string(C),
  eigenvalue(Topology::getEigenvalue(Index))
{
  
}

auxiliary::Sidemarks::Sidemarks(const std::string& S): 
  Index(Topology::getIndex(S)),
  std::string(S),
  eigenvalue(Topology::getEigenvalue(Index))
{
  
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const std::string& S)
{
  std::string::operator=(S);
  Index=Topology::getIndex(S);
  eigenvalue=Topology::getEigenvalue(Index);
  return *this;
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const int& In)
{
  Index=In;
  std::string::operator=(Topology::sideMarksOf(Index));
  eigenvalue=Topology::getEigenvalue(Index);
  return *this;
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

bool auxiliary::Sidemarks::valid() const
{
  return (Index>=0);
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

int * auxiliary::nestedLoop(int* Array, int depth, const int& UpperBound, const int& LowerBound )
{
  int *i=Array;
  ++(*i); 
  while( --depth && *i==UpperBound)
  {
    *(i++)=LowerBound;
    ++(*i);
  }
  return (depth || *i<UpperBound) ? i : nullptr;
}
