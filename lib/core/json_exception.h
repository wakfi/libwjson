#ifndef JSON_EXCEPTION
#define JSON_EXCEPTION

#include <string>

// the compilation stage where the error occurred
enum ExceptionType {LEXER, SYNTAX, SEMANTIC};


// specialized exception for json implementation
class JSONException : public std::exception
{
 public:

  // construct a "normal" error exception
  JSONException(ExceptionType type, const std::string& msg, int line, int column);

  // construct an error exception without a line and column
  JSONException(ExceptionType type, const std::string& msg);

  // return a string representation for printing
  std::string to_string() const;

 private:

  ExceptionType type;
  std::string message;
  bool has_line_column;
  int line;
  int column;

};


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


#endif
