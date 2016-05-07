#include"globals.h"
#include"rubik.h"
#include"auxiliary.h"

void Rubik::REPL(std::istream & IS, std::ostream & OS)
{
  if(&IS==&std::cin)
  {
    OUT_(Color::red<<"\nTo log out from REPL::"<<Color::light<<Object<<Color::red<<", press Ctrl-D or Ctrl-Z on Windows systems"<<Color::gray);
    // Use tab for auto completion
    rl_bind_key ('\t', rl_complete) ;
    // Use our function for auto - complete
    rl_attempted_completion_function = commandCompletion ;
    while(true)
    {
      Stream toParse;
      char prompt[10];
      strcpy(prompt,"REPL > ");
      bool sign_nwln=false;
      String Get;
      do
      {    
	char *buf; 
	OUT(Color::white)
	if((buf=readline(prompt))==NULL)
	{
	  OUT(Color::red<<"\nREPL mode has been closed for: "<<Color::light<<Object<<Color::gray);
	  free(buf);
	  return;
	}
	if(strcmp(buf,"")!=0)
	{
	  add_history(buf);
	}
	else
	{
	  continue;
	}
	OUT(Color::gray)
	String Get(buf);
	TRIM(Get)
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
	prompt[5]='\\';
	
	free(buf);
	buf=NULL;
      }while(sign_nwln);
      if(toParse.str()=="")
      {
	continue;
      }
      while(toParse.good())
      {
	String parsed=parser(toParse)+' ';
	OUT(Color::light)
	OS<<parsed;
	OUT(Color::gray)
      }
      NL_
    }
  }
  else
  { 
    while(IS.good())
    {
      String Get;
      Stream toParse;
      bool sign_nwln=false;
      do
      {  
	getline(IS, Get);
	TRIM(Get)
	if(sign_nwln=Get.back()=='\\')
	{
	  Get.pop_back();
	}
	if(Get=="" && IS.good())
	{
	  sign_nwln=true;
	  continue;
	}
	toParse<<' '<< Get;
	
      } while(sign_nwln);
      while(toParse.good())
      {
	parser(toParse); //	OS<<parser(toParse)+' ';
      }
    }
  }
}