#include "rubik.h"

char ** Rubik::commandCompletion(const char * stem_text,int start,int end) 
{
  char ** matches = NULL;
  matches = rl_completion_matches(stem_text,getCommands);
  return matches ;
}
 
char * Rubik::getCommands (const char * stem_text,int state) 
{
  static std::list<const char*>::const_iterator cit;
  if(state==0)
  {
    cit=InnerCommands->begin();
  }
  const int len=strlen(stem_text);
  while(cit!=InnerCommands->end())
  {
    if(strncmp (*cit,stem_text,len) == 0) 
    {
      return strdup (*(cit++)) ;
    }
    ++cit;
  }
  return nullptr;
}
