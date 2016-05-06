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
///=============================================================================================================

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
	  ; // SKIP
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

String auxiliary::putInString(const String& arg, const String& token, const String& text)
{
  size_t f=text.find(token);
  if(f!=STR_END)
  {
    String modded(text);
    modded.replace(f,token.length(),arg);
    putInString(arg,token,modded);
    return modded;
  }
  else
  {
    return text;
  }
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


String auxiliary::findPath(const Sidemarks& From, const Sidemarks& To, const bool& AllowMiddle)
{
  if(From==To  || From.type()!= To.type() || From.getEigenvalue()!=To.getEigenvalue())
  {
    return "";
  }
  typedef std::pair<Sidemarks,String>  state;
  std::list<state> Trace;
  Trace.push_back(state(From,"E")); // E means: Empty. It will be deleted before return
  while(true)
  {
    Sidemarks next_pos=Trace.front().first;
    String path=Trace.front().second;
    const char last_sign=path.back();
    const char last_move= isletter(last_sign) ? last_sign : path[path.length()-2];
    const String sides(AllowMiddle ? "FRUBLD" : next_pos);
    C_FOR_STR(sides,s)
    {
      if(*s==last_move)
      {
	continue;
      }
      for(int mode=0;mode<3+(2*AllowMiddle);++mode)
      {
	String op;
	op.push_back(*s);
	switch(mode)
	{
	  case 1:  // inverse
	    op.push_back('\'');
	    break;
	  case 2:  // double
	    op.push_back('2');
	    break;
	  case 3:
	    op.push_back('|'); // middle side 
	    break;
	  case 4:
	    op.append("||");
	    break;
	  default:
	    ; // SKIP
	}
	Sidemarks next=next_pos+op;
	if(next==To)
	{
	  path.erase(path.begin());
	  return path+op;
	}
	Trace.push_back(state(next,path+op));
      }
    }
    Trace.pop_front();
  }
}

std::string auxiliary::mirror(const std::string& a, const char& c)
{
    static const String facets="FURBDL";
    static String Plane[3];
    Plane['X'-'X']="RL";
    Plane['Y'-'X']="UD";
    Plane['Z'-'X']="FB";
    bool cuttingSide[6];
    for(int i=0;i<6;++i)
    {
        cuttingSide[i]=(Plane[c-'X'].find(facets[i])==String::npos);
    }
    String Result;
    for(String::const_iterator it=a.begin();it!=a.end();++it)
    {
        String add, side;
	const int low_case=IS_LOWCASE(*it);
        const int pos= facets.find(UPCASE(*it));
        const char I = *it;
        if(cuttingSide[pos])
        {
            side.push_back(I);
        }
        else
        {
            side=facets[(pos+3)%6]; /* opposite side */
        }
        side[0]|=low_case;
        if(it+1!=a.end())
        {
            switch(*(it+1))
            {
            case '\'':
                add=side;
                ++it;
                break;

            case '|':
                add.push_back(I);
                add.push_back('|');
                ++it;
                break;

            case '2':
                add=side+"2";
                ++it;
                break;

            default:
                add=side+"'";
            }
        }
        else
        {
            add=side+"'";
        }
        Result+=add;
    }
    return Result;
}

String auxiliary::permute(String P)
{
  String Result;
  std::sort(P.begin(), P.end());
  do {
    Result+=P+' ';
  } while(std::next_permutation(P.begin(),P.end()));
  TRIM(Result)
  return Result;
}

void auxiliary::cryptText(const String& Key1, const String& Key2, String& Text)
{
  if(Key1.length()!=Key2.length())
  {
    return;
  }
  FOR_STR(Text, t)
  {
    const int pos=Key1.find(*t);
    if(pos==STR_END)
    {
      continue;
    }
    else
    {
      *t=Key2[pos];
    }
  }
}

void auxiliary::drawBarLine(const int& bar, const int& barLength)
{
  static const char boringMarks[]="\\|/-";
  int length=(bar/12)%barLength;
  String BAR=" [";
  for(int i=0;i<length;++i) 
  {
    BAR.push_back('=');
  }
  BAR.push_back(boringMarks[bar%4]);
  for(int i=length;i<=barLength;++i)
  {
    BAR.push_back('-');
  }
  BAR.push_back(']');
  OUT('\r'<<BAR);
}
