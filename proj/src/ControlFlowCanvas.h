#ifndef CONTROLFLOWCANVAS_H
#define CONTROLFLOWCANVAS_H
#include <QWidget>

class FlowNode;

class ControlFlowCanvas : public QWidget
{
	Q_OBJECT
	FlowNode*		root;
	QPoint			refPos;
	bool			dragging;
	QPoint			translation;
public:
	ControlFlowCanvas();
	~ControlFlowCanvas();
public:
	void clear()
	{
		setRoot(0);
	}
	void setRoot(FlowNode* n);
private:
	void paintEvent(QPaintEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
};

#endif // CONTROLFLOWCANVAS_H
