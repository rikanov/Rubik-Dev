#include"rubik_bf.h"
#include"cube_topology.h"
#include"def_colors.h"

void Rubik::printCollection() const
{
  NL_
  if(Global==this)
  {
    OUT_(Color::red<<" * "<<Color::white<<"global \t\t"<<(Global->stack_pointer)<<Color::gray)
  }
  else
  {
    OUT_(Color::gray<<"   global \t\t"<<(Global->stack_pointer))
  }
  for(std::map<String, Rubik*>::iterator it=Collection->begin();it!=Collection->end();++it)
  {
    const String P(it->second->Object);
    const String tab(P.length()>10 ? " \t" : " \t\t");
    if(P==Object)
    {
      OUT_(Color::red<<" * "<<Color::white<<P<<tab<<(it->second->stack_pointer)<<Color::gray)
    }
    else
    {
      OUT_(Color::gray<<"   "<<P<<tab<<(it->second->stack_pointer))
    }
  } 
  NL_ NL_
}

bool Rubik::is_solved(const CubeSlot* Cubes, const int & Limit) const
{ 
  const CubeSlot * b=IdentityMap;
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
  EMPTY(Act)
  Topology::defOperation(Act,findPath(A,B,false),Topology::AllCubes);
  Topology::actOn(A_map,Act);
  Topology::inverse(A_map,B_map);
  Topology::actOn(B_map,Act);
  Topology::inverse(B_map,A_map);
}

void Rubik::swapTwoPieces(const Sidemarks& A, const Sidemarks& B)
{
  Stream SA(permute(A));
  String step;
  EMPTY(temp)
  CPY_FUNC(temp,B_map)
  while(SA >> step)
  {
    String mapped(step);
    auxiliary::cryptText(A,B,mapped);
    temp[Sidemarks(mapped)]=B_map[Sidemarks(step)]; 
    if(Sidemarks::sameCubes(A,B)==false)
    {
      temp[Sidemarks(step)]=B_map[Sidemarks(mapped)]; 
    }
  }
  CPY_FUNC(B_map,temp)
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

void Rubik::setAutocomp(const char* A)
{
  (*Var_space)[A];
}

bool Rubik::variableEquality(String A, const String& B) const
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
  GETLINE(lambda) 
  GETLIST(core)  
  (*Var_space)["mapcar-lambda"]=lambda+" ";
  const bool is_lambda=lambda.find('&')!=String::npos;
  while(core.good())
  {
    String next;
    String query;
    core >> query;
    if(is_lambda)
    {
      next=functionResolver(query,"mapcar-lambda");
    }
    else
    {
      next=lambda+" "+query;
    }
    if((Inv && parser(next)==NIL) ||
      (!Inv && parser(next)!=NIL) )
    {
      Result+=query+' ';
    }
  }
  TRIM_END(Result);
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

void Rubik::setRotation(const String& Rot)
{
  if(Rot==NIL)
  {
    return;
  }
  Topology::defOperation(RotationCache,Rot); 
  NeedAlign=Topology::defOperation(RotationAlign,Rot,Topology::AllCubes, Topology::Middle);
}

Rubik& Rubik::applyRotation()
{
  Topology::actOn(A_map,RotationCache);
  Topology::inverse(A_map,B_map); 
  if(NeedAlign)
  {
    Topology::actOn(B_map, RotationAlign);
    Topology::inverse(B_map,A_map); 
  }
  
  return *this;
}

String Rubik::functionResolver(const String & arg,const String & R)
{
  return auxiliary::putInString(arg,'&',Var_space->at(R));
}

String Rubik::functionResolver(Stream& IS,const String & R)
{
  PARSER(arg); 
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

CubeletColors Rubik::pickCubelet(const int& x, const int& y, const int& z) const
{
  CubeletColors Result;
  Sidemarks cublet_place; 
  cublet_place=convertToSidemarks(x,y,z);
  if(cublet_place!=-1)
  {
    Sidemarks cublet_value(whatIs(cublet_place));
    Result = cublet_place.getColors(cublet_value);
  }
  return Result;
}

void Rubik::rotationByAxis(const int& x, const int& y, const int& z, const bool & inv)
{
  String rot=convertToSidemarks(x,y,z);
  if(inv)
  {
    rot.push_back('\'');
  }
  setRotation(rot);
  applyRotation();
}

Rubik::~Rubik()
{
  delete[] A_map, B_map, Sup_map, Sup_inv, Stack;
  if(Object=="global")
  {
    Rubik_BF::cluster->deinit();
    delete Rubik_BF::cluster;
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
