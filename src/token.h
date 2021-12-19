// JSON parsing Token header

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>


// JSON allowable token types
enum TokenType {
	// basic symbols
	COMMA, COLON, LBRACKET, RBRACKET, LBRACE, RBRACE,
	// primitive types
	BOOL_TYPE, NUMBER_TYPE, STRING_TYPE,
	// values
	BOOL_VAL, NUMBER_VAL, STRING_VAL,
	// end-of-stream
	EOS
};


class Token
{
public:

	// default constructor
	Token();

	// constructor
	Token(TokenType type, const std::string& lexeme, int line, int column);

	// return the type of the token
	TokenType type() const;

	// return the token string value
	std::string lexeme() const;

	// return the line location of lexeme
	int line() const;

	// return the column location where the lexeme starts
	int column() const;

	// a string representation of the token object
	std::string to_string() const;

private:

	// the type of the token
	TokenType token_type;

	// the token's value in the program
	std::string token_lexeme;

	// the line location of the lexeme (starts at 1)
	int token_line;

	// the column location of the start of the lexeme (starts at 1)
	int token_column;

	// token type to string representation (for printing)
	std::map<TokenType, std::string> token_type_map =
    { // basic symbols
        {COMMA, "COMMA"}, {COLON, "COLON"}, {LBRACKET, "LBRACKET"},
		{RBRACKET, "RBRACKET"}, {LBRACE, "LBRACE"}, {RBRACE, "RBRACE"},
		// primitive types
		{BOOL_TYPE, "BOOL_TYPE"}, {NUMBER_TYPE, "NUMBER_TYPE"},
		{STRING_TYPE, "STRING_TYPE"},
		// values
		{BOOL_VAL, "BOOL_VAL"}, {NUMBER_VAL, "NUMBER_VAL"},
		{STRING_VAL, "STRING_VAL"},
		// eos
		{EOS, "EOS"}
	};
};

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


#endif
