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
#include "rubik3d.h"

namespace RUBIK_3D 
{
  GLfloat p=0.0,q=0.0,r=0.0;
  bool moving=false;
  int beginx=0, beginy=0;
  Rubik3D * rubik;
    
  void makeMenu()
  {
    
    glutAddMenuEntry("Right                 R   ", 1);
    glutAddMenuEntry("Right Inv.       shift+R", 2);
    glutAddMenuEntry("Up                    U   ", 3);
    glutAddMenuEntry("Up Inv.           shift+U", 4);
    glutAddMenuEntry("Front                 F   ", 5);
    glutAddMenuEntry("Front Inv.       shift+F", 6);
    glutAddMenuEntry("Left                    L   ", 7);
    glutAddMenuEntry("Left Inv.          shift+L", 8);
    glutAddMenuEntry("Down                 D   ",9);
    glutAddMenuEntry("Down Inv       shift+D",10);
    glutAddMenuEntry("Back                  B    ",11);
    glutAddMenuEntry("Back Inv.       shift+B",12);
    glutAddMenuEntry("Ask server...  shift+A",13);
    glutAddMenuEntry("Exit",14);
  }
  void mouse(int btn,int state,int x,int y)
  {
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
      moving = 1;
      beginx = x;
      beginy = y;
    }
  }
  void motion(int x, int y)
  {
    if (moving) 
    {
      q+= (x - beginx);
      beginx = x;
      p+= (y - beginy);
      beginy=y;
      glutPostRedisplay();
    }
  }

  void myreshape(int w,int h)
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
  void display()
  {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();

      glPushMatrix();
      glRotatef(25.0+p,1.0,0.0,0.0);
      glRotatef(-30.0+q,0.0,1.0,0.0);
      glRotatef(0.0+r,0.0,0.0,1.0);   
      
      rubik->showCube();
      
      glFlush();
      glutSwapBuffers();
  }

  void keyboard(unsigned char key, int, int)
  {
    int x=0,y=0,z=0;
    bool inv=(key & 32)==0;
    key |= 32;
    switch(key)
    {
      case 'l':
	x = -1;
	break;
      case 'r':
	x = 1;
	break;
      case 'd':
	y =-1;
	break;
      case 'u':
	y = 1;
	break;
      case 'b':
	z =-1;
	break;
      case 'f':
	z = 1;
	break;
      default:
	return;
    }
    rubik->twister(x,y,z,inv);
  }

  void mymenu(int ID)
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
	// TODO
	break;
      case 14:
	glutLeaveMainLoop();
	return;
      default:
	;	
    }
    rubik->twister(x,y,z,inv);
  }

  void initLight(void) 
  {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { -1.0, 5.0, 5.0, 0.0 };
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
  }

  int visualization(Rubik * cube, int argc, char **argv) 
  {
    rubik=new Rubik3D(cube);
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (640, 600);
    glutCreateWindow ("RUBIK'S CUBE");
    glClearColor(0.0f,0.0f,0.0f, 1.0f);
    glutPostRedisplay();
  // initLight();
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
    delete rubik;
    return 0;
  }
}