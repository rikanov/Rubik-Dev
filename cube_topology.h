#ifndef CUBE_TOPOLOGY_SINGLETON
#define CUBE_TOPOLOGY_SINGLETON

#include "globals.h"

class Topology
{
public:
  static const char * SideMarks;
private:
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
    Side * Sides[4]={};
    bool OnTheSide[6]={};
    
    Cube();
  } Cubes[NumberOfSideMarks];
public: 
  
  static const int TraceSize=980000;
  struct t_state
  {
    const t_state * parent;
    int * state;
    int Op;
    t_state(): parent(nullptr), state(nullptr),Op(0) {}
    String path() const;
    t_state* alloc();
    void dealloc();
    void copy(const int* C);
  };
private:
  t_state * Trace;
  t_state * Extender[7];
  void initTrace();
  
  void setEigenvalue(Topology::Cube& C);
  int SideDigits[90]={};
  int Hash_In[CompressedArraySize]={};
  int Hash_Out[NumberOfSideMarks]={};
  const int * Rotation[256]={};
  
  String tokenTrails[15]={};
  
  static int hash(const int & r2, const int & r1, const int & r0);
  void setHash();
  void makeConnectionBetween(Topology::Side* A, Topology::Side* B, const int& RotatingSide);
  void makeConnectionBetween(const int& A, const int& B, const int& RotatingSide);
  int computeRotate(const Cube& C, const int& R);
  void buildRotations();
  bool createRotation(int* R, const int & side, const int & mod);
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
  static const String & sideMarksOf(const int & Index);
  static int sideDigit(const char & C);
  static int rotation(const int& Index, const int& Rot, const bool& Invert=false);
  static const int * rotation(const int& Rot) {return Singleton->Rotation[Rot];}
  static bool defOperation(int* Q, const String& Operations, const int & Including=NotDefined, const int & Restriction=NotDefined);
  static const int* operate(const int& Rot, const int& A);
  static void operate(const int * Q, const int * R, int * Result);
  static void operateOnRestrictedSpace(int* Q, const int& Rot, const bool & Invert, const bool & Double);
  static void operateOnRestrictedSpace(int* Q, const int* R, const int& Rot);
  static void inverse(const int* Q, int* Result);
  static void inverse(int* Q);
  static void actOn(int * Q, const int * R);
  static char oppositeSide(const char& C);
  static int sideGroup(const int& S) {return (Singleton->SideGroup[S])<<3;}
  static const t_state * getTrace() {return Singleton->Trace;}
  static const t_state * getExtender(const int& e) {return Singleton->Extender[e];}
};
#endif