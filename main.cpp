#include <iostream>
#include <stdio.h>
#include"rubik.h"

using namespace auxiliary;

int main(int argc, char **argv) 
{ 
    Rubik teszt;
    teszt.REPL(std::cin,std::cout);
    return EXIT_SUCCESS;
}
