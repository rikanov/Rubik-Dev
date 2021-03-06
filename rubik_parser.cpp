#include"rubik.h"
#include"def_colors.h"

#define trigger(X,Y) else if(read_in==X) {read_in=Y(IS);}
#define trigger_v(X,Y) else if(read_in==X) {Y();}
#define trigger_p(X,Y) else if(read_in==X) {read_in=Y(parser(IS));}
#define trigger_pv(X,Y) else if(read_in==X) {Y(parser(IS));}
#define nil_return(X) return(((X)=="" || (X)=="NIL") ? NIL : (X))


UI_rfunc(chainParser)
{
  String Result;
  while(IS.good())
  {
    Result=parser(IS);
  }
  return Result;
}

UI_rfunc(parser)
{ 
  GET(read_in)
  TRIM_END(read_in);
  const String not_defined=read_in;
  
    //=======================================//
   //  *** Self-evaluate EXEC function ***  //
  //=======================================//
  while(read_in=="exec")
  {
    auxiliary::imbueStream(IS,read_in=parser(IS));
  }
  
    //=============================================//
   //  *** Catch the whole line by one string *** //
  //=============================================//
  if(read_in.front()=='#')
  {
    GETLINE(print_out)
    OUT_(Color::green<<print_out<<Color::gray)
    read_in=NIL;
  }
  
    //=======================================//
   //  *** S-expressions, no evaluate  ***  //
  //=======================================//
  else if(read_in.front()=='\'')
  {
    read_in.erase(read_in.begin());
//     read_in+=' '+parser(IS);
  }

    //=====================================//
   //  *** Return constant variables ***  //
  //=====================================//  
  else if(read_in==L_TRUE || read_in==NIL || Sidemarks(read_in).valid())
  {
    // SKIP
  }
    //===========================================//
   //  *** Evaluate arithmetic expressions ***  //
  //===========================================//  
  else if(is_number(read_in.front()))
  {
    // SKIP
  }
  else if(arithmeticExpression(read_in))
  {
    PARSER2(A,B)
    const int NumA=atoi(A.c_str());
    const int NumB=atoi(B.c_str());
    read_in=std::to_string(evalArithmeticExp(read_in.front(),NumA,NumB));
  }
    //=====================================//
   //  *** New Rubik's cube instance ***  //
  //=====================================//
  
  trigger("REPL", REPL)
  trigger("init", init)
  
    //=================================//
   //  *** Collection management ***  //
  //=================================//
  
  trigger_v("collection_view", printCollection)
  
    //=======================================//
   //  *** Make a new list from input  ***  //
  //=======================================//
  
  trigger("list",list)
  
    //=========================================//
   //  *** LISP command PROGN for blocks ***  //
  //=========================================//
  
  trigger("progn",progn)

    //====================================//
   //  *** LISP equality predicates ***  //
  //====================================//
  
  trigger("nil",     nilEquality)
  trigger("eq", variableEquality)
  trigger("eql",    listEquality)
  trigger("eqp", parsingEquality)
  trigger("or",        logicalOr)
  trigger("xor",      logicalXor)
  trigger("and",      logicalAnd) 
  
    //========================================//
   //  *** User-space functions, macros ***  //
  //========================================//
  
  trigger("get",      getValue)
  trigger("defun",       defun)
  trigger("defvar",     defvar)
  trigger("define",     defvar)
  trigger("defmacro", defmacro)
  trigger("delete", delete_var)
  trigger("echo",         echo)
  trigger("print",       print)

    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  
  trigger("%exec_file",file_open)
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  
  trigger("if",          condition)
  trigger("for_in",        forLoop)
  trigger("assoc",           assoc)
  trigger("mapcar",         mapcar)
  trigger("convoke",       convoke)
  trigger("select",         select)
  trigger("deselect",     deselect)
  trigger("conc",             conc)
  trigger("replace", stringReplace)
  trigger("regex",          regExp)
  trigger("reg_fast",   regExpFast)
  
    //=========================================//
   //  *** Seeker and auxiliary functions *** //
  //=========================================//
  
  trigger("same_cubes",       sameCubes)
  trigger("merge",                merge)
  trigger("permute",            permute)
  trigger("mirror",              mirror)
  trigger("inverse",            inverse)
  trigger("reverse",            reverse)
  trigger("length",      rotationLength)
  trigger("path_finder",     pathFinder)
  trigger("heuristic",      getSolution)
  trigger("crypt",                crypt)
  trigger("swap",                  swap)
  trigger("random",              random)
  trigger("side_marks",     printSmarks)
  trigger("cache",            readCache)
  trigger("cat",               catFiles)
  trigger("load",              loadCube)
  trigger("save",              saveCube)
  trigger("edit",                editor)
  
    //=====================================//
   //  *** Evaluate object functions ***  //
  //=====================================// 
 
  trigger   ("do",         doRotations)
  trigger   ("align",         setAlign)
  trigger_v ("reset",        initRubik)
  trigger_v ("no_suppose",   noSuppose)
  trigger_pv("suppose",        suppose) 
  trigger   ("store",            store)
  trigger   ("revert",          revert)
  trigger_pv("Print",       printSides) 
  trigger   ("solvedp",        solvedp)
  trigger   ("check-cubes", checkCubes)
  trigger   ("cube",              cube)
  trigger   ("what_is",        what_is)
  trigger   ("where_is",      where_is)
  trigger   ("eigenvalue",   eigenFunc)
  
    //============================================//
   //  *** Show 3D view of the current cube ***  //
  //============================================//

  trigger("visualization",   show3D)
  
    //========================================//
   //  *** Look for matching user-space ***  //
  //========================================//  
  else
  {
    macro(IS,read_in);
    variable(IS,read_in); 
  }
  if(read_in=="no-return#")
  {
    read_in=not_defined;
  }
 
  nil_return(read_in);
}
