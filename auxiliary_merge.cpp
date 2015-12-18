#include"auxiliary.h"

static int modifierIssue(const char& a)
{
  switch(a)
  {
    case '\'':
      return 3;
    case '2':
      return 2;
    default:
      return 1;
  }
}

static char modifierIssue(const int& a)
{
  switch(a)
  {
    case 3:
      return '\'';
    case 2:
      return '2';
    default:
      return '\0';
  }
}

bool auxiliary::checkSimplePath(const String& S)
{
  for(std::string::const_iterator it=S.begin(); it+1!=S.end(); ++it)
  {
    if(*it==*(it+1) && *it!='|')
    {
      return false;
    }
    if(it+2!=S.end() && isletter(*it) && !isletter(*(it+1)) && *it==*(it+2))
    {
      return false;
    }
    
  }
  return true;
}
String auxiliary::mergeSimplePaths(const String & wA,const String & wB)
{
  if(wA==""||wB=="")
  {
    return wA+wB;
  }
  String A(wA), B(wB);
  const char sideB=B.front();
  B.erase(B.begin());
  const int a_op=modifierIssue(A.back());
  const int b_op= B=="" ? 1 : modifierIssue(B.front());
  const int result=(4+a_op+b_op)%4;
  if(a_op>1)
  {
    A.pop_back();
  }
  if(A.back()!=sideB)
  {
    return wA+wB;
  }
  if(b_op>1)
  {
    B.erase(B.begin());
  }
  if(result>1)
  {
    A.push_back(modifierIssue(result));
  }
  else if(result==0)
  {
    A.pop_back();
  }
  return mergeSimplePaths(A,B);
}