#include "ControlFlowCanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include "FlowNode.h"

ControlFlowCanvas::ControlFlowCanvas()
{
	root = 0;
	setCursor(Qt::OpenHandCursor);
}
ControlFlowCanvas::~ControlFlowCanvas()
{
	if(root)
		delete root;
}
void ControlFlowCanvas::setRoot(FlowNode* n)
{
	if(root)
		delete root;
	root = n;

	translation = QPoint(width()/2, 10);

	update();
}
void ControlFlowCanvas::paintEvent(QPaintEvent* e)
{
	QPainter p(this);

	p.fillRect(rect(), Qt::white);

	if(root)
	{
		p.setRenderHint(QPainter::Antialiasing);
		p.translate(translation);

		root->paint(p);
	}
}
void ControlFlowCanvas::mouseMoveEvent(QMouseEvent* e)
{
	if(dragging)
	{
		translation += e->pos()-refPos;
		update();
		refPos = e->pos();
	}
}
void ControlFlowCanvas::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		dragging = true;

		setCursor(Qt::ClosedHandCursor);
	}
}
void ControlFlowCanvas::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		dragging = false;
		setCursor(Qt::OpenHandCursor);
	}
}

