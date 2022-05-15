#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "json_exception.h"
#include "ast.h"
#include "lexer.h"


class Parser
{
public:
	// create a new recursive descent parser
	Parser(const Lexer&);

	// run the parser
	void parse(JSONDocument&);

private:
	Lexer lexer;
	Token curr_token;

	// helper functions
	void advance();
	void eat(TokenType t, std::string);
	void error(std::string);
	void base_error(std::string);

	// recursive descent functions
    void json(JSON&);
    void records(std::list<Record*>&);
    void record(Record&);
    void array(Array&);
	void values(std::list<RValue*>&);
    void simple(SimpleRValue&);
    void rvalue(RValue*&);

	// Type checks
    void kval();
    void pval();
};


#endif // ifndef PARSER_H
