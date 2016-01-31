#include"rubik.h"

String Rubik::random()
{
  String Result;
  Result.push_back(Topology::SideMarks[1+rand()%6]);
  if(rand()%2)
  {
    Result.push_back('\'');
  }
  return Result;
}

String Rubik::bruteForce(Stream& IS, const String & AS) const
{
  Rubik_BF BF(this, IS, AS, BF_WIDTH);
  std::pair<int,String> Result=BF.start();
#ifndef SILENT
  NL_
  if(Result.first)
  {  
    if(Result.second=="")
    {
      OUT_("The condition "<<Result.first<<" is already satisfied. \nNothing to do.")
    }
    else
    {
      OUT_("The "<<Result.second << " path has been found for condition "<<Result.first );
    }
  }
  else
  {
    OUT_("Not found." );
  }
#endif
  BF_SUCCESS=Result.first ? std::to_string(Result.first) : "NIL";
  BF_RESULT =Result.second;
  return Result.second;
}
