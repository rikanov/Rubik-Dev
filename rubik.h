#ifndef RUBIK_CLASS
#define RUBIK_CLASS

#include<readline/readline.h>
#include<readline/history.h>
#include"auxiliary.h"

#define UI_func(X) String X(const String & S); String X(Stream& IS);
#define UI_rfunc(X) String Rubik::X(const String & S) {Stream IS(S); return X(IS);} String Rubik::X(Stream& IS)
#define CALL_BACK(SN,F,A) Stream SN; {Stream eval__(A);SN.str(F(eval__));}
#define GET(A) String A; IS>>A;
#define GET2(A,B) String A,B; IS>>A; IS>>B;
#define GETLIST(L) Stream L(list(IS));
#define PARSER(A) String A=parser(IS);
#define PARSER2(A,B) PARSER(A) PARSER(B)
#define GETLINE(L) String L;getline(IS,L,';');TRIM_END(L);{String ToEnd; getline(IS,ToEnd); if(ToEnd!="") {IS.str(ToEnd);IS.clear();}}
#define OUTSPREAD(O) String O; getline(IS,O);
#define LOOP_STACK (*Var_space)["loop-stack"]
#define BF_SUCCESS (*Var_space)["brute-force-success"]
#define BF_RESULT  (*Var_space)["brute-force-result"]
#define BF_WIDTH   std::max(atoi((*Var_space)["brute-force-width"].c_str()),10000)

using namespace auxiliary;

class Rubik_BF;
class Rubik 
{
  friend class Rubik_BF;
  static std::map<String, Rubik*> * Collection; 
  static std::map<String, String> * Var_space;
  
  static char ** commandCompletion (const char * stem_text , int start , int end );
  static char *  getCommands       (const char * stem_text ,int state ); 
  static void    setAutocomp       (const char * Add_text);
  
  static Rubik* Global;
  
  const String Object;
  
		// Let I the index of a certain side mark in the current state 
		// of the Rubik's Cube. Then, let:
  CubeSlot * A_map; 	// indices of side marks what we need here, 
  CubeSlot * B_map;	// indices of side marks what ARE actually here

  CubeSlot * Sup_map;  // to find partial solutions we need supposations
  CubeSlot * Sup_inv;  // by default they both are identical mappings. (no supposation)  

  static const int MaximumStackDepth=256;
  CubeSlot ** Stack;
  int stack_pointer=0;
  
  CubeSlot * RotationCache;
  CubeSlot * RotationAlign;
  bool NeedAlign;

  void 
  initRubik		();
  
  void 
  initStack		();
  
  void 
  initCommandComp	();
  
  void 
  printCollection	() const;
  
  bool 
  is_solved		(const CubeSlot * Cubes = IdentityMap, 
			 const int & Limit = NumberOfSideMarks) const;
			 
  bool 
  variableEquality	(
			String & A, 
			const String & B) const;
  void 
  variable		(
			Stream & IS, 
			String & R);
  void 
  macro			(
			Stream & IS, 
			String & R);
  
  void 
  setRotation		(const String& Rot);
  
  Rubik&
  applyRotation		();
  
  String 
  functionResolver	(
			Stream & IS, 
			const String & R);
  
  void 
  select		(
			Stream & IS, 
			String & Result, 
			const bool & Inv); 
  
  void 
  noSuppose		();
  
  void 
  suppose		(const Sidemarks & S);
  
  void 
  align			(
			const Sidemarks & A, 
			const Sidemarks & B);
  
  Sidemarks 
  whatIs		(const Sidemarks & S) const;
			
  Sidemarks 
  locationOf		(const Sidemarks & S) const;

  void 
  swapTwoPieces         (const Sidemarks& A, const Sidemarks& B);
  
  String 
  bruteForce		(
			Stream& IS, 
			const String& AS) const;
			
  static String
  random		();
  
     //==========================//
    //  *** User Interface ***  //
   //==========================//
  UI_func(REPL)
  UI_func(parser)
  UI_func(file_open)
  UI_func(editor)
  
