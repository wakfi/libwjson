// JSON parsing Token header

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>


// JSON allowable token types
enum TokenType {
	// basic symbols
	COMMA, COLON, LBRACKET, RBRACKET, LBRACE, RBRACE,
	// values
	LITERAL_VAL, NUMBER_VAL, STRING_VAL,
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
		// values
		{LITERAL_VAL, "LITERAL_VAL"}, {NUMBER_VAL, "NUMBER_VAL"},
		{STRING_VAL, "STRING_VAL"},
		// eos
		{EOS, "EOS"}
	};
};


#endif // ifndef TOKEN_H
