#ifndef LEXER_H
#define LEXER_H


#include <istream>
#include <string>
#include "token.h"
#include "json_exception.h"

class Lexer
{
	public:
		static bool isValidIdentifier(int);

		static bool skipNextChar(const char&);

		// construct a new lexer from the input stream
		Lexer(std::istream&);

		// return the next available token in the input stream (including
		// EOS if at the end of the stream)
		Token next_token();

	private:

		// input stream, current line, and current column
		std::istream& input_stream;
		int line;
		int column;

		// return a single character from the input stream and advance
		char read();

		// read the next n characters from the input stream and store them in buf.
		// returns actual number of characters read
		int read(char* buf, const int& n);

		// read up to the next 4 characters as bytes and return as a 32-bit unsigned integer
		unsigned int read_u32(const int&);

		// return a single character from the input stream without advancing
		char peek();

		// create and throw a mypl_exception (exits the lexer)
		void error(const std::string&, int, int) const;

		// reads the next character using Lexer::read() and increments column
		// (new lines get handled without this function being called)
		char readNextChar();
};


#endif // ifndef LEXER_H
