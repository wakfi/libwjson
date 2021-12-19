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

bool skipNextChar(const char& c, bool& inComment)
{
	return isspace(c);
}

Token Lexer::next_token()
{
    Token token;
	char nextChar;
	std::string lexeme = "";
	while(skipNextChar(peek(), inComment))
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
                if(!(readNextChar() == 'r' && readNextChar() == 'u' && readNextChar() == 'e')) {
                    error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
                }
                token = Token(BOOL_VAL, "true", startLine, startColumn);
            } else if(nextChar == 'f') {
                if(!(readNextChar() == 'a' && readNextChar() == 'l' && readNextChar() == 's' && readNextChar() == 'e')) {
                    error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
                }
                token = Token(BOOL_VAL, "false", startLine, startColumn);
            } else {
                error("Invalid token '\"" + lexeme + "'", startLine, startColumn);
            }
    }
    return token;
}
