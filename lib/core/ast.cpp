#ifndef AST_CPP
#define AST_CPP

#include "ast.h"


//----------------------------------------------------------------------
// Top-level Abstract AST Nodes
//----------------------------------------------------------------------

// Record
void Record::accept(Visitor& v) {
  v.visit(*this);
}


// JSONDocument
void JSONDocument::accept(Visitor& v) {
  v.visit(*this);
}


//----------------------------------------------------------------------
// RValue nodes
//----------------------------------------------------------------------

// JSON
// cleanup memory
JSON::~JSON() {
  for (Record* d : records) delete d;
}

// return first token (first primitive value)
Token JSON::first_token() {
  return this->records.size()
    ? this->records.front()->key
    : this->rbrace_token;
}

// visitor access
void JSON::accept(Visitor& v) {
  v.visit(*this);
}


// Array
// cleanup memory
Array::~Array() {
  for (RValue* v : values) delete v;
}

// return first token (first primitive value)
Token Array::first_token() {
  return this->values.size()
    ? this->values.front()->first_token()
    : this->rbracket_token;
}

// visitor access
void Array::accept(Visitor& v) {
  v.visit(*this);
}


// SimpleRValue
// return first token
Token SimpleRValue::first_token() {
  return value;
}

// visitor access
void SimpleRValue::accept(Visitor& v) {
  v.visit(*this);
}


#endif // ifndef AST_CPP
