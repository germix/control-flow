#include "Lexer.h"

static int is_eof(int c){return (c == (-1));}
static int is_space(int c){return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n');}
static int is_digit(int c) { return (c >= '0' && c <= '9'); }
static int is_identifier(int c){return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c >= '0' && c <= '9'));}
static int is_start_identifier(int c){return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));}

#define PARSE1(c1, tok1)		\
	case c1:					\
	{							\
		in.read();				\
		tok = tok1;				\
	}							\
	break;

#define PARSE2(c1, tok1, c2, tok2)		\
	case c1:							\
	{									\
		tok = tok1;						\
		if(in.peek() == c2)			\
		{								\
			in.read();					\
			tok = tok2;					\
		}								\
	}									\
	break;

#define PARSE3(c1, tok1, c2, tok2, c3, tok3)	\
	case c1:									\
	{											\
		tok = tok1;								\
		switch(in.peek())						\
		{										\
			case c2:							\
				in.read();						\
				tok = tok2;						\
				break;							\
			case c3:							\
				in.read();						\
				tok = tok3;						\
				break;							\
		}										\
	}											\
	break;

#define PARSE4(c1, tok1, c2, tok2, c3, tok3, c4, tok4)	\
	case c1:											\
	{													\
		tok = tok1;										\
		switch(in.peek())								\
		{												\
			case c2:									\
				in.read();									\
				tok = tok2;								\
				break;									\
			case c3:									\
				in.read();								\
				tok = tok3;								\
				break;									\
			case c4:									\
				in.read();								\
				tok = tok4;								\
				break;									\
		}												\
	}													\
	break;


Lexer::Lexer()
{
}
Lexer::~Lexer()
{
}
bool Lexer::init(const QString& text)
{
	in.init(text);
	return true;
}
Token Lexer::next()
{
	int c;
	int tok = 0;
	int line;
	int column;
//	QString lexeme;

	while(tok == 0)
	{
		c = in.peek();
		line = in.line;
		column = in.column;
		in.lexeme.clear();
		if(is_eof(c))
		{
			tok = TOK_EOF;
		}
		else if(is_space(c))
		{
			in.read();
		}
		else if(is_digit(c))
		{
			//lexeme += QChar(in.read());
			in.read();
			do
			{
				c = in.peek();
				if(c == EOF || !is_digit(c))
					break;
				//lexeme += QChar(in.read());
				in.read();
			}
			while(1);
			tok = TOK_LEXEME;
		}
		else if(is_start_identifier(c))
		{
			//lexeme += QChar(in.read());
			in.read();
			do
			{
				c = in.peek();
				if(c == EOF || !is_identifier(c))
					break;
				//lexeme += QChar(in.read());
				in.read();
			}
			while(1);
			tok = TOK_LEXEME;

			if(in.lexeme == "if")			tok = TOK_IF;
			else if(in.lexeme == "else")	tok = TOK_ELSE;
			else if(in.lexeme == "while")	tok = TOK_WHILE;
			else if(in.lexeme == "do")		tok = TOK_DO;
			else if(in.lexeme == "for")		tok = TOK_FOR;
		}
		else
		{
			switch(c = in.read())
			{
				case '.':
					if(in.peek() == '.')
					{
						in.read();
						if(in.read() == '.')
							tok = TOK_ELLIPSIS;
						//else
							//error("Error(%d,%d): expected \'.\'", line, column);
					}
					else
					{
						tok = c;
					}
					break;
				case '<':
					tok = c;
					switch(in.peek())
					{
						case '=':
							in.read();
							tok = TOK_LE;
							break;
						case '<':
							in.read();
							tok = TOK_SHL;
							if(in.peek() == '=')
							{
								in.read();
								tok = TOK_A_SHL;
							}
							break;
					}
					break;
				case '>':
					tok = c;
					switch(in.peek())
					{
						case '=':
							in.read();
							tok = TOK_GE;
							break;
						case '>':
							in.read();
							tok = TOK_SHR;
							if(in.peek() == '=')
							{
								in.read();
								tok = TOK_A_SHR;
							}
							break;
					}
					break;
				PARSE2('!', '!', '=', TOK_NE)
				PARSE2('=', '=', '=', TOK_EQ)
				PARSE2('*', '*', '=', TOK_A_MUL)
				PARSE2('%', '%', '=', TOK_A_MOD)
				PARSE2('^', '^', '=', TOK_A_XOR)

				PARSE3('+', '+', '+', TOK_INC, '=', TOK_A_ADD)
				PARSE4('-', '-', '-', TOK_DEC, '=', TOK_A_SUB, '>', TOK_ARROW)
				PARSE3('|', '|', '|', TOK_L_OR, '=', TOK_A_OR)
				PARSE3('&', '&', '&', TOK_L_AND, '=', TOK_A_AND)

				// Comentario ú operadores
				case '/':
					switch(in.peek())
					{
						case '=':
							in.read();
							tok = TOK_A_DIV;
							break;
						case '/':
							skipCommentLine();
							break;
						case '*':
							skipCommentBlock();
							break;
						default:
							tok = '/';
							break;
					}
					break;
				// Tokens simples
				case '(':
				case ')':
				case '[':
				case ']':
				case '{':
				case '}':
				case ',':
				case ';':
				case ':':
				case '?':
				case '~':
				case '@':		// dito
				case '$':		// solo usado en ensamblador
					tok = c;
					break;
				default:
					//error("Error(%d,%d): Invalid character \'\\x%02x\'", line, column, c);
					break;
			}
		}
	}
	return Token(tok, line, column, in.lexeme);
}
void Lexer::skipCommentLine()
{
}
void Lexer::skipCommentBlock()
{
}
