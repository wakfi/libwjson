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
	void visit(JSONDocument& node);
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
void Printer::visit(JSONDocument& node)
{
	node.root->accept(*this);
	out << std::endl;
}

void Printer::visit(JSON& node)
{
	if(!node.records.size())
	{
		out << "{}";
		if(!curr_indent) out << std::endl;
		return;
	}
	inc_indent();
	out << "{\n";
	auto it = node.records.begin();
	(*it)->accept(*this);
	++it;
	for(; it != node.records.end(); ++it)
	{
		out << ",\n";
		(*it)->accept(*this);
	}
	dec_indent();
	out << "\n" << get_indent() << "}";
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
		case LITERAL_TYPE:
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
	if(!node.values.size())
	{
		out << "[]";
		return;
	}
	inc_indent();
	out << "[\n" << get_indent();
	auto it = node.values.begin();
	(*it)->accept(*this);
	++it;
	for(; it != node.values.end(); ++it)
	{
		out << ",\n" << get_indent();
		(*it)->accept(*this);
	}
	dec_indent();
	out << "\n" << get_indent() << "]";
}


#endif
