#include "Token.h"

Token::Token() : tok(TOK_EOF)
{
}
Token::Token(const Token& t)
{
	tok = t.tok;
	line = t.line;
	column = t.column;
	lexeme = t.lexeme;
}
Token::Token(int t, int l, int c, const QString& s) : tok(t), line(l), column(c), lexeme(s)
{
}
Token::~Token()
{
}

