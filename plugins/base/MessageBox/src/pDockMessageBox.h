/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pDockMessageBox
** FileName  : pDockMessageBox.h
** Date      : 2007-08-30T18:52:51
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PDOCKMESSAGEBOX_H
#define PDOCKMESSAGEBOX_H

#include "QSingleton.h"
#include "pConsoleManager.h"
#include "pDockWidget.h"

class QLabel;
class QLineEdit;
class QTabWidget;
class QListWidget;
class QListWidgetItem;
class QTextBrowser;
class QTextEdit;

class pDockMessageBox : public pDockWidget, public QSingleton<pDockMessageBox>
{
	Q_OBJECT
	friend class QSingleton<pDockMessageBox>;
	friend class MessageBox;
	
public:
	QString colourText( const QString&, const QColor& = Qt::black );

protected:
	bool mShown;
	QLabel* lRawCommand;
	QLineEdit* leRawCommand;
	QTabWidget* twMessageBox;
	QListWidget* lwBuildSteps;
	QTextBrowser* tbOutput;
	QTextEdit* teLog;
	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );

private:
	pDockMessageBox( QWidget* = 0 );
	~pDockMessageBox();

public slots:
	void appendOutput( const QString& );
	void appendLog( const QString& );
	void appendInBox( const QString&, const QColor& = Qt::red );
	void appendStep( const pConsoleManager::Step& );
	void showBuild();
	void showOutput();
	void showLog();

private slots:
	void lwBuildSteps_itemDoubleClicked( QListWidgetItem* );
	void leRawCommand_returnPressed();
	void commandError( const pCommand&, QProcess::ProcessError );
	void commandFinished( const pCommand&, int, QProcess::ExitStatus );
	void commandReadyRead( const pCommand&, const QByteArray& );
	void commandStarted( const pCommand& );
	void commandStateChanged( const pCommand&, QProcess::ProcessState );
	void commandSkipped( const pCommand& );

signals:
	void saveSettings();
	void restoreSettings();

};

#endif // PDOCKMESSAGEBOX_H
