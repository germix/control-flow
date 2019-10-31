#include "FlowEdge.h"
#include <QPainter>
#include "FlowNode.h"

void FlowEdge::paint(QPainter& p)
{
	if(!points.isEmpty())
	{
		p.drawPolyline(points);
		if(hasArrow)
		{
			QPoint p1 = points.at(points.size()-2);
			QPoint p2 = points.at(points.size()-1);
			QPoint points[3];
			int size = 10;

			points[0] = p2;

			p.setBrush(Qt::black);
			if(p1.y() == p2.y())
			{
				if(p1.x() < p2.x())
				{
					// Right
					points[1] = p2 + QPoint(-size, -(size/2));
					points[2] = p2 + QPoint(-size, +(size/2));
					p.drawPolygon(points, 3);
				}
				else
				{
					points[1] = p2 + QPoint(size, -(size/2));
					points[2] = p2 + QPoint(size, +(size/2));
					p.drawPolygon(points, 3);
				}
			}
			else if(p1.x() == p2.x())
			{
				if(p1.y() < p2.y())
				{
					points[1] = p2 + QPoint(-(size/2), -size);
					points[2] = p2 + QPoint(+(size/2), -size);
					p.drawPolygon(points, 3);
				}
			}
		}
	}
	else if(dx == 0)
	{
		p.drawLine(from->getRect().center(), to->getRect().center());
	}
	else
	{
		QRect rf = from->getRect();
		QRect rt = to->getRect();
		int maxRight = qMax(rf.right(), rt.right());
		QPolygon poly;
		poly.append(QPoint(rf.right(), rf.center().y()));
		poly.append(QPoint(maxRight+dx, rf.center().y()));
		poly.append(QPoint(maxRight+dx, rt.center().y()));
		poly.append(QPoint(rt.right(), rt.center().y()));

		p.drawPolyline(poly);
	}
}
