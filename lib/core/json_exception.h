#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <string>

// the compilation stage where the error occurred
enum ExceptionType {LEXER, SYNTAX, SEMANTIC};


// specialized exception for wjson implementation
class JSONException : public std::exception
{
 public:

  // construct a "normal" error exception
  JSONException(ExceptionType, const std::string&, int, int);

  // construct an error exception without a line and column
  JSONException(ExceptionType, const std::string&);

  // return a string representation for printing
  std::string to_string() const;

 private:

  ExceptionType type;
  std::string message;
  bool has_line_column;
  int line;
  int column;

};


#endif // ifndef JSON_EXCEPTION_H
