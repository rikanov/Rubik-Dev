#include "globals.h"
#include "rubik.h"
#include "editor.h"
#include "visualization.h"
#define boolean(X) return (X) ? L_TRUE : NIL;

UI_rfunc(REPL)
{
  GET(object)
  if(object=="")
  {
    object=Object;
    REPL();
  }
  else if(object=="global")
  {
    Global->REPL();
  }
  else if(Collection->find(object)!=Collection->end())
  {
    (*Collection)[object]->REPL();
  }
  else
  {
    Rubik* Obj=new Rubik(object);
    Obj->REPL();
    delete Obj;
  }
  return object;
}

UI_rfunc(progn)
{
  String Result;
  while(IS.good())
  {
    Result=parser(IS);
  }
  return Result;
}

UI_rfunc(getValue)
{
  GET(fName)
  (*Var_space)[fName]=list(IS);
  return fName;
}

UI_rfunc(defun)
{
  GET(fName)
  (*Var_space)[fName]=fName+'&';
  fName.push_back('&');
  (*Var_space)[fName]="";
  while(IS.good())
  {
    GET(S)
    (*Var_space)[fName]+=S+' ';
  }
  TRIM_END((*Var_space)[fName]);
  return fName;
}

UI_rfunc(defmacro)
{
  GET(macro);
  Regex macro_syntax(MACRO_SYNTAX); // $1: macro-name $2: macro-argument
  String macro_name(FIND(macro,macro_syntax,"$1"));
  String macro_arg (FIND(macro,macro_syntax,"$2"));
  GETLINE(def)
  Stream eval("@ "+macro_arg+' '+def);
  (*Var_space)[macro_name]=stringReplace(eval);
  return macro_name;
}

UI_rfunc(defvar)
{
  GET(fName)
  String former=(*Var_space)[fName];
  (*Var_space)[fName]="";
  for(bool End=false;IS.good() && !End;)
  {
    GET(S)
    while(!S.empty() && S.back()==';')
    {
      S.pop_back();
      End=true;
    }
    if(S==fName)
    {
      S="list "+former+";";
    }
    (*Var_space)[fName]+=S+' ';
  }
  TRIM_END((*Var_space)[fName])
  return fName;
}

UI_rfunc(delete_var)
{
  GET(name)
  std::map<String, String>::iterator it=Var_space->find(name);
  if(it == Var_space->end())
  {
    return NIL;
  }
  else
  {
    Var_space->erase(it);
    return L_TRUE;
  }
}

UI_rfunc(nilEquality)
{
  return parser(IS)==NIL ? L_TRUE : NIL;
}

UI_rfunc(variableEquality)
{
  GET2(A,B)
  boolean(variableEquality(A,B))
}

UI_rfunc(listEquality)
{
  GETLIST(A)
  GETLIST(B)
  boolean(A.str()==B.str())
}

UI_rfunc(parsingEquality)
{
  PARSER2(A,B);
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

UI_rfunc(condition)
{
  GETLINE(A) 
  if(parser(A)!=NIL)
  {
    return parser(IS);
  }
  GETLINE(rest_to_throw)
  return NIL;
}

UI_rfunc(forLoop)
{
  String Result;
  GET(token)
  GETLIST(ForIn)
  OUTSPREAD(core)
  String Next;
  while(ForIn>>Next)
  {
    (*Var_space)[token]=Next;
    Stream eval(core);
    Result+=parser(eval)+' ';
    if(!ForIn.good())
    {
      IS.str(eval.str());
    }
  }
  TRIM_END(Result)
  return Result;
}

UI_rfunc(regExp)
{
  using std::regex_error;
  PARSER2(Reg,Test);
  try
  {
    boolean(MATCH(Test,Regex(Reg)))
  }
  catch (regex_error e) 
  {
     OUT_(e.what())
     OUT_("CODE IS: " << e.code())
     return NIL;
  } 
}

UI_rfunc(regExpFast)
{
  PARSER2(pattern, word);
  String Simplifier;
  boolean(auxiliary::regExp(regSimplifier(pattern,Simplifier).c_str(),word.c_str()))
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
    Result=L_TRUE;
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
    Result=L_TRUE;
  }
  return Result;
}
UI_rfunc(what_is)
{
  Sidemarks S(parser(IS));
  return S.valid() ? whatIs(S) : NIL;
}

UI_rfunc(where_is)
{
  Sidemarks S(parser(IS));
  return S.valid() ? locationOf(S) : NIL;
}

UI_rfunc(solvedp)
{
  boolean(is_solved());
}

UI_rfunc(doRotations)
{
  static const int InfiniteLoopLimit=2000;
  int counter=0;
  GETLINE(core)
  GETLINE(UNTIL);
  Stream test;
  if(core!=NIL)
  {
    String DO;
    do
    {  
      if(++counter > InfiniteLoopLimit)
      {
	OUT_("ERROR: the number of steps exceeded the InfiniteLoopLimit (up to "<<InfiniteLoopLimit<<") number")
	OUT_("The loop has been automatically interrupted")
	break;
      }
      DO=parser(core);
      if(DO!=NIL)
      {
	setRotation(DO); 
	applyRotation();
      }
      LOOP_STACK=mergeSimplePaths(LOOP_STACK,DO);
      test.str(UNTIL);
      test.clear();
    }while (parser(test)!=NIL);
  } 
  return std::to_string(counter);
}
  
UI_rfunc(assoc)
{
  String A=parser(IS);
  String B=parser(IS);
  return (A=="" || B=="") ? A+B : A==B ? A : A+"->"+B;
}

