// Since this declares a definition of the Printer class, we need to typeguard it generically
#ifndef PRINTER_CPP
#define PRINTER_CPP

// libwjson core modules
#include <printer/printer.h>


// constructors
Printer::Printer(std::ostream& output_stream)
: out(output_stream), indent_size(0), indent_char(' ') {}

Printer::Printer(std::ostream& output_stream, const int& indent)
: out(output_stream), indent_size(indent), indent_char(' ') {}

Printer::Printer(std::ostream& output_stream, const int& indent_width, const char& indent_character)
: out(output_stream), indent_size(indent_width), indent_char(indent_character) {}


// Indent managers
void Printer::inc_indent() {
}

void Printer::dec_indent() {
}

std::string Printer::get_indent() {
	return "";
}

// top-level
void Printer::visit(JSONDocument& node)
{
	node.root->accept(*this);
	out.flush();
}

void Printer::visit(JSON& node)
{
	if(!node.records.size())
	{
		out << "{}";
		return;
	}
	inc_indent();
	out << "{";
	auto it = node.records.begin();
	(*it)->accept(*this);
	++it;
	for(; it != node.records.end(); ++it)
	{
		out << ",";
		(*it)->accept(*this);
	}
	dec_indent();
	out << get_indent() << "}";
}

void Printer::visit(Record& node)
{
	out << get_indent();
	out << '"' << node.key.lexeme() << '"';
	out << ":";
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
	out << "[" << get_indent();
	auto it = node.values.begin();
	(*it)->accept(*this);
	++it;
	for(; it != node.values.end(); ++it)
	{
		out << "," << get_indent();
		(*it)->accept(*this);
	}
	dec_indent();
	out << get_indent() << "]";
}


#endif // ifndef PRINTER_CPP
