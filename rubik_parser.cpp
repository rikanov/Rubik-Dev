#include"rubik.h"

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
  
    //=======================================//
   //  *** Make a new list from input  ***  //
  //=======================================//
  else if(read_in=="list")
  {
    read_in=list(IS);
  }
  
    //=========================================//
   //  *** LISP command PROGN for blocks ***  //
  //=========================================//
  else if(read_in=="progn")
  {
    while(IS.good())
    {
      read_in=parser(IS);
    }
  }

    //=====================================//
   //  *** Return constant variables ***  //
  //=====================================//  
  else if(read_in==TRUE || read_in==NIL || Sidemarks(read_in).valid())
  {
    SKIP
  }
  
    //====================================//
   //  *** LISP equality predicates ***  //
  //====================================//
  else if(read_in=="nil")
  {
    read_in= parser(IS)==NIL ? TRUE : NIL;
  }
  else if(read_in=="eq")
  {
    read_in=variableEquality(IS);
  }
  else if(read_in=="equalp")
  {
    read_in=parsingEquality(IS);
  }
  
    //========================================//
   //  *** User declarations, variables ***  //
  //========================================//
  else if(read_in=="defun")
  {
    IS>>read_in;
    Var_space[read_in]=read_in+'&';
    read_in=defvar(IS,read_in+'&');
  }
  else if (read_in=="defvar"||read_in=="define")
  {
    IS>>read_in;
    read_in=defvar(IS,read_in);
  }
  else if (read_in=="clone")
  {
    String value;
    IS>>read_in;
    IS>>value;
    Var_space[read_in]=Var_space.at(value);
  }
  else if (read_in=="get_value")
  {
    IS>>read_in;
    Var_space[read_in]=parser(IS);
  }
  else if (read_in=="echo")
  {
    read_in=echo(IS);
  }
    
    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  else if(read_in=="%file_open")
  {
    read_in=file_open(IS);
  }
  
    //=======================================//
   //  *** Convert numeric data to SM  ***  //
  //=======================================//
  else if(isdigit(read_in.front()))
  {
    read_in=Sidemarks(stoi(read_in));
  }

    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  else if(read_in=="mapcar")
  {
    read_in=mapcar(IS);
  }
  else if(read_in=="assoc")
  {
    read_in=assoc(IS);
  }
  else if(read_in=="what_is")
  {
    Sidemarks S(parser(IS));
    read_in=S.valid() ? whatIs(S) : "";
  }
  else if(read_in=="where_is")
  {
    Sidemarks S(parser(IS));
    read_in=S.valid() ? locationOf(S) : "";
  }
  else if(read_in=="select" )
  {
    read_in=select(IS,false);
  }
  else if(read_in=="deselect" )
  {
    read_in=select(IS,true);
  }
  else if(read_in=="is_solved?")
  {
    if(!is_solved(A_map,NumberOfSideMarks))
    {
      read_in="";
    }
  }
  else if(read_in=="cube")
  {
    read_in=cube(IS);
  }
    
    //=======================================//
   //  *** Line interpreter functions ***   //
  //=======================================//
  else if(read_in=="merge")
  {
    read_in=merge(IS);
  }
  else if(read_in=="path_finder")
  {
    read_in=pathFinder(IS);
  }
  else if(read_in=="brute_force")
  {
    read_in=callBruteForce(IS);
  }
  
    //==========================================//
   //  *** Evaluate side-effect functions ***  //
  //==========================================// 
  else if(read_in=="print")
  {
    print(parser(IS));
  } 
  else if(read_in=="do")
  {
    (*this) << parser(IS);
  }
  else if(read_in=="suppose")
  {
    suppose(parser(IS));
  }
  else if(read_in=="no_suppose")
  {
    noSuppose();
  }
  else if(read_in=="align")
  {
   read_in=setAlign(IS);
  }
  else if(read_in=="side_marks")
  {
    printSidemarks(IS);
  }
  
    //=========================================//
   //  *** Return user-defined variables ***  //
  //=========================================//  
  else if(Var_space.find(read_in)!=Var_space.end())
  {
    read_in=variable(IS,read_in);
  }
  
    //==========================================//
   //  *** Return symbol without evaluate ***  //
  //==========================================//  
  else if(IS.good())
  {
    read_in;
  }
  nil_return(read_in);
}
