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

#include <iostream>
#include <cmath>
#include "rubik3d.h"

static const int PI=3.14156;
static inline GLfloat toRadian(const int & alfa)
{
  return PI*alfa/180.0f;
}
void Rubik3D::makeMenu()
{  
  glutAddMenuEntry("Right                 R   ", 1);
  glutAddMenuEntry("Right Inv.       shift+R", 2);
  glutAddMenuEntry("Up                     U   ", 3);
  glutAddMenuEntry("Up Inv.           shift+U", 4);
  glutAddMenuEntry("Front                 F   ", 5);
  glutAddMenuEntry("Front Inv.       shift+F", 6);
  glutAddMenuEntry("Left                    L   ", 7);
  glutAddMenuEntry("Left Inv.          shift+L", 8);
  glutAddMenuEntry("Down                D   ",9);
  glutAddMenuEntry("Down Inv       shift+D",10);
  glutAddMenuEntry("Back                 B    ",11);
  glutAddMenuEntry("Back Inv.       shift+B",12);
  glutAddMenuEntry("Run skript...   shift+S",13);
  glutAddMenuEntry("Exit",14);
}
void Rubik3D::mouse(int btn,int state,int x,int y)
{
  if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
  {
    moving = true;
    beginx = x;
    beginy = y;
  }
}
void Rubik3D::motion(int x, int y)
{
  if (moving) 
  {
    chi-= (x - beginx);
    beginx = x;
    phi-= (y - beginy);
    beginy=y;
    GLfloat rad_phi=toRadian(phi);
    GLfloat rad_chi=toRadian(chi);
    cameraZ=radius*sin(rad_phi)*cos(rad_chi);
    cameraX=radius*sin(rad_phi)*sin(rad_chi);
    cameraY=radius*cos(rad_phi);
    
    upZ=sin(rad_phi-PI/2)*cos(rad_chi);
    upX=(rad_phi-PI/2)*sin(rad_chi);
    upY=radius*cos(rad_phi-PI/2);
    
    glutPostRedisplay();
  }
}

void Rubik3D::myreshape(int w,int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
  {
    glOrtho(-10.0,10.0,-10.0*(GLfloat)h/(GLfloat)w, 10.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
  }
  else
  {
    glOrtho(-10.0*(GLfloat)w/(GLfloat)h, 10.0*(GLfloat)w/(GLfloat)h,-10.0,10.0,-10.0,10.0);
  }
  glMatrixMode(GL_MODELVIEW);
}
void Rubik3D::display()
{
  if(AutoPlayOn)
  {
    AutoPlayOn=false;
    Singleton->resolver();
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glPushMatrix();
  gluLookAt(cameraX,cameraY,cameraZ,0.0f,0.0f,0.0f,upX,upY,upZ);
  
  applySolution();
  showCube();
  
  glFlush();
  glutSwapBuffers();
}

void Rubik3D::keyboard(unsigned char key, int, int)
{
  if(haveSolution())
  {
    return; // don't rotate until the Cube is solved!
  }
  if(key=='S')
  {
    AutoPlayOn=true;
    return;
  }
  int x=0,y=0,z=0;
  bool inv=(key & 32)==0;
  key &= 95;
  convertToCoordinates(key,x,y,z);
  Singleton->twister(x,y,z,inv);
}

void Rubik3D::mymenu(int ID)
{
  const bool inv=1-(ID%2); // even numbers
  int x=0,y=0,z=0;
  switch(ID)
  {
    case 1:
    case 2:
      x=1;
      break;
    case 3:
    case 4:
      y=1;
      break;
    case 5:
    case 6:
      z=1;
      break;
    case 7:
    case 8:
      x=-1;
      break;
    case 9:
    case 10:
      y=-1;
      break;
    case 11:
    case 12:
      z=-1;
      break;
    case 13:
      AutoPlayOn=true;
      break;
    case 14:
      glutLeaveMainLoop();
      return;
    default:
      ;	
  }
  if(AutoPlayOn==false && haveSolution()==false)
  {
    Singleton->twister(x,y,z,inv);
  }
}

int Rubik3D::init3D() 
{
  int myargc=1;
  char * myargv[1];
  myargv[0]=strdup("3dvisualization");
  glutInit(&myargc, myargv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (640, 600);
  glutCreateWindow ("RUBIK'S CUBE");
  glClearColor(0.0f,0.0f,0.0f, 1.0f);
  glutPostRedisplay();
  myreshape(640,640);
  glutReshapeFunc (myreshape);
  glutIdleFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutCreateMenu(mymenu);
  makeMenu();
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc (display);
  glEnable(GL_DEPTH_TEST);
  glutMainLoop(); 
  OUT_("\nopenGL main loop has been closed...")
  free(myargv[0]);
  return 0;
}
