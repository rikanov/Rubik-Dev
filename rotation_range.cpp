#include"cube_topology.h"

Topology::RotationRange::RotationRange(const int& min_length, const int& max_length):
 CurrentState(Singleton->PathGenerator.head+length_indices[ min_length]),
 MaxLength(max_length)
{
  
}

const Topology::t_state * Topology::RotationRange::state() const
{
  return MaxLength>=CurrentState->length ? CurrentState : nullptr;
}

Topology::RotationRange * Topology::RotationRange::next()
{
  ++CurrentState;
  return this;
}