UI_rfunc(echo)
{
  GET(read_in)
  std::map<String,String>::const_iterator it=Var_space->find(read_in);
  return (it!=Var_space->end() ? it->second : read_in);
}

UI_rfunc(catFiles)
{
  std::fstream f; 
  GET(fileName)
  f.open(fileName.c_str(), std::ios_base::in);     // open file for reading
  const bool t=f.good();
  if (t) 
  {
    String tmp;     
    do
    {     
      getline(f,tmp); 
      OUT_(tmp);
    } while (!f.eof());
  } 
  return t ? L_TRUE : NIL;
}

UI_rfunc(saveCube)
{
  GET(file_name)
  std::ofstream S; 
  S.open(file_name, std::ios::out); 
  for(int i=0; i<78; ++i)
  {
    S<<A_map[i]<<' ';
  }
  S<<NL;
  S.close();
  return NIL;
}

UI_rfunc(loadCube)
{
  GET(file_name)
  std::ifstream S; 
  S.open(file_name, std::ios::in); 
  String numericData;
  CubeSlot * pointer=A_map;
  while(S>>numericData)
  {
    *(pointer++) = atoi(numericData.c_str()); 
  }
  Topology::inverse(A_map, B_map);
  S.close();
  return NIL;
}

UI_rfunc(editor)
{
  GET(file_name)
  Editor ed(file_name);
    
  initscr();            // Start ncurses mode
  noecho();             // Don't echo keystrokes
  cbreak();             // Disable line buffering
  keypad(stdscr, true); // Enable special keys to be recorded
  
  while(ed.getMode() != 'x')
  {
    ed.updateStatus();
    ed.printStatusLine();
    ed.printBuff();
    int input = getch();        
    ed.handleInput(input);
  }    
  refresh();                     
  endwin(); 
  return file_name;
}

UI_rfunc(conc)
{
  GET2(A,B)
  return A+B;
}

UI_rfunc(stringReplace)
{
  GET2(arg,lambda)
  GETLINE(pattern)
  Regex R(lambda);
  return REPLACE(pattern,R,arg)
}

UI_rfunc(list)
{
  String Result,read_in;
  GETLINE(L)
  Stream buffer(L);
  while(buffer.good())
  {
    Result+=parser(buffer);
    Result.push_back(' ');
  }
  TRIM_END(Result);
  return Result;
}

UI_rfunc(mapcar)
{
  String Result, read_in;
  GET(lambda)
  if((*Var_space)[lambda]==String(lambda+'&'))
  {
    lambda.push_back('&');
  }
  Stream buffer(list(IS));
  while(buffer>>read_in)
  {
    Stream evaluate(lambda+' '+read_in);
    Result+=parser(evaluate)+' ';
  }
  TRIM_END(Result)
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
  String Result;
  GETLINE(L)
  CALL_BACK(SS,list,L)
  bool middle=false;
  String From, To;
  SS>>From; 
  if(From=="-ext")
  {
    middle=true;
    SS>>From;
  }
  while(SS>>To)
  {
    Result+=findPath(From,To,middle);
    From=To;
  }
  return Result;
}

UI_rfunc(permute)
{
  GET(A)
  return auxiliary::permute(A);
}

UI_rfunc(crypt)
{
  GET2(A,B)
  if(auxiliary::regExp("?*->?*",A.c_str())==false)
  {
    return A;
  }
  const int split=A.find("->");
  auxiliary::cryptText(A.substr(0,split),A.substr(split+2),B);
  return B;
}

UI_rfunc(swap)
{
  String Success=L_TRUE;
  GETLIST(Pieces)
  String Start;
  Pieces >> Start;
  String To;
  while(Pieces >> To)
  {   
    const Sidemarks from(Start), to(To);
    if(Topology::getEigenvalue(from)==Topology::getEigenvalue(to))
    {
      swapTwoPieces(from,to);
    }
    else
    {
      Success=NIL;
      break;
    }
  }
  return Success;
}

UI_rfunc(sameCubes)
{
  PARSER2(A,B)
  boolean(Sidemarks::sameCubes(A,B))
}
UI_rfunc(merge) 
{
  String A=parser(IS),B=parser(IS);
  return mergeSimplePaths(A,B);
}

UI_rfunc(mirror)
{
  PARSER2(RotC, Plane)
  return auxiliary::mirror(RotC,UPCASE(Plane.at(0)));
}

UI_rfunc(inverse)
{
  PARSER(A)
  return auxiliary::inverse(A);
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
  TRIM_END(Result)
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
    LOOP_STACK+=("["+Result+"]");
    align(A,B);
  }
  return Result;
}

UI_rfunc(callBruteForce)
{
  String As=parser(IS);
  GETLINE(results_list)
  CALL_BACK(results_stream,list,results_list);
  if(results_stream.str()==NIL)
  {
    return NIL;
  }
  return bruteForce(results_stream,As);
}

UI_rfunc(random)
{
  return random();
}

UI_rfunc(file_open)
{
  PARSER(F)
  return file_open(F.c_str());
}

UI_rfunc(init)
{
  GET(A);
  int size=atoi(A.c_str());
  if(size<2 || size>7)
  {
    size=2;
  } 
  Topology::initCache(size);
  (*Var_space)["cache-level"]=A;
  return "init";
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

UI_rfunc(readCache)
{
  GET(A)
  const int index =atoi(A.c_str()); 
  const bool validIndex= (index>=0 && index<Topology::getTraceSize());
  return validIndex ? Topology::cachePoint(index) : NIL;
}

UI_rfunc(show3D)
{
  int myargc=1;
  char * myargv[1];
  myargv[0]=strdup("Visualization");
  RUBIK_3D::visualization(this,myargc,myargv);
  return "visualization";
}