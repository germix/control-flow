#ifndef PARSER_H
#define PARSER_H
#include "Lexer.h"
#include "Token.h"

class FlowNode;
class Context;

#include "FlowNode.h"

#define VSPACE 20
#define HSPACE 20




class BlockFlowNode;

class Parser
{
	Lexer lex;
	Token tok;
	BlockFlowNode* root;
public:
    Parser();
	~Parser();
public:
	FlowNode* parse(const QString& text);
private:
	void next();
	void match(int t);

	void stmt(BlockFlowNode* blk);
	BlockFlowNode* block();

	QString readExpression(int stop = ')');
};

#endif // PARSER_H

