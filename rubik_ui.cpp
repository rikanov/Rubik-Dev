#include"globals.h"
#include"rubik.h"

static String input_last;
static String output_last;


void Rubik::REPL(std::istream & IS, std::ostream & OS)
{
  if(IS==std::cin)
  {
    OS<<"\nTo log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems";
  }
  while(IS.good())
  {
    String Get;
    Stream toParse;
    char prompt='>';
    bool sign_nwln=false;
    NL_
    do
    {   
      OS<<"REPL "<<prompt<<' ';
      getline(IS, Get);
      while(Get.back()==' '|| Get.back()=='\t')
      {
	Get.pop_back();
      }
      if(sign_nwln=Get.back()=='\\')
      {
	Get.pop_back();
      }
      if(Get=="")
      {
	sign_nwln=true;
	continue;
      }
      toParse<<' '<< Get;
      prompt='\\';
    } while(sign_nwln);
    OS<<(output_last=parser(toParse));
    if(Get.find("%i")==std::string::npos)
    {
      input_last=Get;
    }
  }
  if(IS==std::cin)
  {
    OS<<"\nREPL mode has been closed.\n";
  }
}

String Rubik::echo(Stream& IS)
{
  String result,read_in;
    while(IS.good())
    {
      IS>>read_in;
      if(read_in=="%i")
      {
	result+=input_last+' ';
	continue;
      }
      if(read_in=="%o")
      {
	result+=output_last+' ';
	continue;
      }
      std::map<String,String>::const_iterator it=Var_space.find(read_in);
      result+= (it!=Var_space.end() ? it->second : read_in)+' ';
    }
   return result;
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
    read_in=echo(IS);
    IS.str("");
    IS.clear();
    IS<<read_in;
    read_in=parser(IS);
  }  
  
    //========================================//
   //  *** User declarations, variables ***  //
  //========================================//
  if (read_in=="defvar")
  {
    IS>>read_in;
    Var_space[read_in]="";
    while(IS.good())
    {
      String S;
      IS>>S;
      Var_space[read_in]+=S+' ';
    }
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
  else if(read_in.back()=='!')
  {
    read_in.pop_back();
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
   //  *** Convert numeric data to SM  ***  //
  //=======================================//
  else if(isdigit(read_in.front()))
  {
    read_in=Sidemarks(stoi(read_in));
  }

    //=======================================//
   //  *** Evaluate built-in constants ***  //
  //=======================================//
  else if(read_in=="all" || read_in=="All" || read_in=="ALL")
  {
    read_in="FURBDL";
  }
  else if(read_in=="%i")
  {
    read_in=input_last;
  }
  else if(read_in=="%o")
  {
    read_in=output_last;
  }
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  else if(read_in=="what_is" || read_in=="what")
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
    while(IS.good())
    {
      Result=auxiliary::mergeSimplePaths(Result,A);
      A=parser(IS); 
    }
    read_in=Result;
  }
  else if(read_in=="path_finder" || read_in=="pf")
  {
    String Result, From=parser(IS), To=parser(IS);
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
    read_in=Result;
  }
  else if(read_in=="brute_force" || read_in=="bf")
  {
    String As=parser(IS);
    read_in="";
    while(IS.good())
    {
      read_in+=parser(IS)+' ';
    }OUT_(read_in)
    read_in=bruteForce(read_in,As);
  }
  
    //==========================================//
   //  *** Evaluate side-effect functions ***  //
  //==========================================// 
  else if(read_in=="print")
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
  else if(read_in=="side_marks" || read_in=="sm")
  {
    while(IS.good())
    {
      Sidemarks S= parser(IS);
      if(S==0&&S!="F")
      {
	continue;
      }
      OUT(S<<':'<<S.getEigenvalue()<<' ');
    }
    NL_
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
    String IS_;
    if(IS.good())
    {
      getline(IS,IS_);
    }
    IS.str("");
    IS.clear();
    IS<<Var_space.at(read_in)<<' '<<IS_;
    read_in=parser(IS);
  }
  return read_in;
}
