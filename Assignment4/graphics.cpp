// $Id: graphics.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
vector<object> window::objects;
size_t window::selected_obj = 0;
mouse window::mus;
rgbcolor window::col;
float window::borderWidth;
float window::moveNum = 4.0;

// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
   }
   glutPostRedisplay();
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   for (auto& object: window::objects) {
      object.draw();
   }
   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   DEBUGF ('g', "width=" << width << ", height=" << height);
   window::width = width;
   window::height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}

// Executed when a regular keyboard key is pressed.
enum {BS=8, TAB=9, ESC=27, SPACE=32, DEL=127};
void window::keyboard (GLubyte key, int x, int y) {
   DEBUGF ('g', "key=" << (unsigned)key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   auto object = (window::objects).begin();
   auto t = (window::objects).begin();
   for(size_t i = 0; i<(window::objects).size();i++) {
      if(i == window::selected_obj){
         t -> selected(true);
      }
         t ->selected(false);
         t++;
   }
   for(size_t i = 0; i<(window::objects).size();i++) {
      if(i == window::selected_obj){
         object -> selected(true);
         break;
      }
      object++;
   }
   size_t b;
   float g = (object -> objPos()).xpos;
   float h = (object -> objPos()).ypos;
   switch (key) {
      case 'Q': case 'q': case ESC:
         object = (window::objects).begin();
         window::close();
         break;
      case 'H': case 'h':
         object -> move((GLfloat) -(moveNum), (GLfloat) 0);
         if(g < 0){
            object -> changePos(window::width,h);
         }
         break;
      case 'J': case 'j':
         object -> move((GLfloat) 0, (GLfloat) -(moveNum));
         if(h < 0){
            object -> changePos(g,window::height);
         }
         break;
      case 'K': case 'k':
         object -> move((GLfloat) 0, (GLfloat) moveNum);
         if(h > window::height){
            object -> changePos(g,0);
         }
         break;
      case 'L': case 'l':
         object -> move((GLfloat) moveNum, (GLfloat) 0);
         if(g > window::width){
            object -> changePos(0,h);
         }
         break;
      case 'N': case 'n': case SPACE: case TAB:
         if(window::selected_obj == (window::objects).size()-1){
            window::selected_obj = 0;
         }else{
            window::selected_obj = window::selected_obj + 1;
         }
         break;
      case 'P': case 'p': case BS:
         if(window::selected_obj == 0){
            window::selected_obj = (window::objects).size()-1;
         }else{
            window::selected_obj = window::selected_obj - 1;
         }
         break;
      case '0'...'9':
         b = key - '0';
         window::selected_obj = b;
         break;
      default:
         cerr << (unsigned)key << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}

// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   auto object = (window::objects).begin();
   for(size_t i = 0; i<(window::objects).size();i++) {
      if(i == window::selected_obj){
         break;
      }
      object++;
   }
   float g = (object -> objPos()).xpos;
   float h = (object -> objPos()).ypos;
   switch (key) {
      case GLUT_KEY_LEFT:object->move((GLfloat)-(moveNum),(GLfloat)0);
           if(g < 0){
              object -> changePos(window::width,h);
           }
           break;
           //move_selected_object (-1, 0); break;
      case GLUT_KEY_DOWN:object->move((GLfloat)0,(GLfloat)-(moveNum));
           if(h < 0){
              object -> changePos(g,window::height);
            }
           break;
           //move_selected_object (0, -1); break;
      case GLUT_KEY_UP:object->move((GLfloat)0,(GLfloat)moveNum);
           if(h > window::height){
              object -> changePos(g,0);
           }
           break;
           //move_selected_object (0, +1); break;
      case GLUT_KEY_RIGHT:object->move((GLfloat)moveNum,(GLfloat)0);
           if(g > window::width){
              object -> changePos(0,h);
           }
           break; 
           //move_selected_object (+1, 0); break;
      case GLUT_KEY_F1: //select_object (1); break;
      case GLUT_KEY_F2: //select_object (2); break;
      case GLUT_KEY_F3: //select_object (3); break;
      case GLUT_KEY_F4: //select_object (4); break;
      case GLUT_KEY_F5: //select_object (5); break;
      case GLUT_KEY_F6: //select_object (6); break;
      case GLUT_KEY_F7: //select_object (7); break;
      case GLUT_KEY_F8: //select_object (8); break;
      case GLUT_KEY_F9: //select_object (9); break;
      case GLUT_KEY_F10: //select_object (10); break;
      case GLUT_KEY_F11: //select_object (11); break;
      case GLUT_KEY_F12: //select_object (12); break;
      default:
         cerr << (unsigned)key << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}

void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   static int argc = 0;
   window::selected_obj = 0;
   rgbcolor c("red");
   window::setBorder(c,4.0);
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}

void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      glutBitmapString (font, (GLubyte*) text.str().c_str());
   }
}
