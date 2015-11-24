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
  os << sm.c_str() <<'('<<sm.Index<<')'<<std::flush;
}