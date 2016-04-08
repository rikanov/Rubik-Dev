#ifndef ROTATION_GROUPS_DEFINITIONS
#define ROTATION_GROUPS_DEFINITIONS

#include"cube_topology.h"

static inline bool is_inverse(const int& A)
{
  return A&Topology::Inverter;
}

void Topology::buildRotations()
{
  SideGroup[0]=0;
  SideGroup[1]=SingleSide;
  SideGroup[2]=DoubleSide;
  SideGroup[3]=InvertSide;
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
      Rotation[mod*8+side]=createRotation(R,side,mod) ? R : IdentityMap;
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
      const int * op=operate(R,Modifier|Including|all_cubes);
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

void Topology::initTrace()
{
  for(int tabu=0; tabu<7;++tabu)
  {
    Extender[tabu]=new t_state[20]; // 3*6+1+1;
    Extender[tabu]->alloc()->copy(IdentityMap); 
    t_state *node=Extender[tabu];
    for(int j=0;j<6;++j)
    {
      if(j==tabu)
      {
	continue;
      }
      (++node)->alloc()->copy(IdentityMap);
      actOn(node->state,Rotation[j+SingleSide*8]);
      node->last=j+SingleSide*8;
      node->parent=Extender[tabu];
      for(int k=2;k<4;++k)
      {
	(node+1)->alloc()->copy(node->state);
	++node;
	actOn(node->state,Rotation[j+SingleSide*8]);
	node->last=j+SideGroup[k]*8;
	node->parent=Extender[tabu];
      }
    }
    (++node)->state=nullptr;
  }
  OUT_("trace build...")
  for(int tabu=0;tabu<7;++tabu)
  {
    t_state *node=Trace[tabu], *trail=Trace[tabu];
    int length=0;
    for(int next=0;next<6;++next)
    {
      if(next==tabu)
      {
	continue;
      }
      for(t_state * e=Extender[next];e->state; ++e, ++trail)
      {
	*trail=*e; //TODO
	++length;
      }
    }
    ++node;
    while(length<TraceSize)
    {
      for(const t_state * e=Extender[(node->last)&7]+1;e->state; ++e,++trail, ++length)
      {
	trail->alloc()->copy(node->state);
	actOn(trail->state,e->state);
	trail->last=e->last;
	trail->parent=node;
      }
      ++node;
    }
  }
  OUT_("done")
}

void Topology::initSeekers()
{
  //All in 8 depths
  seeker * all = new seeker;
  SelectGroup["all"]=all;
  const int length=78102; //1171601;
  all->head = new t_state[length+1];
  all->head->alloc()->copy(IdentityMap);
  all->head->last=-1;
  const t_state * node = all->head;
  t_state * next = all->head;
  int next_side=5; 
  OUT_(length)
  for(int i=0, j=0; i<length; ++i)
  {
    const int turn=i%3;
    if(turn==0)
    {
      next_side=(next_side+1)%6;
    }
    if(next_side==(node->last&7))
    {
      ++j;
      if(j==18)
      {
	j=0;
	++node;
      }
      continue;
    }
    ++next; ++j;
    next->parent=node;
    next->alloc()->copy(node->state);
    next->last=next_side+SideGroup[turn+1]*8;
    actOn(next->state,Rotation[next->last]);
    if(j==18)
    {
      j=0; 
      ++node;
    }
  }
  ++next=nullptr;
}

void Topology::initSeekers2()
{
  PathGenerator.head = new t_state[976338]; // 976338
  PathGenerator.headFourLengthEnd = PathGenerator.head+1;
  PathGenerator.trails = new t_state* [6];
  
  PathGenerator.head->alloc()->copy(IdentityMap);
  PathGenerator.head->first=-1;
  PathGenerator.head->last=-1;
  PathGenerator.head->length=0;
  t_state * Head=PathGenerator.head;
  int Trails[6]={0};
  for(int i=0;i<6;++i)
  {
    PathGenerator.trails[i]=new t_state [151876]; // 
    Trails[i]=0;
  }
  
  t_state * node = Head;
  t_state * next = node+1;
  t_state overflow[3];
  int index=0, index5=0;
  while(node->length<5)
  {
    for(int side=0;side<6;++side)
    {
      if(side==node->last)
      {
	continue;
      }
      for(int rot=0;rot<3; ++rot, ++next)
      {
	const t_state * from = (rot ? (next-1) : node);
	next->alloc()->copy(from->state);
	actOn(next->state, Rotation[side + SingleSide*8]);
	next->parent=from;
	if(next->parent->first<0)
	{
	  next->first=side;
	}
	else
	{
	  next->first=next->parent->first;
	}
	next->last=side;
	next->length = next->parent->length + (rot==0);
	if(next->length==5)
	{
	 PathGenerator.trails[next->first][Trails[next->first]]=*next;
	 ++Trails[next->first];
	}
	else
	{
	  ++PathGenerator.headFourLengthEnd;
	}
      }
    }
    ++node;
  }
  PathGenerator.trails[4]->path();
  for(t_state * T=PathGenerator.trails[0];T->state!=nullptr;++T)
  {
    OUT_(T->path());
  }
  OUT_("end teszt")
}

#endif