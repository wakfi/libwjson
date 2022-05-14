// JSON parsing Token header

#ifndef TOKEN_CPP
#define TOKEN_CPP

#include "token.h"

// default constructor
Token::Token()
	: token_type(EOS), token_lexeme(""), token_line(0), token_column(0)
{
}

// constructor
Token::Token(TokenType type, const std::string& lexeme, int line, int column)
	: token_type(type), token_lexeme(lexeme), token_line(line),
	  token_column(column)
{
}

// return the type of the token
TokenType Token::type() const
{
	return token_type;
}

// return the token string value
std::string Token::lexeme() const
{
	return token_lexeme;
}

// return the line location of lexeme
int Token::line() const
{
	return token_line;
}

// return the column location where the lexeme starts
int Token::column() const
{
	return token_column;
}

// a string representation of the token object
std::string Token::to_string() const
{
	return token_type_map.find(token_type)->second +
		" '" + lexeme() + "' " +
		std::to_string(line()) + ":" + std::to_string(column());
}


#endif // ifndef TOKEN_CPP
