#include "cube_topology.h"
#include "auxiliary.h"

const char * Topology::SideMarks="@FURLDB";
const Topology * Topology::Singleton(nullptr);
const char Topology::PositiveGroup[][4]={"FUR","FRD","FDL","FLU","UBR","ULB","RBD","BLD"};

Topology::Side::Side(const char& S, const int& D): SideMark(S), Digit(D) 
{
  for(int index=0;index<6;++index)
  {
    ROT[index]=this;
  }
}

///==========================================================

Topology::Cube::Cube()
{
}

///=======================================================

void Topology::singleton()
{
  if(Singleton==nullptr)
  {
    Singleton=new Topology; 
  }
    srand(time(NULL));
}
std::string Topology::t_state::path() const
{
  if(last<0)
  {
    return "";
  }
  String mark;
  mark.push_back(Topology::SideMarks[last+1]);
  return parent ? auxiliary::mergeSimplePaths(parent->path(),mark) : "";
}

Topology::t_state * Topology::t_state::alloc()
{
  state=new int[NumberOfSideMarks];
  return this;
}
void Topology::t_state::dealloc()
{
  delete state;
  state=nullptr;
}
void Topology::t_state::copy(const int* C)
{
  alloc();
  CPY_FUNC(state,C)
}

///=====================


Topology::Topology()
{
  int Digit=0;
  for(const char * p=SideMarks; *p!='\0'; ++p)
  {
    SideDigits[*p]=Digit++;
  }
  
  for(Digit=0; Digit<6; ++Digit)
  {
    Sides[Digit]=new Side(SideMarks[Digit+1],Digit);
  }
  
  makeConnectionBetween(1,2,0);
  makeConnectionBetween(2,4,0);
  makeConnectionBetween(4,3,0);
  makeConnectionBetween(3,1,0);
  
  makeConnectionBetween(0,3,1);
  makeConnectionBetween(3,5,1);
  makeConnectionBetween(5,2,1);
  makeConnectionBetween(2,0,1);
  
  makeConnectionBetween(0,1,2);
  makeConnectionBetween(1,5,2);
  makeConnectionBetween(5,4,2);
  makeConnectionBetween(4,0,2);
 
  setHash(); 
  buildRotations();
  createTokens();
  initSeekers();
}

int Topology::hash(const int& r2, const int& r1, const int& r0)
{
  const int hash_core1=6, hash_core2=34;
  return hash_core2*r2+hash_core1*r1+r0;
}

void Topology::setHash()
{
  int index=0;
  for(index=0;index<6;++index)
  {
    const int h_index=hash(0,0,index+1);
    Hash_In [h_index]=index;
    Hash_Out[index]=h_index;
    Cubes[index].Facets=SideMarks[index+1];
    Cubes[index].Sides[0]=Sides[index];
    Cubes[index].OnTheSide[index]=true;
  }
    
  int hash_checking=hash(0,0,index);
  for(int r2=0;r2<7;++r2)
    for(int r1=1;r1<7;++r1)
      for(int r0=1;r0<7;++r0)
      {
	if(r2==r1 || r1==r0 || r2==r0)
	{
	  continue;
	}
	if(7-r2==r1 || 7-r1==r0 || 7-r2==r0 ||
	   r2==7-r1 || r1==7-r0 || r2==7-r0 )
	{
	  continue;
	}
	const int value=hash(r2,r1,r0);
	if(value<=hash_checking)
	{
	  OUT_("Hash conflict.....")
	}
	hash_checking=value;
	Hash_In [value]=index;
	Hash_Out[index]=value;
	
	Cube & Q=Cubes[index];
	std::string & F=Q.Facets;
	bool * S=Q.OnTheSide;
	Side ** C=Q.Sides;
	if(r2)
	{
	  F.push_back(SideMarks[r2]);
	  S[r2-1]=true;
	  C[2]=Sides[r2-1];
	}
	F.push_back(SideMarks[r1]); S[r1-1]=true; C[1]=Sides[r1-1];
	F.push_back(SideMarks[r0]); S[r0-1]=true; C[0]=Sides[r0-1];
	Cubes[index].Index=index;
	setEigenvalue(Q);
	++index;
      }
}

