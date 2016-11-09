// $Id: bigint.cpp,v 1.73 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+() const {
   return *this;
}

bigint bigint::operator-() const {
   return {uvalue, not is_negative};
}

//new + operator for vectors
bigint bigint::operator+ (const bigint& that) const {
  bigint result;
   if(is_negative == that.is_negative){
      result = uvalue + that.uvalue;
      result.is_negative = that.is_negative;
   }else if(uvalue >= that.uvalue){//opposite signs
      result = uvalue - that.uvalue;
      result.is_negative = !that.is_negative;
   }else if(that.uvalue > uvalue){//opposite signs
      result = that.uvalue - uvalue;
      result.is_negative = that.is_negative;
   }
   return result;
}

//new - operator
bigint bigint::operator- (const bigint& that) const {
   bigint result;
   if(is_negative != that.is_negative){
      if(uvalue >= that.uvalue){
         result = uvalue + that.uvalue;
         result.is_negative = !that.is_negative;
      }else{
         result = that.uvalue + uvalue;
         result.is_negative = is_negative;
      }
   }else{
      if(uvalue >= that.uvalue){
         result = uvalue - that.uvalue;
         result.is_negative = that.is_negative;
      }else if(that.uvalue > uvalue){
         result = that.uvalue - uvalue;
         result.is_negative = !that.is_negative;
      }
   }
   return result;
}

//new * operator
bigint bigint::operator* (const bigint& that) const {
   bigint result = uvalue * that.uvalue;
   if(is_negative == that.is_negative){
      result.is_negative = false;
   }else{
      result.is_negative = true;
   }
   return result;   
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result = uvalue / that.uvalue;
   if(is_negative == that.is_negative){
     result.is_negative = false;
   }else{
     result.is_negative = true;
   }
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result = uvalue % that.uvalue;
   if(is_negative == that.is_negative){
      result.is_negative = false;
   }else{
      result.is_negative = true;
   }
   return result;
}

//new == operator
bool bigint::operator== (const bigint& that) const {
   bool isSame = false;
   if(is_negative == that.is_negative){
      isSame = (uvalue == that.uvalue);
   }
   return isSame;
}

//new < operator
bool bigint::operator< (const bigint& that) const{
   bool isLess = false;
   if((is_negative == false) and (that.is_negative == false)){
      isLess = uvalue < that.uvalue;
   }else if((is_negative == false) and (that.is_negative == true)){
      isLess = false;
   }else if((is_negative == true) and (that.is_negative == false)){
      isLess = true;
   }else if((is_negative == true) and (that.is_negative == true)){
      isLess = uvalue < that.uvalue;
      if(isLess == false){
         isLess = true;
      }else if(isLess == true){
         isLess = false;
      }
   }
   return isLess;
}

ostream& operator<< (ostream& out, const bigint& that) {
   if(that.uvalue == 0){
      return out << "0";
   }
   return out  << (that.is_negative ? "-" : "")
              << that.uvalue;
}
