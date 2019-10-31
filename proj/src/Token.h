#ifndef TOKEN_H
#define TOKEN_H
#include <QString>

enum
{
	TOK_EOF = 128,
	TOK_IF,
	TOK_ELSE,
	TOK_WHILE,
	TOK_DO,
	TOK_FOR,
	TOK_LEXEME,

#define def_op(t, n) t,
#include "def_op.h"
#undef def_op
};

class Token
{
public:
	int tok;
	int line;
	int column;
	QString lexeme;
public:
    Token();
	Token(const Token& t);
	Token(int t, int l, int c, const QString& s);
	~Token();
public:
	operator int() const
	{
		return tok;
	}
	Token& operator = (const Token& t)
	{
		tok = t.tok;
		line = t.line;
		column = t.column;
		lexeme = t.lexeme;
		return *this;
	}
};

#endif // TOKEN_H
