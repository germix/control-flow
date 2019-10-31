#ifndef FLOWEDGE_H
#define FLOWEDGE_H

class QPainter;

#include "QPolygon"

class FlowNode;

class FlowEdge
{
	FlowNode*	from;
	FlowNode*	to;
	int			dx;
public:
	QPolygon		points;
	bool			hasArrow;
public:
	FlowEdge(const QVector<QPoint>& p, bool a)
	{
		points = p;
		hasArrow = a;
	}
	FlowEdge(FlowNode* f, FlowNode* t) : from(f), to(t), dx(0)
	{
	}
	FlowEdge(FlowNode* f, FlowNode* t, int d) : from(f), to(t), dx(d)
	{
	}
	~FlowEdge()
	{
	}
public:
	void paint(QPainter& p);
};

#endif // FLOWEDGE_H
