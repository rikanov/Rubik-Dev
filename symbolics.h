#ifndef SYMBOLICS__
#define SYMBOLICS__

#include "auxiliary.h"

class Symbol;
typedef std::pair<unsigned int,std::unique_ptr<Symbol>> SymbolSlot;
typedef std::vector<SymbolSlot> SymbolResolver;

class Symbol
{
  SymbolResolver Inside;
};

std::unique_ptr<Symbol> teszt(new Symbol);
#endif