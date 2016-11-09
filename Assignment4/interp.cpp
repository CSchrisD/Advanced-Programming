// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_moveby },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"diamond"  , &interpreter::make_diamond  },
   {"triangle" , &interpreter::make_triangle }, 
   {"equilateral", &interpreter::make_equilateral},
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}
void interpreter::do_border (param begin, param end) {
   DEBUGF ('i', range (begin,end));
   rgbcolor color {begin[0]};
   begin++;
   float i = stof(*begin);
   window::setBorder(color,i);
}

void interpreter::do_moveby (param begin, param end) {
   DEBUGF ('i', range (begin, end)); 
   window::changeMove(stof(*begin));
}
void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}

void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor color {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   //itor->second->draw (where, color);
   object m;
   m.makeObj(itor->second,where,color);
   window::push_back(m);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string x = "";
   bool swap = false;
   for(auto i = begin;i!=end;i++){
      if(swap){
         x = x + " " + *i;
      }else{
         x += *i;
         swap = !swap;
      }
   }
   return make_shared<text> (nullptr, string(x));
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat x = stof(*begin);
   ++begin;
   GLfloat y = stof(*begin);
   return make_shared<ellipse> (GLfloat(x), GLfloat(y));
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat x = stof(*begin);
   return make_shared<circle> (GLfloat(x));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vector<vertex> v;
   for(auto i = begin;i!=end;i++){
      GLfloat x = stof(*i);
      ++i;
      GLfloat y = stof(*i);
      vertex n;
      n.xpos = x;
      n.ypos = y;
      v.push_back(n);
   }
   return make_shared<polygon> (vertex_list(v));
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat w = stof(*begin);
   ++begin;
   GLfloat h = stof(*begin);
   return make_shared<rectangle> (GLfloat(w), GLfloat(h));
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat x = stof(*begin);
   return make_shared<square> (GLfloat(x));
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat w = stof(*begin);
   ++begin;
   GLfloat h = stof(*begin);
   return make_shared<diamond> (GLfloat(w), GLfloat(h));
}

shape_ptr interpreter::make_triangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat x0 = stof(*begin);
   ++begin;
   GLfloat y0 = stof(*begin);
   ++begin;
   GLfloat x1 = stof(*begin);
   ++begin;
   GLfloat y1 = stof(*begin);
   ++begin;
   GLfloat x2 = stof(*begin);
   ++begin;
   GLfloat y2 = stof(*begin);
   return make_shared<triangle> (GLfloat(x0),GLfloat(y0),GLfloat(x1),
   GLfloat(y1),GLfloat(x2),GLfloat(y2));
}

shape_ptr interpreter::make_equilateral(param begin, param end){
   DEBUGF('f', range (begin, end));
   GLfloat x = stof(*begin);
   return make_shared<equilateral> (GLfloat(x));
}
