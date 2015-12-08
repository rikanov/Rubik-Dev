#include"auxiliary.h"


std::string auxiliary::mergeSimplePaths(const std::string& A, const std::string& B)
{
  String Result;
  const char last_sign=A.back();
  const char last_move= 'A'<=last_sign && last_sign<='Z' ? last_sign : A[A.length()-2];
  if(last_move!=B.front())
  {
    Result=A+B;
  }
  else if(A.back()=='\'')
  {
    if(B.length()==1)
    {
      Result=A.substr(0,A.size()-2); // X' + X = 0
    }
    else switch (B[1])
    {
      case '\'':
	Result=A.substr(0,A.size()-1)+"2"+B.substr(2); // X' + X' = X2
	break;
      case '2':
	Result=A.substr(0,A.size()-1)+B.substr(2); // X' + X2 = X
	break;
      default:
	Result=A.substr(0,A.size()-2)+B.substr(1); // X' + X = 0
    }
  }
  else if(A.back()=='2')
  {
    if(B.length()==1)
    {
      Result=A.substr(0,A.size()-1)+"'"; // X2 + X = X'
    }
    else switch(B[1])
    {
      case '\'':
	Result=A.substr(0,A.size()-1)+B.substr(2); // X2 + X' = X
	break;
      case '2':
	Result=A.substr(0,A.size()-2)+B.substr(2); // X2 + X2 = 0
	break;
      default:
	Result=A.substr(0,A.size()-1)+"'"+B.substr(1); // X2 + X = X'
    }
  }
  else
  {
    if(B.length()==1)
    {
      Result=A.substr(0,A.size()-1)+"2"; // X + X = X2
    }
    else switch(B[1])
    {
      case '\'':
	Result=A.substr(0,A.size()-1)+B.substr(2); // X + X' = 0
	break;
      case '2':
	Result=A.substr(0,A.size()-1)+"'"+B.substr(2); // X + X2 = X'
	break;
      default:
	Result=A.substr(0,A.size()-1)+"2"+B.substr(1); // X + X = X2
    }
  }
  return Result;
}