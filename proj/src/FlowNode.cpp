#include "FlowNode.h"
#include "FlowEdge.h"
#include <QPainter>

FlowNode::FlowNode()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	type = TYPE_ROOT;
	rect = QRect(0, 0, 0, 0);
}
FlowNode::FlowNode(int t, const QString& lbl) : type(t), label(lbl)
{
	x = -NODE_WIDTH/2;
	y = 0;
	width = 90;
	height = 25;
	rect = QRect(0, 0, 90, 25);

	// Calcular rectángulo del símbolo
	boundRect =
	symbolRect = QRect(-NODE_WIDTH/2, 0, NODE_WIDTH, NODE_HEIGHT);
	// ...
	topPoint = QPoint(0, 0);
	bottomPoint = QPoint(0, NODE_HEIGHT);
}
FlowNode::~FlowNode()
{
	foreach(FlowEdge* e, edges)
		delete e;
	foreach(FlowNode* n, nodes)
		delete n;
}

void FlowNode::paint(QPainter& p)
{
	p.setPen(Qt::black);
	foreach(FlowEdge* e, edges)
	{
		e->paint(p);
	}
	foreach(FlowNode* n, nodes)
	{
		n->paint(p);
	}
	p.setPen(Qt::black);

	if(type == TYPE_ROOT)
	{
#if 0
		QBrush br;

		br.setStyle(Qt::Dense5Pattern);
		br.setColor(Qt::red);
		QPen pen(br, 3);

		p.setPen(pen);
		p.setBrush(QBrush());
		p.drawRect(boundRect);
#endif
	}
	else if(type == TYPE_ACTION)
	{
		p.setBrush(QColor(148, 203, 255));
		p.drawRect(symbolRect);
	}
	else if(type == TYPE_CONDITION)
	{
		x = symbolRect.x();
		y = symbolRect.y();
		width = symbolRect.width();
		height = symbolRect.height();
		int mx = x + width/2;
		int my = y + height/2;
		QPolygon poly;
		poly.append(QPoint(x, my));
		poly.append(QPoint(mx, y+height));
		poly.append(QPoint(x+width, my));
		poly.append(QPoint(mx, y));
		p.setBrush(QColor(129, 244, 96));
		p.drawPolygon(poly);
	}
	else if(type == TYPE_WHILE || type == TYPE_DOWHILE)
	{
		p.setBrush(QColor(255, 211, 63));
		p.drawRoundRect(symbolRect);
	}
	else if(type == TYPE_START)
	{
		p.setPen(QPen());
		p.setBrush(QColor(0, 0, 0));
		p.drawEllipse(symbolRect);
	}
	else if(type == TYPE_END)
	{
		p.setPen(Qt::black);
		p.setBrush(Qt::white);
		p.drawEllipse(symbolRect);
		int s = 4;
		p.setBrush(Qt::black);
		p.drawEllipse(symbolRect.adjusted(+s, +s, -s, -s));
	}
	if(!label.isEmpty())
	{
		QTextOption opt;
		opt.setAlignment(Qt::AlignCenter);
		p.drawText(symbolRect, label, opt);
	}
#if 0
	{
		p.setBrush(Qt::red);
		p.drawEllipse(topPoint, 5, 5);
		p.setBrush(Qt::green);
		p.drawEllipse(bottomPoint, 5, 5);
	}
#endif
}

