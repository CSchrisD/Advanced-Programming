// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <cstring>
#include <memory>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}

polygon::polygon (const vertex_list& vertices):vertices(vertices) {
   DEBUGF ('c', this);
}

rectangle::rectangle(GLfloat width, GLfloat height):
   polygon({vertex_list{vertex{width,height}}}){
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat width, GLfloat height):
   polygon({vertex_list{vertex{width,height}}}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

triangle::triangle (GLfloat x0, GLfloat y0, GLfloat x1,
           GLfloat y1, GLfloat x2, GLfloat y2): polygon({vertex_list{
     vertex{x0,y0},
     vertex{x1,y1},
     vertex{x2,y2}}}){
   DEBUGF ('c', this);
}

equilateral::equilateral(GLfloat x0):triangle(x0,x0,x0,x0,x0,x0){
   DEBUGF ('c', this);
}

void text::draw (const vertex& center,
    const rgbcolor& color, const bool& select) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   string fo = textdata.substr(0,textdata.find_first_of(" "));
   string sentence=textdata.substr(textdata.find_first_of(" ")+1);
   unsigned char unString[sentence.length()+1];
   strcpy((char*)unString,sentence.c_str());
   auto i = fontcode.find(fo);
   if(i != fontcode.end()){
      glBegin(GL_LINE);
         glColor3f(color.red,color.green,color.blue);
         glRasterPos2f(center.xpos, center.ypos);
         glutBitmapString(i->second,unString);
      glEnd();
   }
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
         rgbcolor c = window::getCol();
         glColor3f(c.red,c.green,c.blue);
         glRasterPos2f(center.xpos, center.ypos);
         glutBitmapString(i->second,unString);
      glEnd();
   }
}

void ellipse::draw (const vertex& center,
    const rgbcolor& color, const bool& select)const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
      glColor3f(color.red,color.green,color.blue);
      for (float x = 0; x <2* (3.14); x += (3.14 / 24)) {
         glVertex2f (center.xpos + sin(x)*(dimension.xpos/2),
            center.ypos+ cos(x)*(dimension.ypos/2));
     }
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
      rgbcolor c = window::getCol();
      glColor3f(c.red,c.green,c.blue);
      for (float x = 0; x <2* (3.14); x += (3.14 / 24)) {
         glVertex2f (center.xpos + sin(x)*(dimension.xpos/2),
            center.ypos+ cos(x)*(dimension.ypos/2));
      }
      glEnd();
   }
}

void circle::draw (const vertex& center, 
     const rgbcolor& color, const bool& select) const {
   glBegin(GL_POLYGON);  
     glColor3f(color.red,color.green,color.blue);
     for (float x = 0; x <2* (3.14); x += (3.14 / 24)) {
         glVertex2f(center.xpos + sin(x)*(dimension.xpos/2),
          center.ypos+ cos(x)*(dimension.ypos/2));
     }
   glEnd();
   if(select){
     glLineWidth(window::getWid());
     glBegin(GL_LINE_LOOP);
     rgbcolor c = window::getCol();
     glColor3f(c.red,c.green,c.blue);
     for (float x = 0; x <2* (3.14); x += (3.14 / 24)) {
         glVertex2f(center.xpos + sin(x)*(dimension.xpos/2),
          center.ypos+ cos(x)*(dimension.ypos/2));
     }
     glEnd();
   }
}

void rectangle::draw(const vertex& center,
    const rgbcolor& color,const bool& select)const{
   GLfloat x = vertices.begin() -> xpos;
   GLfloat y = vertices.begin() -> ypos;
   glBegin(GL_POLYGON);
      glColor3f(color.red,color.green,color.blue);
      glVertex2f(center.xpos - (x/2), center.ypos - (y/2));
      glVertex2f(center.xpos - (x/2), center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos - (y/2));
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
      rgbcolor c = window::getCol();
      glColor3f(c.red,c.green,c.blue);
      glVertex2f(center.xpos - (x/2), center.ypos - (y/2));
      glVertex2f(center.xpos - (x/2), center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos - (y/2));
   glEnd();
   }
}

void square::draw (const vertex& center,
     const rgbcolor& color, const bool& select) const {
   GLfloat x = vertices.begin() -> xpos;
   glBegin(GL_POLYGON);   
      glColor3f(color.red,color.green,color.blue);
      glVertex2f (center.xpos - (x/2), center.ypos - (x/2));
      glVertex2f (center.xpos - (x/2), center.ypos + (x/2));
      glVertex2f (center.xpos + (x/2), center.ypos + (x/2));    
      glVertex2f (center.xpos + (x/2), center.ypos - (x/2));
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
         rgbcolor c = window::getCol();
         glColor3f(c.red,c.green,c.blue);
         glVertex2f (center.xpos - (x/2), center.ypos - (x/2));
         glVertex2f (center.xpos - (x/2), center.ypos + (x/2));
         glVertex2f (center.xpos + (x/2), center.ypos + (x/2));
         glVertex2f (center.xpos + (x/2), center.ypos - (x/2));
      glEnd();
   }
}

