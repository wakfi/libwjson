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
	Parser(const Lexer& json_lexer);

	// run the parser
	void parse(JSON& document);

private:
	Lexer lexer;
	Token curr_token;

	// helper functions
	void advance();
	void eat(TokenType t, std::string err_msg);
	void error(std::string err_msg);
	void base_error(std::string err_msg);

	// recursive descent functions
    void json(JSON& node);
    void records(std::list<Record*>& records);
    void record(Record& node);
    void array(Array& node);
	void values(std::list<RValue*>& vals);
    void simple(SimpleRValue& node);
    void rvalue(RValue*& rval);

	// Type checks
    void kval();
    void pval();
};


// constructor
Parser::Parser(const Lexer& json_lexer) : lexer(json_lexer)
{
}


// Helper functions

void Parser::advance()
{
	curr_token = lexer.next_token();
}


void Parser::eat(TokenType t, std::string err_msg)
{
	if (curr_token.type() == t)
		advance();
	else
		error(err_msg);
}


void Parser::error(std::string err_msg)
{
	std::string s = err_msg + "found '" + curr_token.lexeme() + "'";
	this->base_error(s);
}


// same as error(), but the error message is entirely parameter supplied
void Parser::base_error(std::string err_msg)
{
	int line = curr_token.line();
	int col = curr_token.column();
	throw JSONException(SYNTAX, err_msg, line, col);
}


// Recursive-decent functions

void Parser::parse(JSON& document)
{
	advance();
	this->json(document);
	eat(EOS, "Unexpected token: expected end-of-file, ");
}


// Values

void Parser::json(JSON& node)
{
	eat(LBRACE, "Unexpected token: expected '{', ");
	records(node.records);
	eat(RBRACE, "Unexpected token: expected ',', ");
}

void Parser::records(std::list<Record*>& records)
{
	if(curr_token.type() == RBRACE) return;

	while(1)
	{
		Record* r = new Record;
		record(*r);
		records.push_back(r);
		if(curr_token.type() != COMMA) {
			break;
		}
		advance();
	}
}

void Parser::record(Record& node)
{
	node.key = curr_token;
	kval();
	eat(COLON, "Unexpected token: expected ':', ");
	rvalue(node.value);
}

void Parser::array(Array& node)
{
	eat(LBRACKET, "Unexpected token: expected '[', "); // Should never throw
	values(node.values);
	eat(RBRACKET, "Unexpected token: expected ']', ");
}

void Parser::values(std::list<RValue*>& vals)
{
	if(curr_token.type() == RBRACKET) return;

	while(1)
	{
		RValue* r;
		rvalue(r);
		vals.push_back(r);
		if(curr_token.type() != COMMA) {
			break;
		}
		advance();
	}
}

void Parser::simple(SimpleRValue& node)
{
	node.value = curr_token;
	pval();
	switch(node.value.type())
	{
		case BOOL_VAL:
			node.type = BOOL_TYPE;
			break;
		case NUMBER_VAL:
			node.type = NUMBER_TYPE;
			break;
		case STRING_VAL:
			node.type = STRING_TYPE;
			break;
		default:
			// unreachable
			error("Unexpected token: ");
			break;
	}
}

void Parser::rvalue(RValue*& rval)
{
	switch(curr_token.type())
	{
		case LBRACE:
		{
			JSON* node = new JSON;
			json(*node);
			rval = node;
			break;
		}
		case LBRACKET:
		{
			Array* node = new Array;
			array(*node);
			rval = node;
			break;
		}
		default:
		{
			SimpleRValue* node = new SimpleRValue;
			simple(*node);
			rval = node;
			break;
		}
	}
}


// Type checks
void Parser::kval()
{
	if(curr_token.type() == STRING_VAL)
	{
		advance();
	} else {
		error("Unexpected token: expected string, ");
	}
}

void Parser::pval()
{
	switch(curr_token.type())
	{
		case BOOL_VAL:
		case NUMBER_VAL:
		case STRING_VAL:
			advance();
			break;
		default:
			error("Unexpected token: expected value, ");
	}
}


#endif
