#include "rubik.h"
#define auto_comp(X) setAutocomp(X);
#define NEW (new CubeSlot[NumberOfSideMarks])

Rubik* Rubik::Global=nullptr;
std::map<String, Rubik*> * Rubik::Collection=new std::map<String, Rubik*>;
std::map<String, String> * Rubik::Var_space =new std::map<String, String>;
Rubik::Rubik(): A_map NEW, B_map NEW, Sup_map NEW, RotationCache NEW, RotationAlign NEW, Sup_inv NEW, Object("global")
{
  Topology::singleton();
  Global=this;
  initCommandComp();
  initStack();
  initRubik();
}

Rubik::Rubik(const String& name): A_map NEW, B_map NEW, Sup_map NEW, Sup_inv NEW, RotationCache NEW, RotationAlign NEW, Object(name)
{
  delete (*Collection)[Object];
  (*Collection)[Object]=this;
  initStack();
  initRubik();
}

void Rubik::initRubik()
{
  LOOP_STACK="";
  BF_SUCCESS="";
  BF_RESULT ="";
  CPY_FUNC(A_map,IdentityMap)
  CPY_FUNC(B_map,IdentityMap)
  noSuppose();
}

void Rubik::initStack()
{
  Stack=new CubeSlot* [MaximumStackDepth];
  FOR_FUNC(index)
  {
    Stack[index]=new CubeSlot[NumberOfSideMarks];
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
  
  auto_comp("get"     )
  auto_comp("defun"   )
  auto_comp("defvar"  )
  auto_comp("define"  )
  auto_comp("defmacro")
  auto_comp("echo"    )

    //==========================================//
   //  *** Swap REPL to a new file stream ***  //
  //==========================================//
  
  auto_comp("%exec_file")
  
    //===========================================//
   //  *** File operations and text editor ***  //
  //===========================================//
  
  auto_comp("save")
  auto_comp("load")
  auto_comp("cat")
  auto_comp("edit")
    
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
  auto_comp("permute"    )
  auto_comp("mirror"     )
  auto_comp("path_finder")
  auto_comp("crypt"      )
  auto_comp("swap"       )
  auto_comp("brute_force")
  auto_comp("random"     )
  auto_comp("reset"      )
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