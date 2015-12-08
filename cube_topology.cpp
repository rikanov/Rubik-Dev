#include "cube_topology.h"

const char * Topology::SideMarks="@FURLDB";
const Topology * Topology::Singleton(nullptr);

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
    Singleton=new Topology;
}


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
  
  for(Digit=0; Digit<6; ++Digit)
  {
    int * R=new int[NumberOfSideMarks];
    FOR_FUNC(index)
    {
      R[index]=computeRotate(Cubes[index],Digit);
    }
    Rotation[Digit]=R;
  }
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
	//OUT_(index<<':'<<value<<':'<<Cubes[index].Facets)
	++index;
      }
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
      return 0;
    case 1:
      return Singleton->Hash_In[ hash(0,0,Singleton->SideDigits[SMarks[0]]) ];
    case 2:
      return Singleton->Hash_In[ hash(0,Singleton->SideDigits[SMarks[0]],Singleton->SideDigits[SMarks[1]]) ];
    case 3:
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
  return Singleton->SideDigits[C];
}
void Topology::operateOnRestrictedSpace(int* Q, const int* R, const int& Rot, const bool & Invert)
{
  int *q=Q;
  for(const int * r=R; *r!=-1; ++r)
  {
    *(q++)= Singleton->Cubes[*r].OnTheSide[Rot] ?
	    Singleton->Rotation[Invert ? OPPOSITE(Rot):Rot][*r] :
	    *r;
  }
  *q=-1;
}

int Topology::rotation(const int& Index, const int& Rot, const bool & Invert)
{
  return Singleton->Rotation[Invert ? OPPOSITE(Rot):Rot][Index];
}


bool Topology::operate(int* Q, const int& Rot, const int& A)
{
  bool Result=false;
  if(A & AllCubes)
  {
    if(A & DoubleMove)
    {
      EACH_FUNC(Q,q,index)
      {
	  *q=Singleton->Rotation[Rot][*q];
	  *q=Singleton->Rotation[Rot][*q];
      }
      
    }	
    else
    {
      EACH_FUNC(Q,q,index)
      {
	  *q=Singleton->Rotation[A&Inverter ? OPPOSITE(Rot) : Rot][*q]; 
      }
    }
    Result=true;
  }
  else
  {
    EACH_FUNC(Q,q,index)
    {
      const Cube C=Singleton->Cubes[*q];
      bool Acting=false;
      const bool Invert=A>Inverter;
      switch(A)
      {
	case InvertSide:
	case DoubleSide:  
	case SingleSide:
	  Acting= C.OnTheSide[Rot]; 
	  break;
	//----------------
	  
	case InvertMiddle:
	case DoubleMiddle:
	case Middle:
	  Acting= ! C.OnTheSide[Rot] && ! C.OnTheSide[OPPOSITE(Rot)]; 
	  break;
	//----------------
	  
	case InvertBlock:
	case DoubleBlock:
	case Block:
	  Acting= ! C.OnTheSide[OPPOSITE(Rot)]; 
	  break;
	//----------------
	  
	default:
	  break; // an exception should be thrown from here
      }
      if(Acting)
      {
	Result=true;
	*q=Singleton->Rotation[Invert ? OPPOSITE(Rot) : Rot][*q]; 
	if( !Invert && (A & DoubleMove))
	{
	  *q=Singleton->Rotation[Rot][*q];
	}	  
      }
    }
  }
  return Result;
}

void Topology::operate(const int* Q, const int* R, int* S) // Group operation: QR -> S
{
  FOR_FUNC(index)
  {
    S[index]=R[Q[index] ];
  }
}

bool Topology::defOperation(int* Q, const std::string& Operations, const int & Including, const int & Restriction)
{
  bool Result=false;
  CPY_FUNC(Q,IdentityMap)
  for(std::string::const_iterator it=Operations.begin(); it!=Operations.end(); ++it)
  {
    const char C=*it;
    char Rot;
    switch(C)
    {
      case 'X':
	Rot='R';
	break;
      case 'Y':
	Rot='U';
	break;
      case 'Z':
	Rot='F';
	break;
      default:
	Rot=C;
    }
    const int R=Singleton->SideDigits[UPCASE(Rot)]-1;
    int Modifier= IS_UPCASE(C) ? Block : SingleSide;
    if( it+1 !=Operations.end())
    {
      switch(*(it+1))
      {
	case '\'':
	  Modifier|=Inverter;
	  ++it;
	  break;
	case '|':
	  Modifier^=SingleSide;
	  Modifier|=Middle;
	  ++it;
	  if(it+1==Operations.end() || (*(it+1)!='2' && *(it+1)!='|'))
	  {
	    break;
	  } // don't break if middle block rotation is doubled!
	case '2':
	  Modifier|=DoubleMove; 
	  ++it; 
	  break;
	default:
	  break;
      }
    }
    if((Modifier&Restriction)==Restriction)
    {
      Result=operate(Q,R,Modifier|Including);
    }
  }
  return Result;
}

void Topology::inverse(const int* Q, int* Result)
{
  C_EACH_FUNC(Q,q,index)
  {
    Result[*q]=index;
  }
}

void Topology::inverse(int* Q)
{
  EMPTY(temp)
  CPY_FUNC(temp, Q) 
  C_EACH_FUNC(temp,t,index)
  {
    Q[*t]=index;
  }
}

void Topology::actOn(int* Q, const int* R)
{
  int *q=Q;
  FOR_FUNC(index)
  {
    *q=R[*q];
    ++q;
  }
}

Topology::~Topology()
{
  Side ** S=Sides;
  const int ** R=Rotation;
  for(int digit=0;digit<6;++digit)
  {
    delete *(S++);
    delete *(R++);    
  }
}

void Topology::close()
{
  delete Singleton;
}

