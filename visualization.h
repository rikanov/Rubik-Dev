#ifndef RUBIK_3DVISUALIZATION
#define RUBIK_3DVISUALIZATION
#include "rubik.h"

namespace RUBIK_3D
{ 
  void makeMenu();
  void mouse(int btn,int state,int x,int y);
  void motion(int x, int y);
  void myreshape(int w,int h);
  void display();
  void keyboard(unsigned char key, int, int);
  void mymenu(int z);
  void initLight(void);
  int visualization(Rubik * cube, int argc, char **argv);
}


#endif
