#include"rubik.h"

String Rubik::parser(Stream & IS, const String arg)
{  
  String read_in;
  
    //========================================//
   //  *** Replacing function arguments ***  //
  //========================================// 
  if(arg!="")
  {
    getline(IS,read_in);
    replaceArguments(read_in, arg);
    IS.str(read_in);
    IS.clear();
    read_in=parser(IS);
  }
  
  IS>>read_in;
  
    //=======================================//
   //  *** Self-evaluate EXEC function ***  //
  //=======================================//
  if(read_in=="exec")
  {
    read_in=echo(IS);
    IS.str("");
    IS.clear();
    IS<<read_in;
    read_in=parser(IS);
  }  
  
  if(read_in.front()=='\'')
  {
    read_in.erase(read_in.begin());
    read_in+=' '+parser(IS);
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
    String F;
    IS >> F;
    std::ifstream ifs(F,std::ifstream::in);
    if(ifs.is_open())
    {
      REPL(ifs,std::cout);
    }
    else
    {
      OUT_(NL<<"Something went wrong. Unable to open the file: "<<F)
    }
    OUT_(NL<<"The file "<<F<<" has been closed.\n");
    ifs.close();
  }
  
    //=======================================//
   //  *** Convert numeric data to SM  ***  //
  //=======================================//
  else if(isdigit(read_in.front()))
  {
    read_in=Sidemarks(stoi(read_in));
  }

    //=======================================//
   //  *** Handle former input/output  ***  //
  //=======================================//
  else if(read_in=="%i"||read_in=="%o")
  {
    read_in=lastInput(read_in.back());
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
    align(parser(IS),parser(IS));
  }
  else if(read_in=="side_marks")
  {
    printSidemarks(IS);
  }
  
    //=========================================//
   //  *** Return user-defined variables ***  //
  //=========================================//  
  else if(Sidemarks(read_in).valid())
  {
    SKIP
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
  else if(read_in.back()=='!')
  {
    read_in.pop_back();
  }
  return read_in;
}
