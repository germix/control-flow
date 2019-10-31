#ifndef FLOWNODE_H
#define FLOWNODE_H
#include <QVector>
#include <QString>
#include <QRect>

class QPainter;

class FlowEdge;

#include "FlowEdge.h"

#define NODE_WIDTH		90
#define NODE_HEIGHT		25


class FlowNode
{
public:
	enum
	{
		TYPE_ROOT,
		TYPE_ACTION,
		TYPE_CONDITION,
		TYPE_WHILE,
		TYPE_DOWHILE,
		TYPE_END,
		TYPE_START
	};
	int					type;
	QVector<FlowNode*>	nodes;
	QVector<FlowEdge*>	edges;
	QString		label;

	int			x;
	int			y;
	int			width;
	int			height;


	QRect		rect;

	QRect		boundRect;
	QRect		symbolRect;


	QPoint		topPoint;
	QPoint		bottomPoint;
public:
	FlowNode();
	FlowNode(int t, const QString& lbl);
	virtual ~FlowNode();

	void paint(QPainter& p);
public:

	void translate(int dx, int dy)
	{
		x += dx;
		y += dy;
		rect.translate(dx, dy);

		for(int i = 0; i < nodes.size(); i++)
			nodes.at(i)->translate(dx, dy);
		for(int i = 0; i < edges.size(); i++)
			edges.at(i)->points.translate(dx, dy);

		bottomPoint += QPoint(dx, dy);

		topPoint += QPoint(dx, dy);
		boundRect.translate(dx, dy);
		symbolRect.translate(dx, dy);
	}
	QRect getRect() const { return rect; }
//	QRect getRect() const { return QRect(x, y, width, height); }
	int getX() const { return rect.x(); }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

//	QRect getRect() const { return rect; }
//	int getWidth() const { return rect.width(); }
//	int getHeight() const { return rect.height(); }

	virtual QPoint getbottomPoint() const { return bottomPoint; }

	QRect getBoundRect() const { return boundRect; }


	void addEdge(const QPolygon& polyline)
	{
		edges.append(new FlowEdge(polyline, false));
	}
	void addEdge(bool arrow, const QPolygon& polyline)
	{
		edges.append(new FlowEdge(polyline, arrow));
	}
};

#endif // FLOWNODE_H
