#include "auxiliary.h"

auxiliary::Sidemarks::Sidemarks(const int& In): 
  Index(In), 
  std::string(Topology::sideMarksOf(In)),
  eigenvalue(Topology::getEigenvalue(In)),
  pivotNumber(Topology::getPivotNumber(In))
{
  
}

auxiliary::Sidemarks::Sidemarks(const char* C): Sidemarks(String(C))
{

}

auxiliary::Sidemarks::Sidemarks(const String& S): 
  Index(Topology::getIndex(S)),
  String(S),
  eigenvalue(Topology::getEigenvalue(Index)),
  pivotNumber(Topology::getPivotNumber(Index))
{
  
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const std::string& S)
{
  std::string::operator=(S);
  Index=Topology::getIndex(S);
  eigenvalue=Topology::getEigenvalue(Index);
  pivotNumber=Topology::getPivotNumber(Index);
  return *this;
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator=(const int& In)
{
  Index=In;
  std::string::operator=(Topology::sideMarksOf(Index));
  eigenvalue=Topology::getEigenvalue(Index);
  pivotNumber=Topology::getPivotNumber(Index);
  return *this;
}

auxiliary::Sidemarks& auxiliary::Sidemarks::operator<<(const std::string& S)
{
  const bool real_moves= S.front()=='@';
  C_FOR_STR(S,s)
  {
    const bool middle=(s+1)!=S.end() && *(s+1)=='|'; 
    const int side=Topology::sideDigit(*s)-1;
    if(side<0 || (!real_moves && !is_acting(*s,middle)))
    {
      continue;
    }
    const bool inv=(s+1)!=S.end() && *(s+1)=='\''; 
    const bool dub=((s+1)!=S.end() && *(s+1)=='2') || (middle && (s+2)!=S.end() && *(s+2)=='|');
    Index=Topology::rotation(Index,side,inv);
    if(dub)
    {
      Index=Topology::rotation(Index,side);
    }
    s+=middle+inv+dub;
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

int auxiliary::Sidemarks::type() const
{
  return Index>0 ? String::size() : 0;
}

bool auxiliary::Sidemarks::is_acting(const char& C, const bool& middle) const
{
  const size_t n=find(C);
  return middle ? n==STR_END && find(Topology::oppositeSide(C))==STR_END : n!=STR_END;
}

bool auxiliary::Sidemarks::sameCubes(const auxiliary::Sidemarks& A, const auxiliary::Sidemarks& B)
{
  if(A.type()!=B.type())
  {
    return false;
  }
  C_FOR_STR(A,a)
  {
    if(B.find(*a)==STR_END)
    {
      return false;
    }
  }
  return true;
}

auxiliary::CubeletColors auxiliary::Sidemarks::getColors(const Sidemarks & Cubelet) const
{
  CubeletColors Result;
  if(eigenvalue==Cubelet.eigenvalue)
  {
    for(int pos=0;pos<length();++pos)
    {
      const char CH=at(pos);
      const int CO=Topology::sideDigit(Cubelet.at(pos));
      switch(CH)
      {
	case 'F':
	  Result.front=CO;
	  break;
	case 'U':
	  Result.up=CO;
	  break;
	case 'R':
	  Result.right=CO;
	  break;
	case 'L':
	  Result.left=CO;
	  break;
	case 'D':
	  Result.down=CO;
	  break;
	case 'B':
	  Result.back=CO;
	  break;
	default:
	  ;
      }
    }
  }
  return Result;
}
///=============================================================================================================
