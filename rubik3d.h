
#ifndef RUBIK3D_H
#define RUBIK3D_H

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #ifdef _WIN32
    #include <windows.h>
  #endif
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/freeglut.h>
#endif

#include "rubik.h"

class Cube3D
{ 
  friend class Rubik3D;
  
  int Colorize[3][3][3]={}; // to be valid exactly one of the coordinates must differ from 1
  
  int& /* side colours references */
    Front = Colorize[1][1][2],
    Up    = Colorize[1][2][1],
    Right = Colorize[2][1][1],
    Left  = Colorize[0][1][1],
    Down  = Colorize[1][0][1],
    Back  = Colorize[1][1][0];  
    
  int /* sides temporarily colours */
    front,
    up,
    right,
    left,
    down,
    back;


  const int PosX,PosY,PosZ;
  GLfloat Corners[8][3]=
  { 
    {-1.0,-1.0,-1.0},
    { 1.0,-1.0,-1.0},
    { 1.0, 1.0,-1.0},
    {-1.0, 1.0,-1.0}, 
    {-1.0,-1.0, 1.0},
    { 1.0,-1.0, 1.0},
    { 1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0}
  };
    
  void facets(const int & color_mark,const int & a1,const int & a2,const int & a3,const int & a4) const;
  void setSideColors();
  
public:
  
  Cube3D(const int& x, const int& y, const int& z, const auxiliary::CubeletColors & colors);
  void show() const;
};
  
class Rubik3D
{
  static Rubik3D * Singleton;
  
  Rubik * TheCube;
  const char * resolver_skript;
  Cube3D * Cublets[3][3][3];
  int axisX, axisY, axisZ;
  bool inverse;
  int theta;
  int haste;
  
  // auxiliary data members to GL graphics  
  static GLfloat phi;
  static GLfloat chi;
  static GLfloat radius;
  static bool moving;
  static bool AutoPlayOn;
  static int beginx;
  static int beginy;
  static GLfloat cameraX;
  static GLfloat cameraY;
  static GLfloat cameraZ;
  static GLfloat upX;
  static GLfloat upY;
  static GLfloat upZ;
  
public:
  explicit Rubik3D(Rubik * TC,const String & R, const int & H=2);
  ~Rubik3D();
    void twister(const int& X, const int& Y, const int& Z, const bool& inv);
  void rotate(); // to be valid exactly one of the coordinates must differ from zero!
  void setColors();
  void showCube();  
  void resolver();
  
  // static OpenGL members
  static void mouse(int btn,int state,int x,int y);
  static void motion(int x, int y);
  static void myreshape(int w,int h);
  static void display();
  static void keyboard(unsigned char key, int, int);
  static void makeMenu();
  static void mymenu(int z);
  static void initLight(void);
  static int init3D();
};
#endif // RUBIK3D_H*/
