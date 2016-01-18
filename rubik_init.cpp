#include <time.h>
#include "rubik.h"
#define auto_comp(X) setAutocomp(X);
#define NEW (new int[NumberOfSideMarks])

Rubik* Rubik::Global=nullptr;
std::map<String, Rubik*> * Rubik::Collection=new std::map<String, Rubik*>;
std::map<String, String> * Rubik::Var_space =new std::map<String, String>;
Rubik::Rubik(): A_map NEW, B_map NEW, Sup_map NEW, Sup_inv NEW, Object("global")
{
  Topology::singleton();
  initCommandComp();
  Global=this;
  initRubik();
}

Rubik::Rubik(const String& name): A_map NEW, B_map NEW, Sup_map NEW, Sup_inv NEW, Object(name)
{
  delete (*Collection)[Object];
  (*Collection)[Object]=this;
  initRubik();
}

void Rubik::initRubik()
{
  initStack();
  CPY_FUNC(A_map,IdentityMap)
  CPY_FUNC(B_map,IdentityMap)
  noSuppose();
  srand(time(NULL));
}

void Rubik::initStack()
{
  Stack=new int* [MaximumStackDepth];
  FOR_FUNC(index)
  {
    Stack[index]=new int[NumberOfSideMarks];
  }

}

///===========================================================================


void Rubik::initCommandComp()
{
    //=================================//
   //  *** Collection management ***  //
  //=================================//
  
  auto_comp("REPL")
  auto_comp("collection_view")
  
    //=======================================//
   //  *** Make a new list from input  ***  //
  //=======================================//
  
  auto_comp("list")
  
    //=========================================//
   //  *** LISP command PROGN for blocks ***  //
  //=========================================//
  
  auto_comp("progn")

    //====================================//
   //  *** LISP equality predicates ***  //
  //====================================//
  
  auto_comp("nil")
  auto_comp("eq" )
  auto_comp("eql")
  auto_comp("eqp")
  auto_comp("or" )
  auto_comp("xor")
  auto_comp("and") 
  
    //========================================//
   //  *** User-space functions, macros ***  //
  //========================================//
  
  auto_comp("defun"   )
  auto_comp("defvar"  )
  auto_comp("define"  )
  auto_comp("defmacro")
  auto_comp("echo"    )

    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  
  auto_comp("%exec_file")
  
    //=======================================//
   //  *** Evaluate built-in functions ***  //
  //=======================================//
  
  auto_comp("if"      )
  auto_comp("for_in"  )
  auto_comp("assoc"   )
  auto_comp("mapcar"  )
  auto_comp("select"  )
  auto_comp("deselect")
  auto_comp("conc"    )
  auto_comp("replace" )
  auto_comp("regex"   )
  auto_comp("reg_fast")
  
    //=========================================//
   //  *** Seeker and auxiliary functions *** //
  //=========================================//
  
  auto_comp("same_cubes" )
  auto_comp("merge"      )
  auto_comp("path_finder")
  auto_comp("brute_force")
  auto_comp("random"     )
  auto_comp("side_marks" )
  
    //=====================================//
   //  *** Evaluate object functions ***  //
  //=====================================// 
 
  auto_comp("do"        )
  auto_comp("align"     )
  auto_comp("no_suppose")
  auto_comp("suppose"   ) 
  auto_comp("store"     )
  auto_comp("revert"    )
  auto_comp("print"     ) 
  auto_comp("solvedp"   )
  auto_comp("cube"      )
  auto_comp("what_is"   )
  auto_comp("where_is"  )
}