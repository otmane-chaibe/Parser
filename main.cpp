#include <iostream>
#include <fstream>
#include <string>
#include "lex.h"
#include "parse.h"
#include <sstream>
#include <list>

using namespace std;

int main(int argc,  char * argv[]) {
    int lineNumber  =0;

    istream *in = &cin;

    ifstream inFile;

    for(int i =1; i<argc; i++){

        string arg(argv[i]);




        if (i == argc-1){



            inFile.open(arg);

            if( inFile.is_open() == false ) {
                cout << "COULD NOT OPEN "<< arg << endl;
                return -1;
            }
            if(inFile.peek()==EOF){
                cout<<"Lines: "<< lineNumber<<endl;

            }
            in = &inFile;


        }

        else{

            cout << "TOO MANY FILENAMES" <<endl;
            return -1;
        }



    }
    
    
    ParseTree *prog = Prog(*in, lineNumber);
    

    if( prog == 0 ){
        return 0;
    }
    
      
        map<string,bool> ids;
        prog->CheckLetBeforeUse(ids);
 
        
        for( const auto& s : ids ){
            if( s.second==0){
                
                cout<< "UNDECLARED VARIABLE "<< s.first<<endl;
            }
        }
        
        
    cout << "BANG COUNT: "<<prog->BangCount()<<endl;
    cout<< "MAX DEPTH: "<<prog->MaxDepth()<<endl;
    
  
    
        
    
    
    return 0;
}
