#ifndef ROTATION_GROUPS_DEFINITIONS
#define ROTATION_GROUPS_DEFINITIONS

#include"cube_topology.h"

void Topology::buildRotations()
{
  for(int Digit=0; Digit<6; ++Digit)
  {
    int * R=new int[NumberOfSideMarks];
    FOR_FUNC(index)
    {
      R[index]=computeRotate(Cubes[index],Digit);
    }
    Rotation[Digit]=R;
  } 
  for(int mod=1;mod<32;++mod)
  {
    for(int side=0;side<6;++side)
    {
      int * R=new int[NumberOfSideMarks];
      CPY_FUNC(R,IdentityMap)
      Rotation[mod*8+side]=createRotation(R,side,mod) ? R : nullptr;
    }
  }
}

void Topology::operateOnRestrictedSpace(int* Q, const int* R, const int& Rot)
{
  for(const int op=(SingleSide<<3)+Rot; *R!=-1; ++R)
  {
    *(Q++)=Singleton->Rotation[op][*R];
  }
  *Q=-1;
}

int Topology::rotation(const int& Index, const int& Rot, const bool & Invert)
{
  return Singleton->Rotation[Invert ? OPPOSITE(Rot):Rot][Index];
}


bool Topology::createRotation(int* Q, const int& Rot, const int& A)
{
  bool Result=false;
  if(A & AllCubes)
  {
    if(A & DoubleMove)
    {
      EACH_FUNC(Q,q,index)
      {
	  *q=Rotation[Rot][*q];
	  *q=Rotation[Rot][*q];
      }
      
    }	
    else
    {
      EACH_FUNC(Q,q,index)
      {
	  *q=Rotation[A&Inverter ? OPPOSITE(Rot) : Rot][*q]; 
      }
    }
    Result=true;
  }
  else
  {
    EACH_FUNC(Q,q,index)
    {
      const Cube C=Cubes[*q];
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
	*q=Rotation[Invert ? OPPOSITE(Rot) : Rot][*q]; 
	if( !Invert && (A & DoubleMove))
	{
	  *q=Rotation[Rot][*q];
	}	  
      }
    }
  }
  return Result;
}

const int * Topology::operate(const int& Rot, const int& A)
{
  return Singleton->Rotation[(A<<3)+Rot];
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
    int Modifier= IS_LOWCASE(C) ? Block : SingleSide;
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
      const int * op=operate(R,Modifier|Including);
      actOn(Q,op);
      Result=(op!=nullptr);
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

#endif