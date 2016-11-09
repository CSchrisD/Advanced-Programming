// $Id: commands.cpp,v 1.16 2016-01-14 16:10:40-08 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
      const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int x = (signed int) words.size();
   if(x == 2){
      string a = words[1];
      state.read(a);
   }else{
      cerr << "Error: invalid number of arguments." << endl;
   }
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int t = (signed int) words.size();
   vector<string> names;
   bool none = true;
   if(t == 2){
      string a = words[1];
      string b = "";
      for(int i = 0; i < (signed int) a.length(); i++){
          if(a[i] == '/'){
             names.push_back(b);
             b = "";
             none = false;
          }else{
             b += a[i];
          }
      }
      if(none){
         if(a == "/"){
           (*state.getCWD()) = (*state.getROOT()); 
         }else{
           names.push_back(a);
           (*state.getCWD()).changeDir(state, names);
         }
      }else{
         (*state.getCWD()).changeDir(state, names);
      }
   }else{
      cerr << "Error: invalid number of arguments." << endl;
   }
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   state.setLS();
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if((signed int) words.size() == 2){
      string a = words[1];
      state.printALL(state, a);
   }else{
      cerr<<"Error: invalid number of arguments."<<endl;
   }
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if((signed int) words.size() > 1){
      string a = words[1];
      state.written(a, words);
   }else{
      cerr << "Error: insufficent amount of information." <<endl;
   }
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int x = (signed int) words.size();
   if (x == 1 or  x > 2 ){
      cerr << "Error: invalid number of mkdir arguments." << endl;
   }else{
      inode x = *state.getCWD();
      string a = words[1];
      x.getCon() -> mkdir(a);
      size_t g = 1;
      x.getCon() -> changeSize(g);
   }
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   string a = "";
   //todo: join words together with spaces
   for(int i = 1; i < (signed int) words.size(); i++){
      a = a + words[i] + " ";
   }
   state.setPrompt(a);
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if((signed int) words.size() == 1){
      state.showPWD();
   }else{
      cerr << "Error: Too many arguments." << endl;
   }
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int x = (signed int) words.size();
   if(x == 2){
      string a = words[1];
      state.removeThing(a);
   }else{
      cerr << "Error: invalid number of arguments." << endl;
   }
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int x = (signed int) words.size();
   if(x == 2){
      string a = words[1];
      state.rmEvery(a);
   }else{
      cerr << "Error: invalid number of arguments." << endl;
   }
}
