#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
class QTextEdit;

#include "Languages.h"
#include "ControlFlowCanvas.h"
#include <QHash>

class MainWindow : public QMainWindow
{
    Q_OBJECT
	Ui::MainWindow*		ui;
	ControlFlowCanvas*	canvas;
	QTextEdit*			textEdit;

	QString				lastDirectory;
	QString				currentFileName;

	Languages			languages;

	QHash<QDockWidget*, QString> docks;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
	void closeEvent(QCloseEvent* e);
	void changeEvent(QEvent* e);
private:
	bool eventFilter(QObject* obj, QEvent* e);
	QDockWidget* createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas = Qt::AllDockWidgetAreas);
	void newFile();
	void openFile(QString fname);
	bool saveFile(QString fname);
	bool closeFile();
	void updateEnvironment();
private slots:
	void slotAction();
	void slotTextEdit_textChanged();
};

#endif // MAINWINDOW_H
