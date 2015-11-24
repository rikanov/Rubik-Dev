#ifndef CUBE_TOPOLOGY_SINGLETON
#define CUBE_TOPOLOGY_SINGLETON

#include <iostream>
#include<cstring>

#define OPPOSITE(x) (5-(x))
#define UPCASE(x) ((x)&95)
#define IS_UPCASE(x) ((x)&32)
#define FUNCTOR(x) int x[NumberOfSideMarks]; \
		    memcpy(x,IdentityMap,sizeof IdentityMap);
#define EMPTY(x) int x[NumberOfSideMarks]; 
#define CPY_FUNC(x,y) memcpy(x,y, NumberOfSideMarks*sizeof(int)); // DON'T use sizeof y or x !!!!!!!!
#define FOR_FUNC(x) for(int x=0;x<NumberOfSideMarks;++x)
#define EACH_FUNC(x,y,i) for(int* y=x,i=0;i<NumberOfSideMarks;++y,++i)
#define C_EACH_FUNC(x,y,i) int i=0;for(const int* y=x;i<NumberOfSideMarks;++y,++i)
#define OUT_(x) std::cout<<x<<std::endl;
#define OUT(x) std::cout<<x<<' '<<std::flush;
#define NL std::endl
		    
const int CompressedArraySize=240;
const int NumberOfSideMarks=78;	// = 8*3! + 12*2! 
const int IdentityMap[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,\
			40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77};
extern const char * SideMarks;

class Topology
{
  static const Topology * Singleton;
  
  struct Side
  {
    const char SideMark;
    const int  Digit;
    
    Side * ROT[6];
    
    Side(const char& S, const int& D);
  } * Sides[6];
  
  struct Cube 
  {
    std::string Facets;
    int Index;
    Side * Sides[4];
    bool onTheSide[6];
    
    Cube();
  } Cubes[NumberOfSideMarks];
  
  int SideDigits[90];
  int Hash_In[CompressedArraySize];
  int Hash_Out[NumberOfSideMarks];
  const int * Rotation[6];
  
  static int hash(const int & r2, const int & r1, const int & r0);
  void setHash();
  void makeConnectionBetween(Topology::Side* A, Topology::Side* B, const int& RotatingSide);
  void makeConnectionBetween(const int& A, const int& B, const int& RotatingSide);
  int computeRotate(const Cube& C, const int& R);
  
  Topology();
  ~Topology();
public:
  static void singleton();
  static void close();
  
  static const enum // {0,...,15,16}
  {
    NotDefined	=0,
    SingleSide	=1, 
    Middle	=2,
    DoubleMove	=4,
    Inverter	=8,
    
    Block	=SingleSide+Middle, 
    
    DoubleSide	=DoubleMove+SingleSide,
    DoubleMiddle=DoubleMove+Middle,
    DoubleBlock	=DoubleMove+Block,
    
    InvertSide	=Inverter+SingleSide,
    InvertMiddle=Inverter+Middle,
    InvertBlock	=Inverter+Block,
   
    AllCubes=16
  } Modifiers;
  
  static int getIndex(const std::string & SMarks);
  static const std::string & sideMarksOf(const int & Index);
  
  static void defOperation(int* Q, const std::string& Operations, const int & Including=NotDefined, const int & Restriction=NotDefined);
  static void operate(int* Q, const int& Rot, const int& A);
  static void operate(const int* Q, const int* R, int* Result);
  static void inverse(const int* Q, int* Result);
  static void inverse(int* Q);
  static void actOn(int * Q, const int * R);
};
#endif