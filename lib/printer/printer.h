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
	Printer(std::ostream&);
	Printer(std::ostream&, const int&);
	Printer(std::ostream&, const int&, const char&);


	// top-level
	void visit(JSONDocument&);
	void visit(JSON&);
	void visit(Record&);
	void visit(RValue&);
	void visit(SimpleRValue&);
	void visit(Array&);

private:
	const int indent_size;
	const char indent_char;

	std::ostream& out;
	int curr_indent = 0;

	void inc_indent();
	void dec_indent();
	std::string get_indent();
};


#endif // ifndef PRINTER_H
