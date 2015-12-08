#include"globals.h"
#include"rubik.h"

void Rubik::REPL(std::istream & IS, std::ostream & OS)
{
  OS<<"\nTo log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems";
  while(IS)
  {
    OS<<"\nREPL > ";
    OS<<parser(IS);
  }
  OS<<"\nREPL mode has been closed.\n";
}

String Rubik::parser(std::istream & IS)
{
  String read_in;
  IS>>read_in;
  
    //=======================================//
   //  *** Self-evaluate EXEC function ***  //
  //=======================================//
  if(read_in=="exec")
  {
    read_in=parser(IS);
  }
  
    //=======================================//
   //  *** Evaluate built-in constants ***  //
  //=======================================//
  if(read_in=="all" || read_in=="All" || read_in=="ALL")
  {
    return "FURBDL";
  }
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  if(read_in=="what_is" || read_in=="what")
  {
    return whatIs(parser(IS));
  }
  else if(read_in=="where_is" || read_in=="where")
  {
    return locationOf(parser(IS));
  }
  else if(read_in=="path_finder" || read_in=="pf")
  {
    String Result, From=parser(IS), To=parser(IS);
    while(IS && To!=";" && From!=";")
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
    return Result;
  }
  else if(read_in=="brute_force" || read_in=="bf")
  {
    String a,b;
    a=parser(IS);
    while(IS)
    {
      String i=parser(IS);
      if(i==";")
      {
	break;
      }
      b+=i+" ";
    }
    b.pop_back();
    return bruteForce(b,a);
  }
  
    //==========================================//
   //  *** Evaluate side-effect functions ***  //
  //==========================================// 
  else if(read_in=="echo" || read_in=="print")
  {
    print(parser(IS));
  } 
  else if(read_in=="list")
  {
    print(Topology::SideMarks);
  }
  else if(read_in=="do")
  {
    (*this) << parser(IS);
  }
  else if(read_in=="suppose" || read_in=="sup")
  {
    suppose(parser(IS));
  }
  else if(read_in=="no_suppose" || read_in=="nosup")
  {
    noSuppose();
  }
  return read_in;
}
