#include "Input.h"

Input::Input()
{
}
Input::~Input()
{
}
int Input::peek()
{
	if(pos < buf.size())
	{
		return buf.at(pos).unicode();
	}
	return EOF;
}
int Input::read()
{
	int c = peek();
	pos++;
	column++;
	if(c == '\n')
	{
		line++;
		column = 0;
	}
	lexeme += QChar(c);
	return c;
}
bool Input::init(const QString& txt)
{
	buf = txt;
	pos = 0;
	line = 0;
	column = 0;
	return true;
}
