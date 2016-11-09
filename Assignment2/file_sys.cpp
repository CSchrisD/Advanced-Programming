// $Id: file_sys.cpp,v 1.5 2016-01-14 16:16:52-08 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
}

const string& inode_state::prompt() { return prompt_; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

void inode_state::setRoot(){
   string a = ".";
   string b = "..";
   inode_ptr x = make_shared<inode> (inode(file_type::DIRECTORY_TYPE)); 
   inode x1 = *x;
   x1.contents -> setConnect(x,x);
   root = (x);
   cwd = root;
}

void plain_file::setConnect(inode_ptr& parent, inode_ptr& curr){
   cerr << "Error: cant connect" << parent << " " << curr<<endl;
   throw file_error ("is a plain file");
}

void directory::setConnect(inode_ptr& parent, inode_ptr& curr){
   dirents.emplace(".", curr);
   dirents.emplace("..", parent);
}

base_file_ptr inode::getCon(){
   return contents;
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

file_error::file_error (const string& what):
            runtime_error (what) {
}

void inode_state::setPrompt(string& newPrompt){
   prompt_ = newPrompt;
}

inode_ptr inode_state::getROOT(){
      return root;
}

inode_ptr inode_state::getCWD(){
      return cwd;
}

file_type directory::getType(){
   return file_type::DIRECTORY_TYPE;
}

file_type plain_file::getType(){
   return file_type::PLAIN_TYPE;
}

void inode_state::setLS(){
   showPWD();
   inode x = *cwd.get();
   map<string,inode_ptr> m = x.contents -> getMap();
   file_type g;
   for(map<string,inode_ptr>::iterator it =
   m.begin(); it != m.end(); ++it) {
      int q = (*it->second).get_inode_nr();
      if((*it->second).getCon() -> getType()
            == file_type::PLAIN_TYPE){
         g = file_type::PLAIN_TYPE;
      }else{
         g = file_type::DIRECTORY_TYPE;
      }
      if(file_type::PLAIN_TYPE!= g){
         if(it->first != "." and it->first != ".."){
            cout <<"      "<< q << "  "<< ((*it->second).contents
            -> getMap()).size() <<"  /"<< it->first << endl;
         }else{
            cout <<"      "<< q << "  "<< ((*it->second).contents
            -> getMap()).size() <<"  "<< it->first << endl;
         }
      }else{
         int v = (*it->second).contents-> getData().size();
         v = v + (v-1);
         cout<<"      "<<q<<"  "<<v<<"  "<< it->first << endl;
      }
   }
}

void plain_file::changeSize(size_t t){
   //this->size = t;
}

void directory::changeSize(size_t t){
   cout << this->size()<<endl;
   //this->size = t;
}

void inode_state::printALL(inode_state& state, string& name){
   setLS();
   inode x = *cwd.get();
   map<string,inode_ptr> m = x.contents -> getMap();
   vector<string> nextDir;
   vector<string> change;
   for(map<string,inode_ptr>::iterator it =
    m.begin(); it != m.end(); ++it) {
      if((*it->second).getCon() -> getType() 
       == file_type::DIRECTORY_TYPE){
         if(it->first == name){
            change.push_back(name);
            (*state.cwd).changeDir(state, change);
            setLS();
         }
      }
   }
   cwd = make_shared<inode> (x);
}

void inode_state::removeThing(string& name){
   (*cwd).contents -> remove(name);
}

void inode_state::rmEvery(string& name){
   (*cwd).contents -> removeALL(name);
}

map<string,inode_ptr> directory::getMap(){
   return dirents;
}

map<string,inode_ptr> plain_file::getMap(){
   throw file_error ("is a plain file");
}

wordvec directory::getData(){
   throw file_error ("is a directory");
}

wordvec plain_file::getData(){
   return data;
}

void inode_state::showPWD(){
   cout << "/";
   map<string,inode_ptr> m = (*cwd).contents -> getMap();
   inode_ptr x;
   for(map<string,inode_ptr>::iterator it =
    m.begin(); it != m.end(); ++it) {
      if(it->first == ".."){
           x = it->second;
           break;
      }
   }
   map<string,inode_ptr> m2 = (*x).contents -> getMap();
   for(map<string,inode_ptr>::iterator it =
       m2.begin(); it != m2.end(); ++it) {
      if(it->second == cwd){
           if(it->first != "." and it->first != ".."){
              cout<< it->first;
              break;
           }
      }
   }
   cout<< "" << endl;
}

void inode::changeDir(inode_state& state, const wordvec& dirname){
   map<string,inode_ptr> m = contents -> getMap();
   bool found = false;
   string finalDir;
   vector<string> g = dirname;
   g.erase(g.begin());
   if(dirname.size() != 0){
   for(map<string,inode_ptr>::iterator it =
      m.begin(); it != m.end(); ++it) {
     if(it->first == dirname[0]){
        finalDir = dirname[0];
        if(g.size() == 0){
           found = true;
        }else{
           (*it->second).changeDir(state, g);
        }
      }
   }
   }
   if(found){
      state.cwd = (m.find(finalDir)->second);
   }
}

void inode_state::written(string& fileNm, const wordvec& words){
   inode_ptr x = (*cwd).contents -> mkfile(fileNm);
   map<string,inode_ptr> m = (*cwd).contents -> getMap();
   inode_ptr g;
   if( x == nullptr){
      for(map<string,inode_ptr>::iterator it =
         m.begin(); it != m.end(); ++it) {
        if(it->first == fileNm){
          vector<string> sentence = words;
          sentence.erase(sentence.begin());
          sentence.erase(sentence.begin());
          (*it->second).contents -> writefile(sentence);
        }
      }
   }else{
      vector<string> sentence = words;
      sentence.erase(sentence.begin());
      sentence.erase(sentence.begin());
      (*x).contents -> writefile(sentence);
   }
}

void inode_state::read(string& fileNm){
   map<string,inode_ptr> m = (*cwd).contents -> getMap();
   bool found = false;
   inode_ptr x;
   for(map<string,inode_ptr>::iterator it =
        m.begin(); it != m.end(); ++it) {
     if(it->first == fileNm){
        found = true;
        x = it->second;
     }
   }
   if(found){
      vector<string> sentence = (*x).contents -> readfile();
      for(int i = 0; i < (signed int) sentence.size(); i++){
         cout << sentence.at(i) << " ";
      }
   }else{
      cerr << "cat: " << fileNm <<" no such file or directory";
   }
   cout << "" << endl;
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data = words;
}

void plain_file::remove (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkdir (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkfile (const string&) {
   throw file_error ("is a plain file");
}

size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& directory::readfile() const {
   throw file_error ("is a directory");
}

void directory::writefile (const wordvec&) {
   throw file_error ("is a directory");
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   for(map<string,inode_ptr>::iterator it =
        dirents.begin(); it != dirents.end(); ++it) {
      if(filename.compare(it->first) == 0){
         if((*it->second).getCon() -> getType()
              == file_type::DIRECTORY_TYPE){ 
            map<string,inode_ptr> m =
             (*it->second).getCon() -> getMap();
            if((signed int) m.size() == 2){
               (*it->second).getCon() = nullptr;
               dirents.erase(it);
               break;
            }else{
               cerr<<
          "Error: Direcotry is not empty or invalid name."<<endl;
               break;
            }
         }else{
            (*it->second).getCon() = nullptr;
            dirents.erase(it);
            break;
         }
      }
   }
}

void plain_file::removeALL(const string& filename){
   throw file_error ("is a plain file");
   cerr<< filename << endl;   
}

void directory::removeALL(const string& filename){
   for(map<string,inode_ptr>::iterator it =
        dirents.begin(); it != dirents.end(); ++it) {
      if(filename.compare(it->first) == 0){
         if((*it->second).getCon() -> getType()
              == file_type::DIRECTORY_TYPE){
            map<string,inode_ptr> m =
             (*it->second).getCon() -> getMap();
            if(it->first != "." and it->first != ".."){
               (*it->second).getCon() = nullptr;
               dirents.erase(it);
               break;
            }
         }else{
            (*it->second).getCon() = nullptr;
            dirents.erase(it);
            break;
         }
      }
   }
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   inode_ptr curr = make_shared<inode>
        (inode(file_type::DIRECTORY_TYPE));
   if(dirents.size() > 2){
      for(map<string,inode_ptr>::iterator it =
            dirents.begin(); it != dirents.end(); ++it){
         if(it->first != "." and it->first != ".."){
            if(dirname.compare(it->first) < 0){
               dirents.insert(it,pair<string,inode_ptr>
                (dirname, curr));
               break;
            }else if(dirname.compare(it->first) == 0){
               cerr << "Error: "<<dirname <<  " exists" <<endl;
               break;
            }else if(dirname.compare(it->first) > 0){
               dirents.insert(it,pair<string,inode_ptr>
                (dirname, curr));
               break;
            }
         }
      }
   }else{
      dirents.emplace(dirname,curr);
   }
   inode x = *curr;
   base_file_ptr g = x.getCon();
   g -> setConnect(dirents.find(".")->second, curr);
   return nullptr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr curr = make_shared<inode> (inode(file_type::PLAIN_TYPE));
   if((signed int) dirents.size() > 2){
      for(map<string,inode_ptr>::iterator it =
           dirents.begin(); it != dirents.end(); ++it) {
         if(it->first != "." and it->first != "."){
            if(filename.compare(it->first) < 0){
               dirents.insert(it,pair<string,inode_ptr>
               (filename, curr));
               break;
            }else if(filename.compare(it->first) == 0){
               return nullptr;
            }else if(filename.compare(it->first) > 0){
               dirents.insert(it,pair<string,inode_ptr>
                (filename, curr));
               break;
            }
         }
      }
   }else{
      dirents.emplace(filename, curr);
   }
   return curr;
}
