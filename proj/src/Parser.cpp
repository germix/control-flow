#include "Parser.h"
#include "FlowNode.h"
#include "FlowEdge.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// EndFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class EndFlowNode : public FlowNode
{
public:
	EndFlowNode() : FlowNode(TYPE_END, "")
	{
		symbolRect = QRect(-10, 0, 20, 20);
		bottomPoint = QPoint(0, 20);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// StartFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class StartFlowNode : public FlowNode
{
public:
	StartFlowNode() : FlowNode(TYPE_START, "")
	{
		symbolRect = QRect(-10, 0, 20, 20);
		bottomPoint = QPoint(0, 20);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// BlockFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class BlockFlowNode : public FlowNode
{
public:
	BlockFlowNode() : FlowNode()
	{
		bottomPoint = QPoint(0, 0);
	}
public:
	FlowNode* add(FlowNode* n)
	{
		if(nodes.size() > 0)
		{
			n->translate(0, nodes.last()->bottomPoint.y()+VSPACE);

			QPoint p1 = bottomPoint;
			QPoint p2 = n->topPoint;
			addEdge(true, QPolygon() << p1 << p2);
		}
		nodes.append(n);
		boundRect |= n->getBoundRect();
		bottomPoint = n->bottomPoint;

		return n;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// ActionFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class ActionFlowNode : public FlowNode
{
public:
	ActionFlowNode(const QString& e) : FlowNode(TYPE_ACTION, e)
	{
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// IfFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class IfFlowNode : public FlowNode
{
public:
	IfFlowNode(const QString& e, BlockFlowNode* left, BlockFlowNode* right) : FlowNode(TYPE_CONDITION, e)
	{
		QPoint p1,p2,p3;
		QPoint cp;

		//if()
		{
			nodes.append(left);
			left->translate(-left->boundRect.width(), VSPACE*2);
			boundRect |= left->boundRect;
			{
				p1 = QPoint(symbolRect.left(), symbolRect.center().y());
				p2 = QPoint(left->topPoint.x(), p1.y());
				p3 = left->topPoint;
				addEdge(left->boundRect.width() > 0, QPolygon() << p1 << p2 << p3);
			}
			cp.setY(left->bottomPoint.y()+VSPACE);
		}

		if(right)
		{
			nodes.append(right);
			right->translate(+right->boundRect.width(), VSPACE*2);
			boundRect |= right->boundRect;
			{
				p1 = QPoint(symbolRect.right(), symbolRect.center().y());
				p2 = QPoint(right->topPoint.x(), p1.y());
				p3 = right->topPoint;
				addEdge(right->boundRect.width() > 0, QPolygon() << p1 << p2 << p3);
			}
			cp.setY(qMax(cp.y(), right->bottomPoint.y()+VSPACE));
			{
				p1 = right->bottomPoint;
				p2 = QPoint(p1.x(), cp.y());
				p3 = cp;
				addEdge(QPolygon() << p1 << p2 << p3);
			}
		}
		else
		{
			p1 = bottomPoint;
			p2 = cp;
			addEdge(QPolygon() << p1 << p2);
		}
		bottomPoint = cp;

		{
			p1 = left->bottomPoint;
			p2 = QPoint(p1.x(), cp.y());
			p3 = cp;
			addEdge(QPolygon() << p1 << p2 << p3);
		}
		boundRect.setHeight(bottomPoint.y());
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// WhileFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class WhileFlowNode : public FlowNode
{
public:
	WhileFlowNode(const QString& e, BlockFlowNode* r) : FlowNode(TYPE_WHILE, e)
	{
		bottomPoint = QPoint(0, height);
		// ...
		nodes.append(r);
		r->translate(0, symbolRect.height()+VSPACE);

		boundRect |= r->getBoundRect();

		if(r->nodes.size())
		{
			bottomPoint = r->bottomPoint;
		}
		boundRect.setHeight(boundRect.height() + VSPACE*2);
		bottomPoint.setY(boundRect.bottom());

		//
		// Edge 1
		//
		int minX = 0;
		int maxX = 0;
		if(r->nodes.size())
		{
			QPoint p1, p2, p3, p4, p5;
			minX = symbolRect.x();
			for(int i = 0; i < r->nodes.size(); i++)
			{
				minX = qMin(minX, r->nodes.at(i)->boundRect.x());
			}
			//p1 = QPoint(r->nodes.last()->symbolRect.center().x(), r->nodes.last()->symbolRect.center().y());
			//p1 = QPoint(r->nodes.last()->symbolRect.center().x(), r->nodes.last()->bottomPoint.y());
			p1 = r->bottomPoint;
			p2 = QPoint(p1.x(), p1.y()+VSPACE);
			p3 = QPoint(minX - HSPACE, p2.y());
			p4 = QPoint(p3.x(), symbolRect.center().y());
			p5 = QPoint(symbolRect.left(), p4.y());
			addEdge(true, QPolygon() << p1 << p2 << p3 << p4 << p5);

			p1 = QPoint(0, symbolRect.bottom());
			p2 = QPoint(0, r->nodes.first()->symbolRect.y());
			addEdge(true, QPolygon() << p1 << p2);
		}
		//
		// Edge 2
		//
		{
			QPoint p1, p2, p3, p4;

			maxX = boundRect.right();
			for(int i = 0; i < r->nodes.size(); i++)
			{
				maxX = qMax(maxX, r->nodes.at(i)->symbolRect.right());
			}
			p1 = QPoint(symbolRect.right(), symbolRect.center().y());
			p2 = QPoint(maxX+HSPACE, p1.y());
			p3 = QPoint(p2.x(), bottomPoint.y());
			p4 = bottomPoint;
			addEdge(QPolygon() << p1 << p2 << p3 << p4);
		}
		boundRect.setLeft(qMin(minX-HSPACE, boundRect.left()));
		boundRect.setRight(qMax(maxX+HSPACE, boundRect.right()));
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// DoWhileFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
class DoWhileFlowNode : public FlowNode
{
public:
	DoWhileFlowNode(const QString& e, BlockFlowNode* r) : FlowNode(TYPE_DOWHILE, e)
	{
		nodes.append(r);

		r->translate(0, VSPACE);
		symbolRect.translate(0, r->boundRect.height()+VSPACE*2);

		boundRect |= r->getBoundRect();
		boundRect |= symbolRect;
		topPoint = QPoint(0, symbolRect.top());
		bottomPoint = QPoint(0, symbolRect.bottom());

		int minX = qMin(symbolRect.left(), r->boundRect.left())-HSPACE;
		QPoint p1, p2, p3, p4;

		p1 = QPoint(symbolRect.left(), symbolRect.center().y());
		p2 = QPoint(minX, p1.y());
		p3 = QPoint(minX, 0);
		p4 = QPoint(0, 0);
		addEdge(true, QPolygon() << p1 << p2 << p3 << p4);
		boundRect.setLeft(minX);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
// ForFlowNode
////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class ForFlowNode : public FlowNode
{
public:
	ForFlowNode()
	{

	}
};
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::stmt(BlockFlowNode* blk)
{
	QString s;

	if(tok == TOK_IF)
	{
		next();
		match('(');
		s = readExpression();
		match(')');

		BlockFlowNode* c1 = 0;
		BlockFlowNode* c2 = 0;

		c1 = block();
		if(tok == TOK_ELSE)
		{
			next();
			c2 = block();
		}
		blk->add(new IfFlowNode(s, c1, c2));
	}
	else if(tok == TOK_DO)
	{
		next();
		BlockFlowNode* r = block();

		match(TOK_WHILE);
		match('(');
		QString s = readExpression(')');
		match(')');
		match(';');
		blk->add(new DoWhileFlowNode(s, r));
	}
	else if(tok == TOK_WHILE)
	{
		next();
		match('(');
		s = readExpression();
		match(')');

		BlockFlowNode* r = block();

		blk->add(new WhileFlowNode(s, r));
	}
	else if(tok == TOK_FOR)
	{
		next();
		match('(');
		QString e1 = readExpression(';');
		match(';');
		QString e2 = readExpression(';');
		match(';');
		QString e3 = readExpression(')');
		match(')');

		BlockFlowNode* blk2 = block();

		blk->add(new ActionFlowNode(e1));
		blk2->add(new ActionFlowNode(e3));
		blk->add(new WhileFlowNode(e2, blk2));
	}
	else if(tok == '{')
	{
		next();
		while(tok != TOK_EOF && tok != '}')
		{
			stmt(blk);
		}
		match('}');
	}
	else //if(tok == TOK_LEXEME)
	{
		s = readExpression(';');

		match(';');
		blk->add(new ActionFlowNode(s));
	}
}

Parser::Parser()
{
}
Parser::~Parser()
{
}
FlowNode* Parser::parse(const QString& text)
{
	if(lex.init(text))
	{
#if 1
		next();
		root = new BlockFlowNode();

		root->add(new StartFlowNode());
		while(tok != TOK_EOF)
		{
			stmt(root);
		}
		root->add(new EndFlowNode());
		return root;
#else
		next();
		while(tok != TOK_EOF)
		{
			qDebug("tok=%ls", tok.lexeme.constData());
			next();
		}
#endif
	}
	return NULL;
}
void Parser::next()
{
	tok = lex.next();
}
void Parser::match(int t)
{
	if(tok != t)
	{
	}
	next();
}
BlockFlowNode* Parser::block()
{
	BlockFlowNode* blk = new BlockFlowNode();
	if(tok == '{')
	{
		next();
		while(tok != TOK_EOF && tok != '}')
		{
			stmt(blk);
		}
		match('}');
	}
	else
	{
		stmt(blk);
	}
	return blk;
}
QString Parser::readExpression(int stop)
{
	QString s;
	while(tok != TOK_EOF)
	{
		if(!s.isEmpty())
			s += " ";
		s += tok.lexeme;
		next();
		if(tok == stop)
			break;
	}
	return s;
}


