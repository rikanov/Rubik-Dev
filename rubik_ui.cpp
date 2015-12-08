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
  OS<<"\nREPL mode has been closed.";
}

String Rubik::parser(std::istream & IS)
{
  String read_in;
  IS>>read_in;
  
    //=======================================//
   //  *** Evaluate built-in constants ***  //
  //=======================================//
  if(read_in=="all" || read_in=="All" || read_in=="ALL")
  {
    return Topology::SideMarks;
  }
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  if(read_in=="echo" || read_in=="print")
  {
    print(parser(IS));
  } 
  else if (read_in=="list")
  {
    print(Topology::SideMarks);
  }
  else if(read_in=="do")
  {
    (*this) << parser(IS);
  }
  return read_in;
}
