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

#include <QDockWidget>

class QTabWidget;
class QListWidget;
class QTextBrowser;

class pDockMessageBox : public QDockWidget, public QSingleton<pDockMessageBox>
{
	Q_OBJECT
	friend class QSingleton<pDockMessageBox>;
	
public:
	QString colourText( const QString&, const QColor& = Qt::black );

protected:
	QTabWidget* twMessageBox;
	QListWidget* lwErrors;
	QTextBrowser* tbMessages;

private:
	pDockMessageBox( QWidget* = 0 );
	~pDockMessageBox();

public slots:
	void append( const QString& );
	void appendInBox( const QString&, const QColor& = Qt::red );

private slots:
	void commandError( pCommand*, QProcess::ProcessError );
	void commandFinished( pCommand*, int, QProcess::ExitStatus );
	void commandReadyRead( pCommand*, const QByteArray& );
	void commandStarted( pCommand* );
	void commandStateChanged( pCommand*, QProcess::ProcessState );

signals:
	void saveSettings();
	void restoreSettings();

};

#endif // PDOCKMESSAGEBOX_H
