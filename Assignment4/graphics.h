// $Id: graphics.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
      bool select;
   public:
      // Default copiers, movers, dtor all OK.
      void makeObj(const shared_ptr<shape>& a,vertex& b,rgbcolor& c)
       {pshape = a;center = b;color = c;}
      void draw() { pshape->draw (center, color,select); }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
      vertex objPos () { return center; }
      void changePos (GLfloat x, GLfloat y)
        { center.xpos = x; center.ypos = y; }
      rgbcolor objColor() {return color;}
      void selected(bool state) { select = state;}
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};

class window {
      friend class mouse;
   private:
      static int width;         // in pixels
      static int height;        // in pixels
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
      static float borderWidth;
      static float moveNum;
      static rgbcolor col;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static void push_back (const object& obj) {
                  objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void main();
      static void changeMove (float x) { moveNum = x; }
      static void setBorder(rgbcolor x, float y)
                  {col = x; borderWidth = y;}
      static float getWid () {return borderWidth;}
      static rgbcolor getCol() {return col;}
};

#endif