  UI_func(progn)
  UI_func(list)
  UI_func(getValue)
  UI_func(defvar)
  UI_func(defun)
  UI_func(defmacro)
  UI_func(delete_var)
  UI_func(nilEquality)
  UI_func(variableEquality)
  UI_func(listEquality)
  UI_func(parsingEquality)
  UI_func(condition)
  UI_func(forLoop)
  UI_func(logicalOr)
  UI_func(logicalXor)
  UI_func(logicalAnd)
  
  UI_func(assoc)
  UI_func(select)
  UI_func(deselect)
  UI_func(mapcar)
  UI_func(conc)
  UI_func(stringReplace)
  UI_func(regExp)
  UI_func(regExpFast)
  UI_func(echo)
  UI_func(catFiles)
  UI_func(saveCube)
  UI_func(loadCube)
    
  UI_func(sameCubes)
  UI_func(merge)
  UI_func(permute)
  UI_func(mirror)
  UI_func(pathFinder)
  UI_func(crypt)
  UI_func(swap)
  UI_func(callBruteForce)
  UI_func(random)
  
  UI_func(what_is)
  UI_func(where_is)
  UI_func(cube)
  UI_func(solvedp)
  UI_func(doRotations)
  UI_func(store)
  UI_func(revert)
  UI_func(setAlign)
  UI_func(printSmarks)
  
public:
  
  Rubik  ();
  Rubik  (const Rubik&) =default;
  Rubik  (const String& Name);
  ~Rubik ();
 
  Rubik & operator =  (const Rubik& R)=default;
  
  void print(const String & C ) const;
  void REPL (std::istream & IS=std::cin, std::ostream & OS=std::cout);
  
  String file_open(const char * F);
};

class Rubik_BF
{
  friend class Rubik;
  typedef int (Rubik_BF::*SearchEngine)(const Topology::t_state*,const Topology::t_state*);
 
  int seekerDepth;
  int best_choice;
  bool foundBetter;
  const Rubik * RubikBase;
  CubeSlot * InitialState;
  CubeSlot * InvInitialState;
  CubeSlot * SolvedState;
  String resolver(const Topology::t_state* A,const Topology::t_state* B);
  struct Cluster
  {
    int Dim;
    int cluster_size;
    CubeSlot * HeuristicIndices;
    const Topology::t_state** found;
    void clusterInit(const int & dim=0, const CubeSlot * solved_state = nullptr);
  private:
    
    int * ClusterDimensions;
    const Topology::t_state *** ClusteredSolutions;
    bool noChanges(const int& dim, const CubeSlot* solved_state) const;
   
  public:
 
    const int& dimensions(const int & Index) {return ClusterDimensions[Index];}
    const Topology::t_state ** solutions(const int & Index) {return ClusteredSolutions[Index];}
    void deinit();
    bool initialized() const {return Dim!=-1;}
    Cluster();
    ~Cluster() {}
    int indexOf(const Topology::t_state * Rot);
    int indexOf(const CubeSlot * R1, const CubeSlot * R2, const CubeSlot * R3);
  };
  static Cluster cluster;
  SearchEngine Engine;
  void initStates(const int & SizeS);
  void setConditions(Stream & IS);
  bool checkResult(int& result, String& Result, const Topology::t_state * S1, const Topology::t_state * S2);
  // Search Engines:  
  int checkConditions(const Topology::t_state* Foresight, const Topology::t_state* Trail);
  int fastestCheck(const Topology::t_state* Foresight, const Topology::t_state* Trail);
  int heuristicalSearch(const Topology::t_state* Foresight, const Topology::t_state* Trail);
  int useCache(const Topology::t_state* Foresight, const Topology::t_state* Trail);
  
  // ===============
    int searchManager(int& result, String& Result, const int& level);
    int secondStage  (int& result, String& Result, const Topology::t_state* T);
  
public:
  Rubik_BF(const Rubik * R, Stream& IS, const String & AS, const int & bfWidth);
  std::pair<int,String> start();
  ~Rubik_BF();
};

#endif