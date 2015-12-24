#include "auxiliary.h"

auxiliary::Sidemarks::Sidemarks(const int& In): 
  Index(In), 
  std::string(Topology::sideMarksOf(In)),
  eigenvalue(Topology::getEigenvalue(In))
{
  
}

auxiliary::Sidemarks::Sidemarks(const char* C): Sidemarks(String(C))
{

}

auxiliary::Sidemarks::Sidemarks(const String& S): 
  Index(Topology::getIndex(S)),
  String(S),
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
///=====================================================================

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

void auxiliary::imbueStream(Stream& IS, const String & V)
{
  String IS_;
  if(IS.good())
  {
    getline(IS,IS_);
    IS_=' '+IS_;
  }
  IS.str(String());
  IS.clear();
  IS<<V<<IS_;
}

String& auxiliary::regSimplifier(const String& R, String& S)
{
  bool asterisk=false;
  C_FOR_STR(R,rit)
  {
    if(asterisk)
    {
      switch(*rit)
      {
	case '?':
	  S.push_back('?');
	  break;
	case '*':
	  SKIP
	  break;
	default:
	  asterisk=false;
	  S.push_back('*');
	  S.push_back(*rit);
      }
    }
    else if(*rit=='*')
    {
      asterisk=true;
    }
    else
    {
      S.push_back(*rit);
    }
  }
  if(asterisk)
  {
    S.push_back('*');
  }
  return S;
}

bool auxiliary::regExp(const char* R, const char* C)
{
  if (*R=='*' && *(R+1)==0) 
  {
    return true;
  }
  if (*C==0)
  {
    return *R==0; 
  }
  
  switch(*R) { 
    case 0: 
      return false;
    
    case '\\':
      return *(++R)==*C ? regExp(++R, ++C) : false;
  
    case '!':
      return *(++R)!=*C ? regExp(++R, ++C) : false;
      
    case '~':
      return *(++R)!=*C ? regExp(R+1, C) || regExp(R+1, C+1) : false;
      
    case '?': 
      return regExp(++R, ++C);
    
    case '*': 
      return regExp(R+1,C) || regExp(R,C+1 );

    default:
      return (*R)==(*C) ? regExp(++R, ++C) : false;
  }
}

String auxiliary::putInString(const std::string& arg, const char& sig, const String& text)
{
  String Result;
  C_FOR_STR(text,t)
  {
    if(*t!=sig)
    {
      Result.push_back(*t);
    }
    else
    {
      Result.append(arg);
    }
  }
  return Result;
}

String auxiliary::putInString(const String& arg1, const char& sig1, const String& arg2, const char& sig2, const String& text)
{
  String Result;
  C_FOR_STR(text,t)
  {
    if(*t==sig1)
    {
      Result.append(arg1);
    }
    else if(*t==sig2)
    {
      Result.append(arg2);
    }
    else
    {
      Result.push_back(*t);
    }
  }
  return Result;
}

unsigned int auxiliary::countWords(Stream& IS)
{
  Stream SS(IS.str());
  return std::distance(std::istream_iterator<String>(SS), std::istream_iterator<String>());
}

unsigned int auxiliary::countWords(const String& S)
{
  Stream str(S);
  return std::distance(std::istream_iterator<std::string>(str), std::istream_iterator<std::string>());
}
