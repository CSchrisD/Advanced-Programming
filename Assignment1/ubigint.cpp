// $Id: ubigint.cpp,v 1.8 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): ubig_value (that) {
    string num;
   for(int j = 0; j < (signed int) ubig_value.size(); j++){
      num = string(1,ubig_value[j]) + num;;
   }
   DEBUGF('~', this << " -> " << num)
   //DEBUGF ('~', this << " -> " << ubig_value)
}

//new debug operator
/*ubigint::ubigint (ubigvalue_t that): ubig_value (that){
   string num;
   for(int j = 0; j < (signed int) ubig_value.size(); j++){
      num = string(1,ubig_value[j]) + num;;
   }   
   DEBUGF('~', this << " -> " << num)
}
*/

//new constructor operator
ubigint::ubigint (const string& that): ubig_value(0) {
   for (char digit: that) ubig_value.insert(ubig_value.begin(),digit);
}

//new +operator
ubigint ubigint::operator+ (const ubigint& that) const {
   ubigint result;
   int value;
   int remainder;
   int carry = 0;
   int len = max(ubig_value.size(), that.ubig_value.size());
   for(int j = 0; j < len; j++){
      int d = j < (signed int) ubig_value.size() ?
          (int(ubig_value[j])-48):(0);
      int e = j < (signed int) that.ubig_value.size() ?
          (int(that.ubig_value[j])-48):(0);
      value = d + e + carry;
      carry = 0;
      if(value >= 10){
         remainder = value - 10;
         carry = 1;
         value = remainder;
      }
      result.ubig_value.push_back(value+48);
   }
   if(carry == 1){
      result.ubig_value.push_back('1');
   }
   while((result.ubig_value.size() > 0)
        and (result.ubig_value.back() == '0')){
      result.ubig_value.pop_back();
   }
   return result;
}

//new - operator
ubigint ubigint::operator- (const ubigint& that) const{
   ubigint result;
   int value;
   int carry = 0;
   int len = max(ubig_value.size(), that.ubig_value.size());
   for(int j = 0; j < len; j++){
      int d = j < (signed int) ubig_value.size()?
         (int(ubig_value[j])-48):(0);
      int e = j < (signed int) that.ubig_value.size()?
         (int(that.ubig_value[j])-48):(0);
      value = d - e - carry;
      if(value == -1){
         value = value + 10;
         carry = 1;
      }else{
         carry = 0;
      }
      if(d < e){
         value = value + 10;
         carry = 1;
      }
      result.ubig_value.push_back(value+'0');
   }
   if(carry == 1){
      result.ubig_value.push_back('1');
   }
   while((result.ubig_value.size()>0)
        and (result.ubig_value.back() == '0')){
      result.ubig_value.pop_back();
   }
   return result;
}

//new * operator
ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result;
   int j, k, c;
   int p = 1;
   int vSize = (signed int)ubig_value.size();
   int uSize = (signed int)that.ubig_value.size();
   int size = vSize + uSize;
   while(p <= size){
      result.ubig_value.push_back('0');
      p++;
   }
   for(j = 0; j < vSize; j++){
      c = 0;
      for(k = 0; k < uSize; k++){
         int d = ((int)result.ubig_value[j+k] - 48) +
            ((int)ubig_value[j]-48)*((int)that.ubig_value[k]-48) + c;
         result.ubig_value[j+k] = (d % 10) + 48;
         c = d / 10;
      }
      result.ubig_value[j+uSize] = c + 48;
   }
   while((result.ubig_value.size()>0)
          and (result.ubig_value.back()=='0')){
      result.ubig_value.pop_back();
   }
   return result;
}

//new multiply by 2
void ubigint::multiply_by_2(){
   int value;
   int carry = 0;
   for(int j = 0; j < (signed int) ubig_value.size(); j++){
      value = ((int(ubig_value[j])-48) * 2) + carry;
      carry = 0;
      if(value >= 10){
         value = value - 10;
         carry = 1;
      }
      ubig_value[j] = value + 48;
   }
   if(carry == 1){
      ubig_value.push_back('1');
   }
   while((ubig_value.size()>0)
          and (ubig_value.back()=='0')){
      ubig_value.pop_back();
   }
}

//new divide by 2
void ubigint::divide_by_2(){
   for(int j = 0; j < (signed int) ubig_value.size(); j++){
      if((j + 1) < (((signed int) ubig_value.size()))){
         if((((int(ubig_value[j+1]))-48) %2) == 0){
           ubig_value[j] = ((int(ubig_value[j])-48) / 2) +48;
         }else{
            ubig_value[j] = ((((int(ubig_value[j])-48)) / 2)+5) +48;
         }
      }else{
         ubig_value[j] = ((int(ubig_value[j])-48) / 2) +48;
      }
   }
   while((ubig_value.size()>0)
          and (ubig_value.back()=='0')){
      ubig_value.pop_back();
   }
}

ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   //static const ubigint zero = 0;
   //static const ubigint zero;
   ubigint zero1;
   zero1.ubig_value.push_back('0');
   //zero.ubig_value.push_back('0');
   if (that.ubig_value.size() == 0)
       throw domain_error ("ubigint::divide: by 0");
   //ubigint power_of_2 = 1;
   ubigint power_of_2;
   power_of_2.ubig_value.push_back('1');
   ubigint divisor = that; // right operand, divisor
   //ubigint quotient = 0;
   ubigint quotient;
   quotient.ubig_value.push_back('0');
   ubigint remainder = *this; // left operand, dividend

   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero1) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return divide (that).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

//new == operator
bool ubigint::operator== (const ubigint& that) const{
   bool isSame = true;
   int len = ubig_value.size();
   if(ubig_value.size() == that.ubig_value.size()){
      for(int j = 0; j < len; j++){
         int d = int(ubig_value[j])-48;
         int e = int(that.ubig_value[j])-48;
         if(d != e){
            isSame = false;
            break;
         }
      }
   }else{
      isSame = false;
   }
   return isSame;
}

//new < operator
bool ubigint::operator< (const ubigint& that) const {
   bool isLess = false;
   int len = max(ubig_value.size(), that.ubig_value.size());
   if(ubig_value.size() == that.ubig_value.size()){
      for(int j = 0; j < len; j++){
         int d = int(ubig_value[j])-48;
         int e = int(that.ubig_value[j])-48;
         if(d < e){
            isLess = true;
         }else if(d > e){
            isLess = false;
         }
      }
   }else if(ubig_value.size() < that.ubig_value.size()){
      isLess = true;
   }else if(that.ubig_value.size() < ubig_value.size()){
      isLess = false;
   }
   return isLess;
}

//new ostream
ostream& operator<< (ostream& out, const ubigint& that){
   string num;
   int size = (signed int)that.ubig_value.size();
   for(int j = 0; j < size; j++){
      num = string(1,that.ubig_value[j]) + num;
   }
   return out << num;
}
