#ifndef CUBE_TOPOLOGY_SINGLETON
#define CUBE_TOPOLOGY_SINGLETON

#include "globals.h"

class Topology
{
public:
  static const char * SideMarks;
private:
  
  static int CONFIG_CACHE_MEMORY_BOUND; // 6: HIGH 5: NORMAL 4 OR LESS: VERY LOW, MUST BE SET
                                       // BY THE INIT FUNCTION IN THE LISP ENVIRONMENT
  static const Topology * Singleton;
  static const char PositiveGroup[][4];
  int SideGroup[4];
  struct Side
  {
    const char SideMark;
    const int  Digit;
    
    Side * ROT[6];
    Side(const char& S, const int& D);
  } * Sides[6];
  
  struct Cube 
  {
    String Facets;
    int Index;
    int Eigenvalue;
    int Pivot;
    Side * Sides[4]={};
    bool OnTheSide[6]={};
    
    Cube();
  } Cubes[NumberOfSideMarks];
public:
  static const int length_indices[]; // {1,19,289,4177,60553,877033,12704329}
  static int TraceSize; 
  struct t_state
  {
    const t_state * parent;
    const t_state * first_child;
    const t_state * inverse;
    CubeSlot * state;
    CubeSlot first;
    CubeSlot last;
    int length;
    t_state(): parent(nullptr),first_child(nullptr),inverse(nullptr), state(nullptr),first(7), last(7), length(0) {}
    String path() const;
    t_state* alloc();
    void dealloc();
    void copy(const CubeSlot * C);
    const t_state * ancestor(const int & i) const {return i<=0 ? this : parent->ancestor(i-1);}
  };
 
  class RotationRange
  {
    const t_state * CurrentState;
    const int MaxLength;
  public:
    RotationRange(const int&,const int&);
    const t_state* state() const;
    RotationRange* next();
  };
  struct seeker
  {
    t_state * 	head;
    seeker();
    void init();
    ~seeker();
  };
  private:
  
  mutable seeker PathGenerator;
  
  void initSeekers() const;
  
  void setEigenvalue(Topology::Cube& C);
  void setPivotNumbers();
  CubeSlot SideDigits[90]={};
  CubeSlot Hash_In[CompressedArraySize]={};
  CubeSlot Hash_Out[NumberOfSideMarks]={};
  const CubeSlot * Rotation[256]={};
  
  String tokenTrails[15]={};
  
  static int hash(const int & r2, const int & r1, const int & r0);
  void setHash();
  void makeConnectionBetween(Topology::Side* A, Topology::Side* B, const int& RotatingSide);
  void makeConnectionBetween(const int& A, const int& B, const int& RotatingSide);
    int computeRotate(const Topology::Cube& C, const CubeSlot& R);
  void buildRotations();
  bool createRotation(CubeSlot* Q, const CubeSlot& Rot, const int& A);
  void createTokens();
  
  Topology();
  ~Topology();
public:
  static void singleton();
  static void close();
  static String token(int T);
  static const enum // {0,...,15,16}
  {
    NotDefined	=0,
    DoubleMove	=1,
    Inverter	=2,
    SingleSide	=4, 
    Middle	=8,
    
    Block	=SingleSide+Middle, 
    
    DoubleSide	=DoubleMove+SingleSide,
    DoubleMiddle=DoubleMove+Middle,
    DoubleBlock	=DoubleMove+Block,
    
    InvertSide	=Inverter+SingleSide,
    InvertMiddle=Inverter+Middle,
    InvertBlock	=Inverter+Block,
   
    AllCubes=16
  } Modifiers;

  static const bool& onTheSide(const int& In, const int& S);
  static int getIndex(const String & SMarks);
  static int getIndex(const int & x, const int & y, const int & z);
  static const int& getEigenvalue(const int& Index);
  static const int& getPivotNumber(const int& Index);
  static const String & sideMarksOf(const int & Index);
  static int sideDigit(const char & C);
  static int rotation(const CubeSlot& Index, const CubeSlot& Rot, const bool& Invert=false);
  static const CubeSlot * rotation(const int& Rot) {return Singleton->Rotation[Rot];}
  static bool defOperation(CubeSlot* Q, const std::string& Operations, const int& Including = NotDefined, const int& Restriction = NotDefined);
  static const CubeSlot * operate(const CubeSlot& Rot, const int& A);
  static void operate(const CubeSlot * Q, const CubeSlot * R, CubeSlot * Result);
  static void operateOnRestrictedSpace(CubeSlot * Q, const int& Rot, const bool & Invert, const bool & Double);
  static void operateOnRestrictedSpace(CubeSlot * Q, const CubeSlot * R, const CubeSlot& Rot);
  static void inverse(const CubeSlot * Q, CubeSlot * Result);
  static void inverse(CubeSlot * Q);
  static void actOn(CubeSlot * Q, const CubeSlot * R);
  static char oppositeSide(const char& C);
  static int sideGroup(const int& S) {return (Singleton->SideGroup[S])<<3;}
  static const t_state * getTrace() {return Singleton->PathGenerator.head;}
  static void initCache(const int& C); 
  static int cacheLevel() {return CONFIG_CACHE_MEMORY_BOUND;}
  static const int& getTraceSize() {return TraceSize;}
  static String cachePoint(const int & index) {return Singleton->PathGenerator.head[index].path();}
};
#endif