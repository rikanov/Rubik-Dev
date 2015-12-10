#include"globals.h"
#include"rubik.h"

void Rubik::REPL(std::istream & IS, std::ostream & OS)
{
  if(IS==std::cin)
  {
    OS<<"\nTo log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems";
  }
  while(IS)
  {
    OS<<"\nREPL > ";
    String Get;
    Stream toParse;
    getline(IS, Get);
    toParse << Get;
    OS<<parser(toParse);
  }
  if(IS==std::cin)
  {
    OS<<"\nREPL mode has been closed.\n";
  }
}

String Rubik::parser(Stream & IS)
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
  
    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  if(read_in=="%file_open" || read_in=="%fo")
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
   //  *** Evaluate built-in constants ***  //
  //=======================================//
  if(read_in=="all" || read_in=="All" || read_in=="ALL")
  {
    read_in="FURBDL";
  }
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  if(read_in=="what_is" || read_in=="what")
  {
    read_in=whatIs(parser(IS));
  }
  else if(read_in=="where_is" || read_in=="where")
  {
    read_in=locationOf(parser(IS));
  }
    
    //=======================================//
   //  *** Line interpreter functions ***   //
  //=======================================//
  else if(read_in=="merge" || read_in=="add")
  {
    String Result=parser(IS),A=parser(IS);
    while(IS)
    {
      Result=auxiliary::mergeSimplePaths(Result,A);
      A=parser(IS); 
    }
    read_in=Result;
  }
  else if(read_in=="path_finder" || read_in=="pf")
  {
    String Result, From=parser(IS), To=parser(IS);
    while(IS)
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
    read_in=Result;
  }
  else if(read_in=="brute_force" || read_in=="bf")
  {
    String As=parser(IS);
    read_in=bruteForce(IS,As);
  }
  
    //==========================================//
   //  *** Evaluate side-effect functions ***  //
  //==========================================// 
  else if(read_in=="echo" || read_in=="print")
  {
    print(parser(IS));
  } 
  else if(read_in=="is_solved?" || read_in=="solv?")
  {
    const bool check=is_solved(A_map,NumberOfSideMarks);
#ifndef SILENT
    OUT_((check ? "It is solved." : "It is not solved yet."))
#endif
    if(check==false)
    {
      read_in="";
    }
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
  else if(read_in=="align")
  {
    align(parser(IS),parser(IS));
  }
  return read_in;
}
