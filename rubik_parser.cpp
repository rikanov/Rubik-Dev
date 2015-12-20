#include"rubik.h"

#define trigger(X,Y) else if(read_in==X) {read_in=Y(IS);}
#define trigger_v(X,Y) else if(read_in==X) {Y();}
#define trigger_p(X,Y) else if(read_in==X) {read_in=Y(parser(IS));}
#define trigger_pv(X,Y) else if(read_in==X) {Y(parser(IS));}

String Rubik::parser(Stream & IS)
{ 
  String read_in;
  
  IS>>read_in;
  
    //=======================================//
   //  *** Self-evaluate EXEC function ***  //
  //=======================================//
  while(read_in=="exec")
  {
    auxiliary::imbueStream(IS,read_in=parser(IS));
  }
  
    //===========================================//
   //  *** Catch the whole line in one line *** //
  //===========================================//
  if(read_in.front()=='#')
  {
    getline(IS,read_in);
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
  else if(read_in==TRUE || read_in==NIL || Sidemarks(read_in).valid())
  {
    SKIP
  }
    
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
  
  trigger("nil",        nilEquality)
  trigger("eq",    variableEquality)
  trigger("equalp", parsingEquality)
  
    //========================================//
   //  *** User declarations, variables ***  //
  //========================================//
  
  trigger("defun",   defun)
  trigger("defvar", defvar)
  trigger("define", defvar)
  trigger("echo",     echo)

    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  
  trigger("%exec_file",file_open)
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  
  trigger("assoc",       assoc)
  trigger("mapcar",     mapcar)
  trigger("what_is",    whatIs)
  trigger("where_is",  whereIs)
  trigger("select",     select)
  trigger("deselect", deselect)
  trigger("solvedp",   solvedp)
  trigger("cube",         cube)
    
    //=========================================//
   //  *** Seeker and auxiliary functions *** //
  //=========================================//
  
  trigger("merge",                merge)
  trigger("path_finder",     pathFinder)
  trigger("brute_force", callBruteForce)
  
    //==========================================//
   //  *** Evaluate side-effect functions ***  //
  //==========================================// 
 
  trigger   ("align",         setAlign)
  trigger_v ("no_suppose",   noSuppose)
  trigger_pv("suppose",        suppose) 
  trigger_pv("print",            print)
  trigger   ("side_marks", printSmarks)
   
    //===========================================//
   //  *** Look for user-defined variables ***  //
  //===========================================//  
  else
  {
    variable(IS,read_in);
  }
  nil_return(read_in);
}
