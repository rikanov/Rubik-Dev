#include"rubik_bf.h"

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
  BF_SUCCESS=Result.first ? std::to_string(Result.first) : "NIL";
  BF_RESULT =Result.second;
  return Result.second;
}
