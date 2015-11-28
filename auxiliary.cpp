#include "auxiliary.h"

auxiliary::Sidemarks::Sidemarks(const int& In): Index(In), std::string(Topology::sideMarksOf(In))
{
  
}

auxiliary::Sidemarks::Sidemarks(const char* C): Index(Topology::getIndex(C)), std::string(C)
{

}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const std::string& S)
{
  std::string::operator=(S);
  Index=Topology::getIndex(S);
}

auxiliary::Sidemarks::Sidemarks(const std::string& S): Index(Topology::getIndex(S)),std::string(S)
{
  
}

std::ostream& auxiliary::operator<<(std::ostream& os, const auxiliary::Sidemarks& sm)
{
  os << sm.c_str() <<'('<<sm.Index<<')';
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

