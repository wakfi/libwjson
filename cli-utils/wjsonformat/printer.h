#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>

// libwjson core modules
#include <core/token.h>
#include <core/ast.h>


class Printer : public Visitor
{
public:
	// constructors
	Printer(std::ostream& output_stream)
	: out(output_stream), indent_size(1), indent_char('\t') {}

	Printer(std::ostream& output_stream, const int& indent)
	: out(output_stream), indent_size(indent), indent_char(' ') {}

	Printer(std::ostream& output_stream, const int& indent_width, const char& indent_character)
	: out(output_stream), indent_size(indent_width), indent_char(indent_character) {}


	// top-level
	void visit(JSON& node);
	void visit(Record& node);
	void visit(RValue& node);
	void visit(SimpleRValue& node);
	void visit(Array& node);

private:
	const int indent_size;
	const char indent_char;

	std::ostream& out;
	int curr_indent = 0;

	void inc_indent() {curr_indent += indent_size;}
	void dec_indent() {curr_indent -= indent_size;}
	std::string get_indent() {return std::string(curr_indent, indent_char);}
};

// top-level
void Printer::visit(JSON& node)
{
	out << "{\n";
	inc_indent();
	std::string maybeComma = "";
	for(Record* record : node.records)
	{
		out << maybeComma;
		record->accept(*this);
		maybeComma = ",\n";
	}
	dec_indent();
	out << "\n" << get_indent() << "}";
	if(!curr_indent) out << std::endl;
}

void Printer::visit(Record& node)
{
	out << get_indent();
	out << '"' << node.key.lexeme() << '"';
	out << ": ";
	node.value->accept(*this);
}

void Printer::visit(SimpleRValue& node)
{
	switch(node.type)
	{
		case BOOL_TYPE:
		case NUMBER_TYPE:
			out << node.value.lexeme();
			break;
		case STRING_TYPE:
			out << '"' << node.value.lexeme() << '"';
			break;
		default:
			break; // no other types
	}
}

void Printer::visit(Array& node)
{
	out << "[\n";
	inc_indent();
	std::string maybeComma = "";
	for(RValue* val : node.values)
	{
		out << maybeComma << get_indent();
		val->accept(*this);
		maybeComma = ",\n";
	}
	dec_indent();
	out << "\n" << get_indent() << "]";
}


#endif
