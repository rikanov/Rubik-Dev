#ifndef RUBIK_BRUTE_FORCE
#define RUBIK_BRUTE_FORCE

#include "rubik.h"

class Rubik_BF
{
  friend class Rubik;
  typedef int (Rubik_BF::*SearchEngine)(const Topology::t_state*,const Topology::t_state*);
 
  int seekerDepth;
  int best_choice;
  bool foundBetter;
  bool useExtendedPath;
  
  const Rubik * RubikBase;
  CubeSlot * InitialState;
  CubeSlot * InvInitialState;
  CubeSlot * SolvedState;
  bool HeuristicSearch;
  String resolver(const Topology::t_state* A,const Topology::t_state* B);
  struct Cluster
  {
    typedef const Topology::t_state * ClusterSprig;
    typedef ClusterSprig * ClusterBranch;
    typedef ClusterBranch* ClusterTree;
    int definedLevel;
    int Dim;
    int cluster_size;
    CubeSlot * HeuristicIndices;
    ClusterBranch found;
    ClusterBranch trail;
    void mainClusterInit(const CubeSlot* solved_state);
    void sideClusterInit();
    void clusterInit(const int & dim=0, const CubeSlot * solved_state = nullptr);
  private:
    
    int * ClusterDimensions;
    ClusterTree ClusteredSolutions;
    
    int * SideClusterDimensions;
    ClusterTree SideCluster;
    
    bool noChanges(const int& dim, const CubeSlot* solved_state) const;
   
  public:
 
    const int& dimensions(const int & Index) 
    {
      return ClusterDimensions[Index];      
    }
    const int& sideDimensions(const int & Index) 
    {
      return SideClusterDimensions[Index];
    }
    ClusterBranch solutions(const int & Index) 
    {
      return ClusteredSolutions[Index];
    }
    ClusterBranch sideSolutions(const int & Index) 
    {
      return SideCluster[Index];
    }
    void deinit();
    bool initialized() const {return Dim!=-1;}
    Cluster();
    ~Cluster() {}
    int indexOf(const Topology::t_state * Rot, const bool & inv) const;
    int indexOf(const CubeSlot * R1, const CubeSlot * R2) const;
    int subIndexOf(const Topology::t_state * Rot, const bool & inv) const;
    int subIndexOf(const CubeSlot* R1, const CubeSlot* R2, const CubeSlot* R3) const;
  };
  static Cluster * cluster;
  SearchEngine Engine;
  void initStates(const int & SizeS);
  void setConditions(Stream & IS);
  bool checkResult(int& result, String& Result, const Topology::t_state * S1, const Topology::t_state * S2);
  // Search Engines:  
  int checkConditions(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence);
  int fastestCheck(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence, const Topology::t_state* third_sequence);
  int fastestCheck(const Topology::t_state* first_sequence, const Topology::t_state* second_sequence);
  int heuristicSearch(const Topology::t_state* rot_sequence, const Topology::t_state*);
  int useCache(const Topology::t_state* rot_sequence);
  int useExtendedCache(const Topology::t_state* rot_sequence);
  bool isHeuristicSearch() const {return HeuristicSearch;}
  // ===============
    int searchManager(int& result, String& Result, const int& level);
    int secondStage  (int& result, String& Result, const Topology::t_state* T);
  
public:
  Rubik_BF(const Rubik * R, Stream& IS, const String & AS);
  std::pair<int,String> start();
  ~Rubik_BF();
};

#endif
