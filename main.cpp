#include <iostream>
#include <stdio.h>
#include"rubik.h"

int main(int argc, char **argv) 
{ 

  intro();
  Rubik global;
  if(argc==1)
  {
    global.file_open("autorun.lisp");
  }
  else
  {
    for(int i=1;i<argc;++i)
    {
      global.file_open(argv[i]);
    }
  }
  Topology::close();
  return 0;
}
