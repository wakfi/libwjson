#ifndef JSON_EXCEPTION_CPP
#define JSON_EXCEPTION_CPP

#include <string>
#include "json_exception.h"

JSONException::JSONException(ExceptionType t, const std::string& m, int l, int c)
  : type(t), message(m), line(l), column(c), has_line_column(true)
{
}


JSONException::JSONException(ExceptionType t, const std::string& m)
  : type(t), message(m), has_line_column(false)
{
}


std::string JSONException::to_string() const
{
  std::string s;
  switch(type) {
    case LEXER: s = "Lexer"; break;
    case SYNTAX: s = "Parser"; break;
    case SEMANTIC: s = "Type"; break;
  }
  s += " Error: " + message;
  if (has_line_column)
    s += " at line " + std::to_string(line) +
      " column " + std::to_string(column);
  return s;
}


#endif // ifndef JSON_EXCEPTION_CPP
