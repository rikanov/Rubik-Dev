#include"globals.h"
#include"rubik.h"

#define boolean(X) return (X) ? TRUE : NIL;
UI_rfunc(progn)
{
  String Result;
  while(IS.good())
    {
      Result=parser(IS);
    }
  return Result;
}
UI_rfunc(defun)
{
  GET(fName)
  Var_space[fName]=fName+'&';
  fName.push_back('&');
  Var_space[fName]="";
  while(IS.good())
  {
    GET(S)
    Var_space[fName]+=S+(IS.good() ? " " : "");
  }
  return fName;
}

UI_rfunc(defvar)
{
  GET(fName)
  String former=Var_space[fName];
  Var_space[fName]="";
  while(IS.good())
  {
    GET(S)
    if(S==fName)
    {
      S=former;
    }
    Var_space[fName]+=S+(IS.good() ? " " : "");
  }
  return fName;
}

UI_rfunc(nilEquality)
{
  return parser(IS)==NIL ? TRUE : NIL;
}

UI_rfunc(variableEquality)
{
  GET2(A,B)
  boolean(variableEquality(A,B))
}

UI_rfunc(parsingEquality)
{
  GET2(A,B);
  Stream sA(A); Stream sB(B);
  A=list(sA);
  B=(B=="list") ? list(IS) : list(sB);
  boolean(A==B)
}

UI_rfunc(logicalOr)
{
  PARSER2(A,B)
  boolean(A!=NIL || B!=NIL)
}

UI_rfunc(logicalXor)
{
  PARSER2(A,B)
  boolean((A!=NIL) != (B!=NIL))
}

UI_rfunc(logicalAnd)
{
  PARSER2(A,B)
  boolean(A!=NIL && B!=NIL)
}

UI_rfunc(regExp)
{
  GET2(R,S);
  String Simplified;
  boolean(auxiliary::regExp(auxiliary::regSimplifier(R,Simplified).c_str(),S.c_str()))
}
UI_rfunc(store)
{
  String Result;
  if(stack_pointer==MaximumStackDepth)
  {
    Result=NIL;
  }
  else
  {
    CPY_FUNC(Stack[stack_pointer++],A_map)
    Result=TRUE;
  }
  return Result;
}

UI_rfunc(revert)
{
  String Result;
  if(stack_pointer==0)
  {
    Result=NIL;
  }
  else
  {
    CPY_FUNC(A_map,Stack[--stack_pointer])
    Topology::inverse(A_map, B_map);
    Result=TRUE;
  }
  return Result;
}
UI_rfunc(whatIs)
{
  Sidemarks S(parser(IS));
  return S.valid() ? whatIs(S) : NIL;
}

UI_rfunc(whereIs)
{
  Sidemarks S(parser(IS));
  return S.valid() ? locationOf(S) : NIL;
}

UI_rfunc(solvedp)
{
  // TODO
}

UI_rfunc(doRotations)
{
  (*this) << parser(IS);
  return "do";
}
  
UI_rfunc(assoc)
{
  String A=parser(IS);
  String B=parser(IS);
  return (A=="" || B=="") ? A+B : A==B ? A : A+"->"+B;
}

UI_rfunc(echo)
{
  String read_in;
  IS>>read_in;
  std::map<String,String>::const_iterator it=Var_space.find(read_in);
  return (it!=Var_space.end() ? it->second : read_in)+' ';
}

UI_rfunc(conc)
{
  String A,B;
  IS>>A,IS>>B;
  return A+B;
}
UI_rfunc(list)
{
  String Result,read_in;
  while(IS.good())
  {
    Result+=parser(IS);
    if(IS.good())
    {
      Result.push_back(' ');
    }
  }
  return Result;
}

UI_rfunc(mapcar)
{
  String Result, read_in;
  String lambda;
  IS>>lambda;
  if(Var_space[lambda]==String(lambda+'&'))
  {
    lambda.push_back('&');
  }
  Stream buffer(list(IS));
  while(buffer>>read_in)
  {
    Stream evaluate(lambda+' '+read_in);
    Result+=parser(evaluate)+' ';
  }
  return Result;
}

UI_rfunc(select)
{
  String Result;
  select(IS,Result,false);
  return Result;
}

UI_rfunc(deselect)
{
  String Result;
  select(IS,Result,true);
  return Result;
}

UI_rfunc(pathFinder)
{
  String Result, From=parser(IS), To=parser(IS), last_tag;
  while(IS.good())
  {
    String segment=findPath(From,To);
    if(segment=="" && From != To)
    {
      Result="";
      break;
    }
    Result+=segment;
    From=To;
    To=parser(IS);
  }
  last_tag=findPath(From,To);
  return last_tag=="" ? NIL : Result+findPath(From,To);
}

UI_rfunc(merge) 
{
  String A=parser(IS),B=parser(IS);
  return mergeSimplePaths(A,B);
}

UI_rfunc(cube)
{
  String Result;
  C_EACH_FUNC(B_map,b,index)
  {
    Stream S;
    S<<Sidemarks(index).c_str()<<' '<<Sidemarks(*b).c_str();
    Result+=assoc(S)+' ';
  }
  CUT_END(Result)
  return Result;
}

UI_rfunc(setAlign)
{
  String Result="";
  String A=parser(IS);
  String B=parser(IS);
  if(Sidemarks(A).type()==2 && Sidemarks(B).type()==2)
  {
    Result=A+"->"+B; // assoc(A,B)
    align(A,B);
  }
  return Result;
}

UI_rfunc(callBruteForce)
{
  String As=parser(IS);
  String Result;
  while(IS.good())
  {
    Result+=parser(IS)+' ';
  }
  return bruteForce(Result,As);
}

UI_rfunc(file_open)
{
  String F;
  IS >> F;
  file_open(F.c_str());
  return F;
}

UI_rfunc(printSmarks)
{
  while(IS.good())
  {
    Sidemarks S= parser(IS);
    if(!S.valid())
    {
      continue;
    }
    OUT(S<<':'<<S.getEigenvalue()<<' ');
  }
  NL_
  return "side_marks";
}
