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

auxiliary::Line& auxiliary::Line::fill_up(const std::string& Read, const std::string& Separator)
{
  int index=0, begin=-1;
  for(const char a: Read)
  {
    const bool S = (Separator.find(a)!=std::string::npos);
    if( !S && begin==-1 ) 
    {
      begin=index;
    } 
    else if( S && begin>-1)
    {
      begin=-1;
      const int Length=index-begin;
      push_back(Read.substr(begin, Length));
    }
    ++index;
  }
  if(begin>-1)
    push_back(Read.substr(begin));
}

auxiliary::Line::Line(const std::string& Read, const std::string& Separator)
{
  fill_up(Read,Separator);
}

std::ostream& auxiliary::operator<<(std::ostream& os, const auxiliary::Line& line)
{
  for(const std::string S : line)
    os << S;
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