void Topology::setEigenvalue(Topology::Cube & C)
{
  if(C.Facets.length()<3)
  {
    C.Eigenvalue=0;
  }
  else
  {
    char tester[4];
    strcpy(tester,C.Facets.c_str());
    for(int j=0;j<8;++j)
    {
      if(strcmp(tester,PositiveGroup[j])==0)
      {
	C.Eigenvalue=1;
	return;
      }
    }
    for(int i=0;i<2;++i)
    {
      const char t=tester[0];
      tester[0]=tester[1];
      tester[1]=tester[2];
      tester[2]=t; // cyclic rotation <<
      for(int j=0;j<8;++j)
      {
	if(strcmp(tester,PositiveGroup[j])==0)
	{
	  C.Eigenvalue=1;
	  return;
	}
      }
    }
    C.Eigenvalue=-1;
  }
}

const int & Topology::getEigenvalue(const int & Index)
{
  return Index > 0 ? Singleton->Cubes[Index].Eigenvalue : Index;
}

int Topology::computeRotate(const Cube & C, const int& R)
{
  int r[3], i=0; r[1]=r[2]=0;
  for(Side * const* S=C.Sides; *S; ++S)
  {
    r[i++]=(*S)->ROT[R]->Digit+1; 
  }
  return Hash_In[hash(r[2],r[1],r[0])];
}

void Topology::makeConnectionBetween(const int& A, const int& B, const int& RotatingSide)
{
  makeConnectionBetween(Sides[A],Sides[B],RotatingSide);
}

void Topology::createTokens()
{
  tokenTrails[DoubleSide]	="2";
  tokenTrails[DoubleBlock]	="2";
  tokenTrails[Middle]		="|";
  tokenTrails[DoubleMiddle]	="||";
  tokenTrails[InvertSide]	="'";
  tokenTrails[InvertMiddle]	="|'";
  tokenTrails[InvertBlock]	="'";
}

std::string Topology::token(int T)
{
  String Result;
  char C=SideMarks[1+(T&7)];
  T>>=3;
  C+=((T&Block)==Block)?32:0; 
  Result.push_back(C); 
  Result+=Singleton->tokenTrails[T];
  return Result;
}

void Topology::makeConnectionBetween(Topology::Side* A, Topology::Side* B, const int & RotatingSide)
{
  A->ROT[RotatingSide]=B;
  B->ROT[5-RotatingSide]=A;
}

int Topology::getIndex(const std::string& SMarks)
{
  switch(SMarks.length())
  {
    case 0:
      return -1; 
    case 1:
      if(IS_FACET(SMarks[0])==false)
      {
	return -1;
      }
      return Singleton->Hash_In[ hash(0,0,Singleton->SideDigits[SMarks[0]]) ];
    case 2:
      if((IS_FACET(SMarks[0])&&IS_FACET(SMarks[1]))==false)
      {
	return -1;
      }
      return Singleton->Hash_In[ hash(0,Singleton->SideDigits[SMarks[0]],Singleton->SideDigits[SMarks[1]]) ];
    case 3:
      if((IS_FACET(SMarks[0])&&IS_FACET(SMarks[1])&&IS_FACET(SMarks[2]))==false)
      {
	return -1;
      }
      return Singleton->Hash_In[ hash(Singleton->SideDigits[SMarks[0]],Singleton->SideDigits[SMarks[1]],Singleton->SideDigits[SMarks[2]]) ];
    default:
      return -1; //ToDo Wrong input
  }
}

const bool & Topology::onTheSide(const int& In, const int& S)
{
  return Singleton->Cubes[In].OnTheSide[S];
}

int Topology::getIndex(const int& x, const int& y, const int& z)
{
  return Singleton->Hash_In[hash(x,y,z)];
}

const std::string& Topology::sideMarksOf(const int& Index)
{
  return Singleton->Cubes[Index].Facets;
}

int Topology::sideDigit(const char& C)
{
  return String(SideMarks).find(C)!=STR_END ? Singleton->SideDigits[C] : -1;
}

char Topology::oppositeSide(const char& C)
{
  return SideMarks[7-(String(SideMarks).find(C))];
}

Topology::~Topology()
{
  Side ** S=Sides;
  const int ** R=Rotation;
  for(int digit=0;digit<256;++digit)
  {
    delete *(S++);
    delete *(R++);    
  }
}

void Topology::close()
{
  delete Singleton;
}

