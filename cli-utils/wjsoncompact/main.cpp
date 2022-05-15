#include <iostream>
#include <fstream>

// libwjson modules
#include <core/token.cpp>
#include <core/json_exception.cpp>
#include <core/lexer.cpp>
#include <core/parser.cpp>
#include <core/ast.cpp>

#include "wjsoncompact.cpp"

using namespace std;


int main(int argc, char* argv[])
{
  // use standard input if no input file given
  istream* input_stream = &cin;
  if (argc == 2)
    input_stream = new ifstream(argv[1]);

  // create the lexer & parser
  Lexer lexer(*input_stream);
  Parser parser(lexer);

  // read each token in the file until EOS or error
  try {
    JSONDocument ast_root_node;
    parser.parse(ast_root_node);
    Printer printer(cout);
    ast_root_node.accept(printer);
  } catch (JSONException e) {
    cerr << e.to_string() << endl;
    exit(1);
  }
  // clean up the input stream
  if (input_stream != &cin)
    delete input_stream;
}
