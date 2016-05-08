#include "rubik_bf.h"
#include "def_colors.h"

Rubik_BF::Cluster Rubik_BF::cluster=Cluster();

Rubik_BF::Cluster::Cluster():
  Dim(-1),
  cluster_size(0),
  ClusterDimensions(nullptr),
  ClusteredSolutions(nullptr),
  SideClusterDimensions(nullptr),
  SideCluster(nullptr),
  HeuristicIndices(nullptr),
  found(nullptr),
  trail(nullptr)
{
  
}

bool Rubik_BF::Cluster::noChanges(const int& dim, const CubeSlot * solved_state) const
{
  bool Result=false;
  if(dim==Dim && definedLevel==Topology::cacheLevel())
  {
    int i=0;
    while(i<dim+2)
    {
      if(solved_state[i] !=HeuristicIndices[i])
      {
	break;
      }
      ++i;
    }
    Result=(i==dim+2);
  }
  return Result;
}

void Rubik_BF::Cluster::mainClusterInit(const CubeSlot * solved_state)
{
  OUT("\nMain cluster init...  ")
  HeuristicIndices=new CubeSlot[Dim+2];
  for(int i=0; i<Dim+2; ++i)
  {
    HeuristicIndices[i]=solved_state[i]; 
  }
  cluster_size=1;
  for(int i=0; i<Dim; ++i)
  {
    cluster_size*=24; // ClusterSize=24^Dim;
  }
  ClusterDimensions = new int [cluster_size];
  for(int i=0;i<cluster_size;++i)
  {
    ClusterDimensions[i]=0;
  }
  for(Topology::RotationRange range(Topology::cacheLevel()-1,Topology::cacheLevel());range.state();range.next())
  {
    ++ClusterDimensions[indexOf(range.state(),true)];
  } 
  ClusteredSolutions=new ClusterBranch [cluster_size];
  for(int i=0;i<cluster_size;++i)
  {
    ClusteredSolutions[i]= ClusterDimensions[i]==0 ? nullptr : new ClusterSprig [ClusterDimensions[i]];
    ClusterDimensions[i]=0; 
  } 
  for(Topology::RotationRange range(Topology::cacheLevel()-1,Topology::cacheLevel());range.state();range.next())
  {
    const int index=indexOf(range.state(),true);
    ClusteredSolutions[index][ClusterDimensions[index]]=range.state();
    ++ClusterDimensions[index];
  }
  OUT_(Color::white<<"done."<<Color::gray)
}

void Rubik_BF::Cluster::sideClusterInit()
{
  OUT("Side cluster init...  ")
  SideCluster = new ClusterBranch [576]; // 24*24
  SideClusterDimensions = new int [576];
  for(int i=0;i<576;++i)
  {
    ClusterDimensions[i]=0;
  }
  const int indexOfIdentity=indexOf(Topology::getTrace(), true); 
  for(int i=0;i<ClusterDimensions[indexOfIdentity];++i)
  {
    const int index=subIndexOf(ClusteredSolutions[indexOfIdentity][i],true);
    ++SideClusterDimensions[index];
  }
  for(int i=0;i<576;++i)
  {
    SideCluster[i] = SideClusterDimensions[i]==0 ? nullptr :new ClusterSprig[SideClusterDimensions[i]]; 
    SideClusterDimensions[i]=0;
  } 
  for(int i=0;i<ClusterDimensions[indexOfIdentity];++i)
  {
    const int index=subIndexOf(ClusteredSolutions[indexOfIdentity][i],true);
    SideCluster[index][SideClusterDimensions[index]]=ClusteredSolutions[indexOfIdentity][i];
    ++SideClusterDimensions[index];
  }
  OUT_(Color::white<<"done."<<Color::gray)
}

void Rubik_BF::Cluster::clusterInit(const int& dim, const CubeSlot * solved_state)
{
  found=nullptr;
  if(noChanges(dim,solved_state))
  {
    return;
  }
  deinit();
  definedLevel=Topology::cacheLevel();
  if(dim==0)
  {
    cluster_size=0;
    HeuristicIndices=nullptr;
    ClusterDimensions=nullptr;
    ClusteredSolutions=nullptr;
    SideClusterDimensions=nullptr;
    SideCluster=nullptr;
    return;
  } 
  OUT("caching heuristical ends... ")
  Dim=dim;
  mainClusterInit(solved_state);
  sideClusterInit();
  OUT_("Initialization status:"<<Color::white<<"  OK"<<Color::gray)
}

int Rubik_BF::Cluster::indexOf(const Topology::t_state *Rot, const bool & inv) const
{
  int radix=1;
  int result=0; 
  for(int i=0; i<Dim; ++i, radix*=24)
  {
    const Sidemarks sm(inv ? Rot->i_state[HeuristicIndices[i]] : Rot->state[HeuristicIndices[i]]);
    result+=radix*sm.getPivot();
  }
  return result;
}

int Rubik_BF::Cluster::indexOf(const CubeSlot * R1,const CubeSlot * R2) const
{
  int radix=1;
  int result=0; 
  for(int i=0; i<Dim; ++i, radix*=24)
  {
    const Sidemarks sm(R1[R2[HeuristicIndices[i]]]);
    result+=radix*sm.getPivot();
  }
  return result;
}

int Rubik_BF::Cluster::subIndexOf(const Topology::t_state *Rot, const bool & inv) const
{
  int result=0;
  int index;
  for(index=0;index<Dim;++index)
  {
    if(Rot->state[HeuristicIndices[index]]!=HeuristicIndices[index])
    {
      return 0;
    }
  }
  for(int i=0, radix=1; i<2; ++i, radix*=24)
  {
    const Sidemarks sm(inv ? Rot->i_state[HeuristicIndices[index+i]] : Rot->state[HeuristicIndices[index+i]]);
    result+=radix*sm.getPivot();
  }
  return result;
}

int Rubik_BF::Cluster::subIndexOf(const CubeSlot * R1, const CubeSlot * R2, const CubeSlot * R3) const
{
  int result=0;
  for(int i=Dim, radix=1; i<Dim+2; ++i, radix*=24)
  {
    const Sidemarks sm(R1[R2[R3[HeuristicIndices[i]]]]);
    result+=radix*sm.getPivot();
  }
  return result;
}

void Rubik_BF::Cluster::deinit()
{
  if(ClusteredSolutions)
  {
    for(int i=0;i<cluster_size;++i)
    {
      delete ClusteredSolutions[i];
    }
  }
  if(SideCluster)
  {
    for(int i=0;i<576;++i)
    {
      delete SideCluster[i];
    }
  }
  delete[] SideClusterDimensions;
  delete[] SideCluster;
  delete[] ClusterDimensions; 
  delete[] ClusteredSolutions; 
  delete[] HeuristicIndices; 
}
