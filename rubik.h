#ifndef RUBIK_CLASS
#define RUBIK_CLASS

#include<readline/readline.h>
#include<readline/history.h>
#include"auxiliary.h"

#define UI_func(X) String X(const String & S); String X(Stream& IS);
#define UI_rfunc(X) String Rubik::X(const String & S) {Stream IS(S); return X(IS);} String Rubik::X(Stream& IS)
#define CALL_BACK(SN,F,A) Stream SN; {Stream eval__(A);SN.str(F(eval__));}
#define GET(A) String A; IS>>A;
#define GET2(A,B) String A,B; IS>>A; IS>>B;
#define GETLIST(L) Stream L(list(IS));
#define PARSER(A) String A=parser(IS);
#define PARSER2(A,B) PARSER(A) PARSER(B)
#define GETLINE(L) String L;getline(IS,L,';');TRIM_END(L);{String ToEnd; getline(IS,ToEnd); if(ToEnd!="") {IS.str(ToEnd);IS.clear();}}
#define OUTSPREAD(O) String O; getline(IS,O);
#define LOOP_STACK (*Var_space)["loop-stack"]
#define BF_SUCCESS (*Var_space)["brute-force-success"]
#define BF_RESULT  (*Var_space)["brute-force-result"]
#define BF_WIDTH   std::max(atoi((*Var_space)["brute-force-width"].c_str()),10000)

using namespace auxiliary;

class Rubik_BF;
class Rubik 
{
  friend class Rubik_BF;
  static std::map<String, Rubik*> * Collection; 
  static std::map<String, String> * Var_space;
  
  static char ** commandCompletion (const char * stem_text , int start , int end );
  static char *  getCommands       (const char * stem_text ,int state ); 
  static void    setAutocomp       (const char * Add_text);
  
  static Rubik* Global;
  
  const String Object;
  
		// Let I the index of a certain side mark in the current state 
		// of the Rubik's Cube. Then, let:
  CubeSlot * A_map; 	// indices of side marks what we need here, 
  CubeSlot * B_map;	// indices of side marks what ARE actually here

  CubeSlot * Sup_map;  // to find partial solutions we need supposations
  CubeSlot * Sup_inv;  // by default they both are identical mappings. (no supposation)  

  static const int MaximumStackDepth=256;
  CubeSlot ** Stack;
  int stack_pointer=0;
  
  CubeSlot * RotationCache;
  CubeSlot * RotationAlign;
  bool NeedAlign;

  void 
  initRubik		();
  
  void 
  initStack		();
  
  void 
  initCommandComp	();
  
  void 
  printCollection	() const;
  
  bool 
  is_solved		(const CubeSlot * Cubes = IdentityMap, 
			 const int & Limit = NumberOfSideMarks) const;
			 
  bool 
  variableEquality	(
			String & A, 
			const String & B) const;
  void 
  variable		(
			Stream & IS, 
			String & R);
  void 
  macro			(
			Stream & IS, 
			String & R);
  
  void 
  setRotation		(const String& Rot);
  
  Rubik&
  applyRotation		();
  
  String 
  functionResolver	(
			Stream & IS, 
			const String & R);
  
  void 
  select		(
			Stream & IS, 
			String & Result, 
			const bool & Inv); 
  
  void 
  noSuppose		();
  
  void 
  suppose		(const Sidemarks & S);
  
  void 
  align			(
			const Sidemarks & A, 
			const Sidemarks & B);
  
  Sidemarks 
  whatIs		(const Sidemarks & S) const;
			
  Sidemarks 
  locationOf		(const Sidemarks & S) const;

  void 
  swapTwoPieces         (const Sidemarks& A, const Sidemarks& B);
  
  String 
  bruteForce		(
			Stream& IS, 
			const String& AS) const;
			
  static String
  random		();
  
     //==========================//
    //  *** User Interface ***  //
   //==========================//
  UI_func(REPL)
  UI_func(parser)
  UI_func(file_open)
  UI_func(editor)
  
  UI_func(progn)
  UI_func(list)
  UI_func(getValue)
  UI_func(defvar)
  UI_func(defun)
  UI_func(defmacro)
  UI_func(delete_var)
  UI_func(nilEquality)
  UI_func(variableEquality)
  UI_func(listEquality)
  UI_func(parsingEquality)
  UI_func(condition)
  UI_func(forLoop)
  UI_func(logicalOr)
  UI_func(logicalXor)
  UI_func(logicalAnd)
  
  UI_func(assoc)
  UI_func(select)
  UI_func(deselect)
  UI_func(mapcar)
  UI_func(conc)
  UI_func(stringReplace)
  UI_func(regExp)
  UI_func(regExpFast)
  UI_func(echo)
  UI_func(catFiles)
  UI_func(saveCube)
  UI_func(loadCube)
    
  UI_func(sameCubes)
  UI_func(merge)
  UI_func(permute)
  UI_func(mirror)
  UI_func(pathFinder)
  UI_func(crypt)
  UI_func(swap)
  UI_func(callBruteForce)
  UI_func(random)
  
  UI_func(what_is)
  UI_func(where_is)
  UI_func(cube)
  UI_func(solvedp)
  UI_func(doRotations)
  UI_func(store)
  UI_func(revert)
  UI_func(setAlign)
  UI_func(printSmarks)
  
public:
  
  Rubik  ();
  Rubik  (const Rubik&) =default;
  Rubik  (const String& Name);
  ~Rubik ();
 
  Rubik & operator =  (const Rubik& R)=default;
  
  void print(const String & C ) const;
  void REPL (std::istream & IS=std::cin, std::ostream & OS=std::cout);
  
  String file_open(const char * F);
};

#endif