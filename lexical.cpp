/*
 * lexical.cpp
 *Otmane Chaibe
 * CS280
 * Fall 2019
 */


#include "lex.h"
#include <map>

using namespace std;
 

Lex getNextToken(istream& in, int& linenum)
{
    enum LexState
    {
        BEGIN, INID, INSTRING, ININT, INCOMMENT
    };
    LexState lexstate = BEGIN;
    string lexeme;
    char ch;

    while (in.get(ch)){
        if (ch == '\n'){
            
            (linenum)++;
        }
        switch (lexstate){ 
                
            case BEGIN:
                if (isspace(ch)){
                    continue;
                }
                
                else if (isalpha(ch))
                {
                    lexeme.push_back(ch);
                    lexstate = INID;
                }
                else if (ch == '-' or isdigit(ch))
                {
                    lexeme.push_back(ch);
                    lexstate = ININT;
                }
                else if (ch == '"')
                {
                    lexstate = INSTRING;
                }
                else if (ch == '#')
                {
                    lexstate = INSTRING;
                }
                else if (ch == '+')
                {
                    lexeme.push_back(ch);
                    return Lex(PLUS, lexeme, linenum);
                }
                else if (ch == '*')
                {
                    lexeme.push_back(ch);
                    return Lex(STAR, lexeme, linenum);
                }
                else if (ch == '/')
                {
                    if(in.peek()=='/'){
                    lexstate = INCOMMENT;
                        continue;
                    }
                    else{
                    lexeme.push_back(ch);
                    return Lex(SLASH, lexeme, linenum);
                    }
                }
                
               
                else if (ch == '!')
                {
                    lexeme.push_back(ch);
                   
                        if (in.peek() == '\n')
                        {
                            (linenum)++;
                        }
                        return Lex(BANG, lexeme, linenum);
                 
                }
                
              
               
                else if (ch == '(')
                {
                    lexeme.push_back(ch);
                    return Lex(LPAREN, lexeme, linenum);
                }
                else if (ch == ')')
                {
                    lexeme.push_back(ch);
                    return Lex(RPAREN, lexeme, linenum);
                }
                else if (ch == ';')
                {
                    lexeme.push_back(ch);
                    return Lex(SC, lexeme, linenum);
                }
                else
                {
                    if (in.peek() == '\n')
                    {
                        (linenum)++;
                    }
                    lexeme.push_back(ch);
                    return Lex(ERR, lexeme, linenum);
                }

                break;
            case INID:
                if (isalnum(ch))
                {
                    lexeme.push_back(ch);
                }
                else
                {
                    in.putback(ch);
                    if (ch == '\n')
                    {
                        (linenum)--;
                    }
                    if (lexeme == "print" || lexeme == "PRINT")
                    {
                        lexeme.clear();
                        return Lex(PRINT, lexeme, linenum);
                    }
                    if (lexeme == "let" || lexeme == "LET")
                    {
                        lexeme.clear();
                        return Lex(LET, lexeme, linenum);
                    }
                    else if (lexeme == "if" || lexeme == "IF")
                    {
                        lexeme.clear();
                        return Lex(IF, lexeme, linenum);
                    }
                   
                    else
                    {
                        int nextlinenumb = linenum + 1;
                        if(in.peek() == '\n' && nextlinenumb == EOF)
                        {
                            linenum++;
                        }
                        return Lex(ID, lexeme, linenum);

                    }
                }
                break;
           case INSTRING:
                if (ch == '"')
                {
                    return Lex(STR, lexeme, linenum);
                }
                else if (ch == '\\' )
                {
                   
                    
                    if(in.peek()=='n'){
                    lexeme.push_back('\n');
                    in.ignore(1);
                    }
                    else if(in.peek()=='\\'){
                        lexeme.push_back(ch);
                        in.ignore(0);
                       
                    }else if(in.peek()=='\"'){
                        lexeme.push_back('\"');
                        in.ignore(1);
                        
                       
                    }
                    
                }
               
                
                else if (ch == '\n')
                {linenum--;
                    lexeme.push_back(ch);
                    lexeme = '"' + lexeme;
                    return Lex(ERR, lexeme, linenum);
                }
                
                else{lexeme.push_back(ch);}
                break;
           case INCOMMENT:
                if (ch == '\n')
                {
                    lexstate = BEGIN;
                   
                }
                break; 
            case ININT:
                if (!isdigit(ch)){
                    if (lexeme == "-")
                    {
                        in.putback(ch);
                        return Lex(MINUS, lexeme, linenum);
                    }
                    else if (isalpha(ch))
                    {
                        lexeme.push_back(ch);
                        return Lex(ERR, lexeme, linenum);
                    }
                    else //If it is
                    {
                        if (ch == '\n')
                        {
                            (linenum)--;
                        }

                        in.putback(ch);
                        return Lex(INT, lexeme, linenum);
                    }
                }
                else
                {
                    lexeme.push_back(ch);
                }
                break;
            
        }
    }
    return Lex(DONE, lexeme, linenum);
}

map < Token,string > TokenMap2 = {
        {  PRINT, "PRINT"},
        {  IF, "IF"},
        {  LOOP, "LOOP"},
        {  BEGIN, "BEGIN"},
        {  END, "END"},
        {  ID, "ID"},
        {  INT, "INT"},
        {  STR, "STR"},
        {  PLUS, "PLUS"},
        {  MINUS, "MINUS"},
        {  STAR, "STAR"},
        {  SLASH, "SLASH"},
        {  LPAREN, "LPAREN"},
        {  RPAREN, "RPAREN"},
        {  SC, "SC"},
        {  BANG, "BANG"},
        {  ERR, "ERR"},
        {  DONE, "DONE"},
        {LET,"LET"}
        
    
    };


    
ostream& operator <<(ostream& out, const Lex& tok){
    
  
    if(tok.GetToken()== ID || tok.GetToken()==INT || tok.GetToken()==STR || tok.GetToken()== ERR){
      out<<TokenMap2[tok.GetToken()]<< "(" << tok.GetLexeme() <<")";
    }
   else{
      out<<TokenMap2[tok.GetToken()];
    }
    
    
   return out ;
}
 
