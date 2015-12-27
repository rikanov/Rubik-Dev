#include"lift.h"

OrientedPath::OrientedPath(const Sidemarks& Start, const Sidemarks& End, const int & Checkpoints):
 Start(Start),
 End(End),
 Checkpoints(Checkpoints),
 Indices(new int[Checkpoints])
{
  static const int FirstEdgeIndex=Topology::getIndex(0,1,2);
  static const int FirstCornerIndex=Topology::getIndex(1,2,3);
  UpperBound=Start.getEigenvalue() ? NumberOfSideMarks : FirstCornerIndex;
  LowerBound=Start.getEigenvalue() ? FirstCornerIndex : FirstEdgeIndex; 
  //used to be some memset() shit here
  for(int i=0;i<Checkpoints;++i)
  {
    Indices[i]=LowerBound;
  }
}

void OrientedPath::iterate()
{
  static int bound=  1000;
  if(bound-- && auxiliary::nestedLoop(Indices,Checkpoints,UpperBound,LowerBound))
  {
    Sidemarks From=Start;
    String Add;
    const int *next=Indices;
    for(int i=0;i<Checkpoints;++i,++next)
    {
      const String seq=auxiliary::findPath(From,*next,true);
      if(seq=="" && From!=*next)
      {
	iterate();
	return;
      }
      Add=auxiliary::mergeSimplePaths(Add,seq);
      From=*next;
    }
    Add=auxiliary::mergeSimplePaths(Add,auxiliary::findPath(From,End,true));
    insert(Add);
    iterate();
  }
}

const std::string& OrientedPath::getPath() const
{
  return Path;
}
