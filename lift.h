#include"rubik.h"
#include<set>

class OrientedPath: private std::set<String>
{
  const int Checkpoints;
  const Sidemarks Start;
  const Sidemarks End;
  int * Indices;
  int UpperBound;
  int LowerBound;
  String Path;
  
public:
  OrientedPath(const Sidemarks & Start, const Sidemarks & End, const int & Checkpoints);
  void reset();
  void iterate();
  const String & getPath() const;
};