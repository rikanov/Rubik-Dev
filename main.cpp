#include <iostream>
#include <stdio.h>
#include"rubik.h"
#include"lift.h"
int main(int argc, char **argv) 
{ 
  Rubik teszt;
  teszt.REPL(std::cin,std::cout);
  OUT_("exit with success...")
  NL_
  return EXIT_SUCCESS;
}
