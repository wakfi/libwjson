#ifndef AST_H
#define AST_H

#include <list>
#include "token.h"

enum ValueType {
  STRING_TYPE, NUMBER_TYPE,
  LITERAL_TYPE, ARRAY_TYPE, JSON_TYPE
};

//----------------------------------------------------------------------
// Visitor interface
//----------------------------------------------------------------------

// declare to use in Visitor interface
class JSONDocument;
class Record;
class RValue;
class SimpleRValue;
class JSON;
class Array;

class Visitor {
public:
  // top-level
  virtual void visit(JSON& node) = 0;
  virtual void visit(JSONDocument& node) = 0;
  virtual void visit(Record& node) = 0;
  virtual void visit(SimpleRValue& node) = 0;
  virtual void visit(Array& node) = 0;
};


//----------------------------------------------------------------------
// Top-level Abstract AST Nodes
//----------------------------------------------------------------------

// root AST node interface
class ASTNode
{
  public:
    virtual ~ASTNode() {};
    virtual void accept(Visitor& v) = 0;
};


// root rhs value node
class RValue : public ASTNode
{
  public:
    ValueType type;
    virtual Token first_token() = 0;
};


// root declaration node
class Record : public ASTNode
{
  public:
    Token key;
    RValue* value;
    // visitor access
    void accept(Visitor& v) {v.visit(*this);}
};

// Document
class JSONDocument : public ASTNode
{
  public:
    RValue* root;
    void accept(Visitor& v) {v.visit(*this);}
};


//----------------------------------------------------------------------
// RValue nodes
//----------------------------------------------------------------------


class JSON : public RValue
{
  public:
    // JSON is always JSON type
    ValueType type = JSON_TYPE;
    // Token of the closing right brace for this object
    Token rbrace_token;
    //  list of declarations
    std::list<Record*> records;
    // cleanup memory
    ~JSON() {for (Record* d : records) delete d;}
    // return first token (first primitive value)
    Token first_token() {return this->records.size() ? this->records.front()->key : this->rbrace_token;}
    // visitor access
    void accept(Visitor& v) {v.visit(*this);}
};


class Array : public RValue
{
  public:
    // Array is always Array type
    ValueType type = ARRAY_TYPE;
    // Token of the closing right bracket for this array
    Token rbracket_token;
    // list of elements
    std::list<RValue*> values;
    // cleanup memory
    ~Array() {for (RValue* v : values) delete v;}
    // return first token (first primitive value)
    Token first_token() {return this->values.size() ? this->values.front()->first_token() : this->rbracket_token;}
    // visitor access
    void accept(Visitor& v) {v.visit(*this);}
};


class SimpleRValue : public RValue
{
  public:
    // primitive value
    Token value;
    // return first token
    Token first_token() {return value;}
    // visitor access
    void accept(Visitor& v) {v.visit(*this);}
};


#endif
