#include "rubik.h"


void Rubik_BF::Cluster::clusterInit(const int& dim, const CubeSlot * solved_state)
{
  found=nullptr;
  if(dim==0)
  {
    cluster_size=0;
    HeuristicIndices=nullptr;
    ClusterDimensions=nullptr;
    ClusteredSolutions=nullptr;
    return;
  } 
  OUT("caching heuristical ends... ")
  Dim=dim; 
  HeuristicIndices=new CubeSlot[dim];
  for(int i=0; i<dim; ++i)
  {
    HeuristicIndices[i]=solved_state[i]>0 ? solved_state[i] : 0; 
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
  for(const Topology::t_state * state_pointer=Topology::getTrace()+1; state_pointer->state!=nullptr; ++state_pointer)
  {
    ++ClusterDimensions[indexOf(state_pointer)];
  }
  ClusteredSolutions=new const Topology::t_state** [cluster_size]; OUT_(NL<<cluster_size)
  int usage=0;
  for(int i=0;i<cluster_size;++i)
  {
    ClusteredSolutions[i]= ClusterDimensions[i]==0 ? nullptr : new const Topology::t_state* [ClusterDimensions[i]];
    if(ClusterDimensions[i])
    {
      ++usage;
    }
    ClusterDimensions[i]=0; 
  } 
  OUT_(usage)
  for(const Topology::t_state * state_pointer=Topology::getTrace()+1; state_pointer->state!=nullptr; ++state_pointer)
  {
    const int index=indexOf(state_pointer);
    ClusteredSolutions[index][ClusterDimensions[index]]=state_pointer;
    ++ClusterDimensions[index];
  }
  OUT_("DONE")
}

int Rubik_BF::Cluster::indexOf(const Topology::t_state *Rot)
{
  int multiplier=1;
  int result=0; 
  for(int i=0; i<Dim; ++i, multiplier*=24)
  {
    const Sidemarks sm(Rot->i_state[HeuristicIndices[i]]);
    result+=multiplier*sm.getPivot();
  }
  return result;
}

Rubik_BF::Cluster::~Cluster()
{
  for(int i=0;i<cluster_size;++i)
  {
    delete ClusteredSolutions[i];
  }
  delete[] ClusterDimensions; 
  delete[] ClusteredSolutions; 
  delete[] HeuristicIndices; 
}
