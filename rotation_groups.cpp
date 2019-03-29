#ifndef ROTATION_GROUPS_DEFINITIONS
#define ROTATION_GROUPS_DEFINITIONS

#include"cube_topology.h"

static inline bool is_inverse(const CubeSlot& A)
{
  return A&Topology::Inverter;
}

static bool redundancy(const CubeSlot & side, const CubeSlot & last, const CubeSlot & exclude)
{
  return side==(last&7) || ((side==OPPOSITE(last&7))&&(side==(exclude&7))); // use &7 to hide the number of side rotation
}

static inline int relative_position(CubeSlot skip_,CubeSlot skip, const CubeSlot & rot)
{
  skip_&=7;
  skip &=7;
  const int a=(rot/8); 
  const int b=(rot%8); 
  int Result=3*b+a;   
  Result-= (skip < b) ? 3 : 0;
  Result-= (skip_< b) && (skip_==OPPOSITE(skip)) ? 3 : 0;
  return Result;
}

static inline int reverse_rotation(const CubeSlot & R)
{
  const int turn=2-(R/8);
  const int side=R&7;
  return 8*turn+side;
}

void Topology::buildRotations()
{
  SideGroup[0]=0;
  SideGroup[1]=SingleSide;
  SideGroup[2]=DoubleSide;
  SideGroup[3]=InvertSide;
  for(int Digit=0; Digit<6; ++Digit)
  {
    CubeSlot * R=new CubeSlot[NumberOfSideMarks];
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
      CubeSlot * R=new CubeSlot[NumberOfSideMarks];
      CPY_FUNC(R,IdentityMap)
      createRotation(R,side,mod);
      Rotation[mod*8+side]= R;
    }
  }
}

void Topology::operateOnRestrictedSpace(CubeSlot* Q, const CubeSlot* R, const CubeSlot& Rot)
{
  for(const int op=(SingleSide<<3)+Rot; *R!=-1; ++R)
  {
    *(Q++)=Singleton->Rotation[op][*R];
  }
  *Q=-1;
}

int Topology::rotation(const CubeSlot & Index, const CubeSlot & Rot, const bool & Invert)
{
  return Singleton->Rotation[Invert ? OPPOSITE(Rot):Rot][Index];
}


bool Topology::createRotation(CubeSlot * Q, const CubeSlot & Rot, const int & A)
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
	  *q=Rotation[is_inverse(A) ? OPPOSITE(Rot) : Rot][*q]; 
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
      const bool Invert=is_inverse(A); 
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

const CubeSlot * Topology::operate(const CubeSlot & Rot, const int& A)
{
  return Singleton->Rotation[(A<<3)+Rot];
}

void Topology::operate(const CubeSlot * Q, const CubeSlot * R, CubeSlot * S) // Group operation: QR -> S
{
  FOR_FUNC(index)
  {
    S[index]=R[Q[index] ];
  }
}

bool Topology::defOperation(CubeSlot * Q, const std::string& Operations, const int & Including, const int & Restriction)
{
  bool Result=false;
  int all_cubes=0;
  CPY_FUNC(Q,IdentityMap)
  for(std::string::const_iterator it=Operations.begin(); it!=Operations.end(); ++it)
  {
    const char C=*it;
    char Rot;
    switch(C)
    {
      case 'X':
	Rot='R';
	all_cubes=AllCubes;
	break;
      case 'Y':
	Rot='U';
	all_cubes=AllCubes;
	break;
      case 'Z':
	Rot='F';
	all_cubes=AllCubes;
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
      const CubeSlot * op=operate(R,Modifier|Including|all_cubes);
      actOn(Q,op);
      Result=(op!=nullptr);
    }
  }
  return Result;
}

void Topology::inverse(const CubeSlot * Q, CubeSlot * Result)
{
  C_EACH_FUNC(Q,q,index)
  {
    Result[*q]=index;
  }
}

void Topology::inverse(CubeSlot * Q)
{
  EMPTY(temp)
  CPY_FUNC(temp, Q) 
  C_EACH_FUNC(temp,t,index)
  {
    Q[*t]=index;
  }
}

void Topology::actOn(CubeSlot * Q, const CubeSlot * R)
{
  CubeSlot *q=Q;
  FOR_FUNC(index)
  {
    *q=R[*q];
    ++q;
  }
}

void Topology::initSeekers() const
{
  if(PathGenerator.head)
  {
    for(t_state * p = PathGenerator.head; p->state; ++p)
    {
      p->dealloc();
    }
  delete[] PathGenerator.head; 
  }
  TraceSize=length_indices[CONFIG_CACHE_MEMORY_BOUND]-1;
  PathGenerator.head = new t_state[TraceSize+2]; // 976338 -> 877032
  PathGenerator.head->copy(IdentityMap);
  PathGenerator.head->inverse=PathGenerator.head;
  PathGenerator.head->parent =PathGenerator.head;
  t_state * Head=PathGenerator.head;
  t_state * node = Head;
  t_state * next = node+1;
  t_state overflow[3];
  int index=0;
  while(node->length<CONFIG_CACHE_MEMORY_BOUND)
  {
    for(int side=0,first_ch=1;side<6;++side)
    {
      if(redundancy(side,node->last,node->parent->last))
      {//OUT_((index++)<<"redundancy!!!")
	continue;
      }
      if(first_ch)
      {
	node->first_child=next;
	first_ch=0;
      }
      for(int rot=0;rot<3; ++rot, ++next)
      {
	next->copy(rot ? (next-1)->state : node->state);
	actOn(next->state, Rotation[side + SingleSide*8]);
	next->parent=node;
	if(next->parent->first==7)
	{
	  next->first=side;
	}
	else
	{
	  next->first=next->parent->first;
	}
	next->last=side+(rot*8);
	next->length = next->parent->length+1;
      }
    }
    ++node;
  }
  next->length=CONFIG_CACHE_MEMORY_BOUND+1;
  for(t_state * p=PathGenerator.head+1; p->length<=CONFIG_CACHE_MEMORY_BOUND;++p)
  {
    const t_state * INV_PAR = p->parent->inverse;
    const t_state * INV=PathGenerator.head+1+relative_position(7,7,reverse_rotation(p->last)); 
   
    if(p->parent!=PathGenerator.head)
    for(int anc=INV_PAR->length-1;anc>=0;--anc)
    {
      const CubeSlot step=INV_PAR->ancestor(anc)->last; 
      INV=INV->first_child+relative_position(INV->parent->last,INV->last,step);
    }
    p->inverse=INV;
  }
}

#endif