#include<readline/readline.h>
#include<readline/history.h>
#include"globals.h"
#include"rubik.h"

void Rubik::REPL(std::istream & IS, std::ostream & OS)
{
  if(IS==std::cin)
  {
    OUT_("\nTo log out from REPL, press Ctrl-D or Ctrl-Z on Windows systems");
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
	if((buf=readline(prompt))==NULL)
	{
	  OUT_("\nREPL mode has been closed.\n");
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
	String Get(buf);
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
	prompt[5]='\\';
	
	free(buf);
	buf=NULL;
      }while(sign_nwln);
      OS<<parser(toParse);
      NL_
    }
  }
  else
  {  OUT_("teszt")
    while(IS.good())
    {
      String Get;
      Stream toParse;
      bool sign_nwln=false;
      do
      {  
	getline(IS, Get);
	while(Get.back()==' '|| Get.back()=='\t')
	{
	  Get.pop_back();
	}
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
      OS<<NL;
      OS<<(parser(toParse));
    }
  }
}
