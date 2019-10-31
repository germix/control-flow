#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDockWidget>
#include <QKeyEvent>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextStream>

#include "utils.h"
#include "Parser.h"
#include "Highlighter.h"
#include "AboutDialog.h"

#define SETTINGS_APPLICATION "ControlFlow"
#define SETTINGS_ORGANIZATION "Germix"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	languages.init(ui->menuLanguages, "translations", "controlflow", s.value("Language").toString());


	textEdit = new QTextEdit();
	new Highlighter(textEdit->document());
	setCentralWidget(textEdit);
	{
		const int tabStop = 4;  // 4 characters
		QFontMetrics metrics(textEdit->font());
		textEdit->setTabStopWidth(tabStop * metrics.width(' '));
	}
	connect(textEdit, SIGNAL(textChanged()), this, SLOT(slotTextEdit_textChanged()));
	textEdit->installEventFilter(this);

	// ...
	ui->menuWindows->addAction(ui->mainToolBar->toggleViewAction());
	ui->menuWindows->addSeparator();


	canvas = new ControlFlowCanvas();
	createDockPanel(canvas, "ControlFlowCanvas", tr("Control flow"), "Control flow", true, Qt::RightDockWidgetArea);

	//
	// Restaurar estado anterior
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());
	// ...
	updateEnvironment();
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());
	s.setValue("Language", languages.language());

    delete ui;
}
void MainWindow::closeEvent(QCloseEvent* e)
{
	if(closeFile())
		e->accept();
	else
		e->ignore();
}
void MainWindow::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
#if 1
			case QEvent::LocaleChange:
				{
					QString locale = QLocale::system().name();
					locale.truncate(locale.lastIndexOf('_'));
					languages.load(locale);
				}
				break;
#endif
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
				updateEnvironment();
				for(QHash<QDockWidget*, QString>::ConstIterator it = docks.constBegin(); it != docks.constEnd(); it++)
				{
					QDockWidget* dw = it.key();
					QString      title = it.value();
					dw->setWindowTitle(tr((const char*)title.toLatin1().constData()));
				}
				break;
			default:
				break;
		}
	}
	QMainWindow::changeEvent(e);
}
bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
	if(obj == textEdit && e->type() == QEvent::KeyPress)
	{
		QKeyEvent* evt = (QKeyEvent*)e;
		if((evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) && evt->modifiers() & Qt::ControlModifier)
		{
			FlowNode* node;
			Parser parser;
			if(NULL != (node = parser.parse(textEdit->toPlainText())))
			{
				canvas->setRoot(node);
			}
			return true;
		}
	}
	return QMainWindow::eventFilter(obj, e);
}
QDockWidget* MainWindow::createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas)
{
	//
	// Crear el dock widget
	//
	QDockWidget* dock = new QDockWidget(title, this);
	dock->setObjectName(name);
	dock->setWidget(widget);
	dock->setAllowedAreas(allowedAreas);
	if(!restoreDockWidget(dock))
	{
		dock->setVisible(visible);
		addDockWidget(initialArea, dock);
	}
	docks.insert(dock, rawTitle);

	//
	// Obtener la acción y agregarla al menu
	//
	QAction* action = dock->toggleViewAction();

	ui->menuWindows->addAction(action);

	return dock;
}
void MainWindow::newFile()
{
	if(closeFile())
	{
		textEdit->clear();
		updateEnvironment();
	}
}
void MainWindow::openFile(QString fname)
{
	if(closeFile())
	{
		QFile file(fname);
		if(file.open(QFile::ReadOnly))
		{
			QTextStream ts(&file);
			textEdit->setPlainText(ts.readAll());
			currentFileName = QFileInfo(fname).absoluteFilePath();

			updateEnvironment();
		}
	}
}
bool MainWindow::saveFile(QString fname)
{
	if(fname.isEmpty())
	{
		fname = QFileDialog::getSaveFileName(
					NULL,
					tr("Save file"),
					lastDirectory,
					tr("Text files") + " (*.txt);;");
	}
	if(!fname.isNull())
	{
		QFile file(fname);
		if(file.open(QFile::WriteOnly))
		{
			file.write(textEdit->toPlainText().toLatin1());

			currentFileName = QFileInfo(fname).absoluteFilePath();
			textEdit->document()->setModified(false);
			updateEnvironment();

			return true;
		}
	}
	return false;
}

bool MainWindow::closeFile()
{
	bool modified = textEdit->document()->isModified();

	if(modified)
	{
		QString s;
		if(currentFileName.isEmpty())
			s = tr("Untitled");
		else
			s = QFileInfo(currentFileName).fileName();

		switch(QMessageBox::question(this,
									 tr("Save"),
									 tr("Save file \"%1\"?").arg(s),
									 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes:
				if(!saveFile(currentFileName))
				{
					return false;
				}
				break;
			case QMessageBox::Cancel:
				return false;
			default:
			case QMessageBox::No:
				break;
		}
	}
	canvas->clear();
	currentFileName.clear();
	// ...
	return true;
}
void MainWindow::updateEnvironment()
{
	QString title;
	bool modified = textEdit->document()->isModified();

	ui->actionFileSave->setEnabled(modified);

	ui->actionFileOpenContainerFolder->setEnabled(!currentFileName.isEmpty());

	title += "Control Flow - ";
	if(currentFileName.isEmpty())
		title += tr("Untitled");
	else
		title += QFileInfo(currentFileName).fileName();

	if(textEdit->document()->isModified())
		title += " *";
	setWindowTitle(title);
}

void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionFileNew)
	{
		newFile();
	}
	else if(action == ui->actionFileOpen)
	{
		QString fname = QFileDialog::getOpenFileName(
					this,
					tr("Open file"),
					lastDirectory,
					tr("Text files") + " (*.txt);");
		if(!fname.isEmpty())
		{
			openFile(fname);
		}
	}
	else if(action == ui->actionFileSave)
	{
		saveFile(currentFileName);
	}
	else if(action == ui->actionFileSaveAs)
	{
		saveFile(QString());
	}
	if(action == ui->actionFileExit)
	{
		close();
	}
	else if(action == ui->actionHelpAbout)
	{
		AboutDialog().exec();
	}
	else if(action == ui->actionFileOpenContainerFolder)
	{
		ShowInContainerFolder(currentFileName);
	}
}
void MainWindow::slotTextEdit_textChanged()
{
	updateEnvironment();
}




