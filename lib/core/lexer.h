#ifndef LEXER_H
#define LEXER_H


#include <istream>
#include <string>
#include "token.h"
#include "json_exception.h"


class Lexer
{
	public:

		// construct a new lexer from the input stream
		Lexer(std::istream& input_stream);

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

		// read up to the next 4 characters as bytes and return as a 32-bit integer
		unsigned int read_u32(const int& n);

		// return a single character from the input stream without advancing
		char peek();

		// create and throw a mypl_exception (exits the lexer)
		void error(const std::string& msg, int line, int column) const;

		// reads the next character using Lexer::read() and increments column
		// (new lines get handled without this function being called)
		char readNextChar();
};


Lexer::Lexer(std::istream& input_stream)
	: input_stream(input_stream), line(1), column(1)
{
}


char Lexer::read()
{
	return input_stream.get();
}

int Lexer::read(char* buf, const int& n)
{
	input_stream.read(buf, n);
	return input_stream.gcount();
}

unsigned int Lexer::read_u32(const int& n)
{
	unsigned int val = 0;
	int len = n > 4 ? 4 : n;
	switch(len) {
		case 4:
			val |= read() << 24;
		case 3:
			val |= read() << 16;
		case 2:
			val |= read() << 8;
		case 1:
			val |= read();
	}
	column += n;
	return val;
}

char Lexer::peek()
{
	return input_stream.peek();
}


void Lexer::error(const std::string& msg, int line, int column) const
{
	throw JSONException(LEXER, msg, line, column);
}

char Lexer::readNextChar()
{
	column += 1;
	return read();
}

bool isValidIdentifier(int c)
{
	return c == '_' || isalnum(c);
}

bool skipNextChar(const char& c)
{
	return isspace(c);
}

Token Lexer::next_token()
{
	Token token;
	char nextChar;
	std::string lexeme = "";
	while(skipNextChar(peek()))
	{
		nextChar = read();
		if(nextChar == '\n')
		{
			line += 1;
			column = 1;
		} else {
			column += 1;
		}
	}
	int startColumn = column;
	int startLine = line;
	nextChar = readNextChar();
	lexeme += nextChar;
	switch(nextChar) {
		case EOF:
			token = Token(EOS, "", startLine, startColumn);
			break;
		case '{':
			token = Token(LBRACE, "{", startLine, startColumn);
			break;
		case '}':
			token = Token(RBRACE, "}", startLine, startColumn);
			break;
		case '[':
			token = Token(LBRACKET, "[", startLine, startColumn);
			break;
		case ']':
			token = Token(RBRACKET, "]", startLine, startColumn);
			break;
		case ':':
			token = Token(COLON, ":", startLine, startColumn);
			break;
		case ',':
			token = Token(COMMA, ",", startLine, startColumn);
			break;
		case '"':
			// string
			lexeme = "";
			while(peek() != '"')
			{
				nextChar = readNextChar();
				if(nextChar == '\\') //handle escapes (primarily for quote escape)
				{
					lexeme += nextChar;
					nextChar = readNextChar();
				}
				if(nextChar == EOF || nextChar == '\n')
				{
					error("Invalid token '\"" + lexeme + "': string values require an opening and closing quotation mark,", startLine, startColumn);
				}
				lexeme += nextChar;
			}
			nextChar = readNextChar();
			token = Token(STRING_VAL, lexeme, startLine, startColumn);
			break;
		default:
			if(isdigit(nextChar)) {
				while(isdigit(peek()))
				{
					nextChar = readNextChar();
					lexeme += nextChar;
				}
				if(peek() == '.')
				{
					// double literal
					nextChar = readNextChar();
					lexeme += nextChar;
					// need to finish reading numbers in
					if(!isdigit(peek()))
					{
						// no numbers after dot error
						error("Invalid token: '" + lexeme + "': double values must have at least one trailing digit,", startLine, startColumn);
					}
					while(isdigit(peek()))
					{
						nextChar = readNextChar();
						lexeme += nextChar;
					}
				}
				token = Token(NUMBER_VAL, lexeme, startLine, startColumn);
			} else if(nextChar == 't') {
				const unsigned int MAGIC_NUMBER = 0x727565;
				unsigned int buf = read_u32(3);
				if(~(buf ^~ MAGIC_NUMBER)) {
					error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
				}
				token = Token(LITERAL_VAL, "true", startLine, startColumn);
			} else if(nextChar == 'f') {
				const unsigned int MAGIC_NUMBER = 0x616c7365;
				unsigned int buf = read_u32(4);
				if(~(buf ^~ MAGIC_NUMBER)) {
					error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
				}
				token = Token(LITERAL_VAL, "false", startLine, startColumn);
			} else if(nextChar == 'n') {
				const unsigned int MAGIC_NUMBER = 0x756c6c;
				unsigned int buf = read_u32(3);
				if(~(buf ^~ MAGIC_NUMBER)) {
					error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
				}
				token = Token(LITERAL_VAL, "null", startLine, startColumn);
			} else {
				error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
			}
	}
	return token;
}

#endif
