#include"rubik.h"
#include"cube_topology.h"

void Rubik::printCollection() const
{
  NL_
  OUT((Global==this ? " * " : "   "))
  OUT_("global \t"<<Global->stack_pointer)
  for(std::map<String, Rubik*>::iterator it=Collection->begin();it!=Collection->end();++it)
  {
    String P(it->second->Object);
    OUT((P==Object ? " * " : "   "))
    OUT_(P<<" \t"<<it->second->stack_pointer)
  } 
  NL_ NL_
}

bool Rubik::is_solved(const int* Cubes, const int & Limit) const
{ 
  const int * b=IdentityMap;
  C_EACH_FUNC(A_map,a, index)
  {
    if(index==Limit)
    {
      break;
    }
    if(*a!=*(b++))
    {
      return false;
    }
  }
  // TODO
  return true;
}

void Rubik::noSuppose()
{
  CPY_FUNC(Sup_map,IdentityMap)
  CPY_FUNC(Sup_inv,IdentityMap)
}

void Rubik::suppose(const Sidemarks& S)
{
  noSuppose();
  Topology::defOperation(Sup_inv,findPath(S,whatIs(S),false),Topology::AllCubes,Topology::NotDefined);
  Topology::inverse(Sup_inv,Sup_map);
}

void Rubik::align(const Sidemarks& A, const Sidemarks& B)
{
  EMPTY(Act) OUT_("test defOperation()")
  Topology::defOperation(Act,findPath(A,B,false),Topology::AllCubes);OUT_("done")
  Topology::actOn(A_map,Act);
  Topology::inverse(A_map,B_map);
  Topology::actOn(B_map,Act);
  Topology::inverse(B_map,A_map);
}

Sidemarks Rubik::whatIs(const Sidemarks& S) const
{
  return Sup_map[B_map[S]];
}

Sidemarks Rubik::locationOf(const Sidemarks& S) const
{
  return A_map[Sup_inv[S]];
}

Rubik& Rubik::operator<<(const String & Rot)
{
  EMPTY(Act)
  Topology::defOperation(Act,Rot);
  Topology::actOn(A_map,Act);
  Topology::inverse(A_map,B_map); 
  if(Topology::defOperation(Act,Rot,Topology::AllCubes, Topology::Middle))
  {
    Topology::actOn(B_map,Act);
    Topology::inverse(B_map,A_map);
  }
  return *this;
}

void Rubik::setAutocomp(const char* A)
{
  (*Var_space)[A];
}

bool Rubik::variableEquality(String& A, const String& B) const
{
  bool Result=false;
  if(A==B)
  {
    Result=true;
  }
  else
  {
    while(Var_space->find(A)!=Var_space->end())
    {
      A=Var_space->at(A);
      if(A==B)
      {
	Result=true;
	break;
      }
    }
  }
  return Result;
}

void Rubik::select(Stream& IS, String& Result, const bool & Inv)
{
  GET(head)
  Stream SS(parser(IS));
  while(SS.good())
  {
    Stream next;
    String query;
    SS>>query;
    next << head << ' ' << query;
    if((Inv && parser(next)==NIL) ||
      (!Inv && parser(next)!=NIL) )
    {
      Result+=query+' ';
    }
  }
  TRIM_END(Result)
}

void Rubik::macro(Stream& IS, String& R)
{
  const size_t n=R.find('(');
  if(n==STR_END)
  {
    return;
  }
  const String mnemonic=R.substr(0,n);
  const String arg=R.substr(n+1,R.size()-n-2);
  GET(input)
  String eval(auxiliary::putInString(arg,'@',input,'&',(*Var_space)[mnemonic]));
  imbueStream(IS,eval);
  R=parser(IS); 
}

void Rubik::variable(Stream& IS, String& R)
{
  if(Var_space->find(R)!=Var_space->end())
  {
    const String arg= (R.back()=='&') ? functionResolver(IS,R) : Var_space->at(R);
    auxiliary::imbueStream(IS,arg);
    R=parser(IS);
  }
}

String Rubik::functionResolver(Stream& IS,const String & R)
{
  GET(arg);
  return auxiliary::putInString(arg,'&',Var_space->at(R));
}

String Rubik::file_open(const char * F)
{
  std::ifstream ifs(F,std::ifstream::in);
  Stream OS;
  if(ifs.is_open())
  {
    REPL(ifs,OS);
  }
  else
  {
    OUT_(NL<<"Something went wrong. Unable to open the file: "<<F)
  }
  OUT_(NL<<"The file "<<F<<" has been closed.\n");
  ifs.close();
  return OS.str();
}

Rubik::~Rubik()
{
  delete[] A_map, B_map, Sup_map, Sup_inv, Stack;
  if(Object=="global")
  {
    for(std::map<String, Rubik*>::iterator it=Collection->begin();it!=Collection->end();++it)
    {
      delete it->second;
    }
    delete Collection;
    OUT_("exit with success...")
    exit(EXIT_SUCCESS);
  } 
  else
  {
    Collection->erase(Object);
  }
}