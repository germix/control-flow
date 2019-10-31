#ifndef LEXER_H
#define LEXER_H
#include "Input.h"
#include "Token.h"

class Lexer
{
	Input in;
public:
    Lexer();
	~Lexer();
public:
	bool init(const QString& text);
	Token next();
private:
	void skipCommentLine();
	void skipCommentBlock();
};

#endif // LEXER_H