void diamond::draw(const vertex& center, 
    const rgbcolor& color,const bool& select)const{
   GLfloat x = vertices.begin() -> xpos;
   GLfloat y = vertices.begin() -> ypos;
   glBegin(GL_POLYGON);
      glColor3f(color.red,color.green,color.blue);
      glVertex2f(center.xpos - (x/2), center.ypos);
      glVertex2f(center.xpos, center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos);
      glVertex2f(center.xpos, center.ypos - (y/2));
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
      rgbcolor c = window::getCol();
      glColor3f(c.red,c.green,c.blue);
      glVertex2f(center.xpos - (x/2), center.ypos);
      glVertex2f(center.xpos, center.ypos + (y/2));
      glVertex2f(center.xpos + (x/2), center.ypos);
      glVertex2f(center.xpos, center.ypos - (y/2));
   glEnd();
   }
}

void triangle::draw(const vertex& center, 
      const rgbcolor& color,const bool& select)const{
   glBegin(GL_POLYGON);
   glColor3f(color.red,color.green,color.blue);
   GLfloat x;
   GLfloat y;
   for(auto i = vertices.begin();i!=vertices.end();i++){
         x += i ->xpos;
         y += i ->ypos;
      }
      GLfloat xavg = x/vertices.size();
      GLfloat yavg = y/vertices.size();
      for(auto i = vertices.begin();i!=vertices.end();i++){
         glVertex2f(center.xpos + (i->xpos)-xavg,
         center.ypos + (i->ypos) - yavg);
      }
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
      rgbcolor c = window::getCol();
      glColor3f(c.red,c.green,c.blue);
      GLfloat x;
      GLfloat y;
      for(auto i = vertices.begin();i!=vertices.end();i++){
         x += i ->xpos;
         y += i ->ypos;
      }
      GLfloat xavg = x/vertices.size();
      GLfloat yavg = y/vertices.size();
      for(auto i = vertices.begin();i!=vertices.end();i++){
         glVertex2f(center.xpos + (i->xpos)-xavg,
         center.ypos + (i->ypos) - yavg);
      }
   glEnd();
   }
}

void equilateral::draw(const vertex& center,
    const rgbcolor& color,const bool& select)
  const{
   GLfloat halfBase = pow(vertices.begin() -> xpos /2, 2);
   GLfloat height = pow(vertices.begin() ->xpos,2);
   GLfloat halfHeight = sqrt(height - halfBase) /2 ;
   glBegin(GL_POLYGON);
   glColor3f(color.red,color.green,color.blue);
   glVertex2f(center.xpos - (vertices.begin() ->xpos / 2),
       center.ypos - (halfHeight));
   glVertex2f(center.xpos + (vertices.begin() ->xpos / 2),
        center.ypos - (halfHeight));
   glVertex2f(center.xpos, center.ypos + (halfHeight));
   glEnd();
   if(select){
   glLineWidth(window::getWid());
   glBegin(GL_LINE_LOOP);
   rgbcolor c = window::getCol();
   glColor3f(c.red,c.green,c.blue);
   glVertex2f(center.xpos - (vertices.begin() ->xpos / 2),
       center.ypos - (halfHeight));
   glVertex2f(center.xpos + (vertices.begin() ->xpos / 2),
        center.ypos - (halfHeight));
   glVertex2f(center.xpos, center.ypos + (halfHeight));
   glEnd();
   }
}

void polygon::draw (const vertex& center, const rgbcolor& color,
     const bool& select) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
      GLfloat x;
      GLfloat y;
      glColor3f(color.red,color.green, color.blue);
      for(auto i = vertices.begin();i!=vertices.end();i++){
         x += i ->xpos;
         y += i ->ypos;
      }
      GLfloat xavg = x/vertices.size();
      GLfloat yavg = y/vertices.size();
      for(auto i = vertices.begin();i!=vertices.end();i++){
         glVertex2f(center.xpos+(i->xpos)-xavg,
         center.ypos+(i->ypos)-yavg);
      }
   glEnd();
   if(select){
      glLineWidth(window::getWid());
      glBegin(GL_LINE_LOOP);
      GLfloat x;
      GLfloat y;
      rgbcolor c = window::getCol();
      glColor3f(c.red,c.green, c.blue);
      for(auto i = vertices.begin();i!=vertices.end();i++){
         x += i ->xpos;
         y += i ->ypos;
      }
      GLfloat xavg = x/vertices.size();
      GLfloat yavg = y/vertices.size();
      for(auto i = vertices.begin();i!=vertices.end();i++){
         glVertex2f(center.xpos+(i->xpos)-xavg,
         center.ypos+(i->ypos)-yavg);
      }
   glEnd();
   }
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}
