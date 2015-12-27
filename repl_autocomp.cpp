#include "rubik.h"

char ** Rubik::commandCompletion(const char * stem_text,int start,int end) 
{
  char ** matches = NULL;
  matches = rl_completion_matches(stem_text,getCommands);
  return matches ;
}
 
char * Rubik::getCommands (const char * stem_text,int state) 
{
  static std::map<String,String>::const_iterator cit;
  if(state==0)
  {
    cit=Var_space->begin();
  }
  const int len=strlen(stem_text);
  while(cit!=Var_space->end())
  {
    if(strncmp ((cit->first).c_str(),stem_text,len) == 0) 
    {
      return strdup ((cit++)->first.c_str()) ;
    }
    ++cit;
  }
  return nullptr;
}
