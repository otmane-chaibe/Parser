/*
 * parsetree.h
 *Otmane Chaibe
 *CS 280
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>

using std::vector;
using std::map;


// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
protected:
    int			linenum;
    ParseTree	*left;
    ParseTree	*right;

public:
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
            : linenum(linenum), left(l), right(r) {}

    virtual ~ParseTree() {
        delete left;
        delete right;
    }

    int GetLineNumber() const { return linenum; }
    virtual NodeType GetType() const { return ERRTYPE; }


    int MaxDepth() const {
         int lc = 0;
        
        if( left ){
            if(left->MaxDepth()>lc)
                lc = left->MaxDepth();
        }
        if( right ){
            if(right->MaxDepth()>lc)
                lc = right->MaxDepth();
        }
        
            lc++;
        return lc;
    }

    virtual bool IsIdent() const { return false; }
    virtual bool IsVar() const { return false; }
    virtual string GetId() const { return ""; }
    virtual int IsBang() const { return 0; }
    virtual bool IsLet() const { return false; }

    int BangCount() const {
        int bang=0;
        if( left ){
           
                bang += left->BangCount();
        }
        if( right ){
           
                bang += right->BangCount();
        }
        
        if(IsBang()){
            bang++;
        }
        return bang;
        
    }
    
     virtual void CheckLetBeforeUse(map<string,bool>& var) {
        if( left ) left->CheckLetBeforeUse(var);
        if( right ) right->CheckLetBeforeUse(var);
       if(IsVar()){
            var[ this->GetId() ]++;
       }
         if(IsIdent()){
             if(var.find(this->GetId())==var.end()){
                 cout<<"UNDECLARED VARIABLEhhh "<<this->GetId()<<endl;
             }
             
         }
         
            
       
    }
};

class StmtList : public ParseTree {

public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
};

class Let : public ParseTree {
    string id;

public:
    Let(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

    bool IsVar() const { return true; }
    bool IsLet() const { return true; }
};

class Print : public ParseTree {
public:
    Print(int& line, ParseTree *expr) : ParseTree(line, expr) {}
};

class Loop : public ParseTree {
public:
    Loop(int& line, ParseTree *expr, ParseTree *slist) : ParseTree(line, expr, slist) {}
};

class If : public ParseTree {
public:
    If(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};


class PlusExpr : public ParseTree {
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class MinusExpr : public ParseTree {
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class TimesExpr : public ParseTree {
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class DivideExpr : public ParseTree {
public:
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

};

class BangExpr : public ParseTree {
public:
   BangExpr(int& line, ParseTree *ex) : ParseTree (line, ex){}
      int IsBang() const { return true;}
    

    
};

class IConst : public ParseTree {
    int val;

public:
    IConst(int l, int i) : ParseTree(l), val(i) {}
    IConst(Lex t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }
    NodeType GetType() const { return INTTYPE; }
};

class SConst : public ParseTree {
    string val;

public:
    SConst(Lex t) : ParseTree(t.GetLinenum()) {
        val = t.GetLexeme();
    }
    NodeType GetType() const { return STRTYPE; }
    string GetStringValue() const { return val; }
};

class Ident : public ParseTree {
    string id;

public:
    Ident(Lex& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

    bool IsIdent() const { return true; }
    bool IsVar() const { return true; }
};

#endif /* PARSETREE_H_ */
