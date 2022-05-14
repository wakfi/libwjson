#ifndef LEXER_CPP
#define LEXER_CPP


#include <istream>
#include <string>
#include "lexer.h"

bool Lexer::isValidIdentifier(int c)
{
	return c == '_' || isalnum(c);
}

bool Lexer::skipNextChar(const char& c)
{
	return isspace(c);
}

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
			if(isdigit(nextChar) || nextChar == '-')
			{
				if(nextChar == '-')
				{
					nextChar = readNextChar();
					lexeme += nextChar;
					if(!isdigit(nextChar)) {
						error("Invalid token,", startLine, startColumn);
					}
				}
				bool leadingZero = nextChar == '0';
				if(leadingZero && peek() == '0') {
					error("Invalid token: leading 0's are not allowed,", startLine, startColumn);
				}
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
				if(peek() == 'e' || peek() == 'E')
				{
					nextChar = readNextChar();
					lexeme += nextChar;
					nextChar = readNextChar();
					if(nextChar == '-' || nextChar == '+')
					{
						lexeme += nextChar;
						nextChar = readNextChar();
					}
					if(!isdigit(nextChar))
					{
						error("Invalid token: '" + lexeme + "':", startLine, startColumn);
					}
					lexeme += nextChar;
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

#endif // ifndef LEXER_CPP
