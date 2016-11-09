// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

string trim(string word){
   string a = "";
   size_t j = word.find_first_not_of(' ');
   size_t k = word.find_last_not_of(' ');
   if((signed int)j != -1){ //and (signed int)k != -1){
      if(j != 0){
      word.erase(0,j-1);
      }
      }
      if((signed int)k != -1){
      if(k != word.size()){
      word.erase(k+1,(signed int) word.size()-1);
      }
      }
   
   /*for(int i = 0; i < (signed int) word.size(); i++){
      if(word[i] != ' '){
        a += word[i];
      }
   }*/
   return word;
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   str_str_map newMap;
   int count = 1;
   string input = "";
   if(argc == 1){
      for(;;){
         if(getline(cin,input)){
            string beforeTrim = input;
            string curr = trim(input);
           // size_t bt = beforeTrim.find_first_of('=');
            size_t foundEq = curr.find_first_of('=');
            string key1 = curr.substr(0,foundEq);
            string value1 = curr.substr(foundEq+1, curr.length()-1);
            string key = trim(key1);
            string value = trim(value1);
            if((signed int)curr.find("=") != -1){
               if(key != "" and value != ""){ // key = value
                  str_str_pair newPair(key,value);
                  str_str_map::iterator i = newMap.insert(newPair);
                  cout<< "-: " << count<< ": "<<input <<endl;
                  //cout<<beforeTrim.substr(0,bt)<<" = "<<
                  // beforeTrim.substr(bt+1,beforeTrim.length()-1)
                   //<<endl;
                  cout<<(i->first)<<" =" <<(i->second)<<endl;
                  count++;
               }else if(key == "" and value != ""){ // = value
                  cout<< "-: " << count<< ": "<< input <<endl;
                  count++;
                  str_str_map::iterator i = newMap.begin();
                  for(;i != newMap.end();++i){
                     if((i -> second) == value){
                        cout<<(i -> first)<<" ="<<(i->second)<<endl;
                     }
                  }
               }else if(key != "" and value == ""){ // key =
                  cout<< "-: " << count<< ": "<< input <<endl;
                  count++;
                  newMap.erase(newMap.find(key));
               }else if(key == "" and value == ""){ // =
                  cout<< "-: " << count<< ": "<< input <<endl;
                  count++;
                  str_str_map::iterator i = newMap.begin();
                  for(;i != newMap.end();++i){
                     cout<<(i -> first)<<" ="<<(i -> second)<<endl;
                  }
               }
            }else{
              cout<< "-: " << count<< ": "<< input <<endl;
               str_str_map::iterator i = newMap.begin();
              bool found = false;
              if((signed int)beforeTrim.find('#') != -1 
               or beforeTrim.size() == 0){
              }else{
                 if(i != newMap.end()){
                    for(;i != newMap.end();++i){
                       if(key.compare(i -> first) == 0){
                          cout<<(i -> first)<<" = "<<
                          (i -> second)<<endl;
                          found = true;
                       }
                    }
                  }
                  if(found == false){
                      cerr<<key<< ": key not found"<<endl;
                  }
              }
              count++;
            }
         }else{break;}
      }
   }else{
      for(int i = 1; i < argc; ++i){
         count = 1;
         ifstream myfile(argv[i]);
         string name = argv[i];
         while(getline(myfile,input)){
         if(myfile.eof()){
            break;
         }else{
         string beforeTrim = input;
         string curr = trim(input);
         //size_t bt = beforeTrim.find_first_of('=');
         size_t foundEq = curr.find_first_of('=');
         string key1 = curr.substr(0,foundEq);
            string value1 = curr.substr(foundEq+1, curr.length()-1);
            string key = trim(key1);
            string value = trim(value1);
         //string key = curr.substr(0,foundEq);
         //string value = curr.substr(foundEq+1, curr.length()-1);
         if((signed int)curr.find("=") != -1){
            if(key != "" and value != ""){ // key = value
               str_str_pair newPair (key,value);
              str_str_map::iterator i = newMap.insert(newPair);
               cout<< name <<": " << count<< ": "<< input <<endl;
               //cout<<beforeTrim.substr(0,bt)<<" = "<<
               //beforeTrim.substr(bt+1,beforeTrim.length()-1)<<endl;
               cout<<(i->first)<< " = " <<(i->second)<<endl;
               count++;
            }else if(key == "" and value != ""){ // = value
               cout<<name <<": " << count<< ": "<< input <<endl;
               count++;
               str_str_map::iterator i = newMap.begin();
               for(;i != newMap.end();++i){
                  if((i -> second) == value){
                     cout<<(i -> first)<< " = " << value <<endl;
                  }
               }
            }else if(key != "" and value == ""){ // key =
               cout<<name<< ": " << count<< ": "<< input <<endl;
               count++;
               newMap.erase(newMap.find(key));
            }else if(key == "" and value == ""){ // =
                  cout<<name <<": " << count<< ": "<< input <<endl;
                  count++;
                  str_str_map::iterator i = newMap.begin();
                  for(;i != newMap.end();++i){
                     cout<<(i -> first)<<" = "<<(i -> second)<<endl;
                  }
            }
         }else{
              cout<<name <<": " << count<< ": "<< input <<endl;
              str_str_map::iterator i = newMap.begin();
              bool found = false;
              if((signed int)beforeTrim.find('#') != -1
                or beforeTrim.size() == 0){    
              }else{
                 if(i != newMap.end()){
                    for(;i != newMap.end();++i){
                       if(key.compare(i -> first) == 0){
                          cout<<(i -> first)<<" = "
                          <<(i -> second)<<endl;
                          found = true;
                       }
                    }
                  }
                  if(found == false){
                      cerr<<key<< ": key not found"<<endl;
                  }
              }
              count++;
         }
         }
         }
         myfile.close();   
      }
   }

   /*str_str_map test;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
        cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);
   */
   // cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
