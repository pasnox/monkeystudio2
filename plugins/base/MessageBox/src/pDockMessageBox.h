/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : pDockMessageBox.h
** Date      : 2008-01-14T00:40:02
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
	void showNextError();

private slots:
	void lwBuildSteps_itemPressed( QListWidgetItem* );
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
