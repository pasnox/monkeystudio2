#ifndef MESSAGEBOXDOCKS_H
#define MESSAGEBOXDOCKS_H

#include "ui_UIBuildStep.h"
#include "ui_UIOutput.h"
#include "ui_UICommand.h"
#include "ui_UISearchResult.h"

#include <fresh.h>
#include <consolemanager.h>

class UIBuildStep : public pDockWidget, public Ui::UIBuildStep
{
public:
	UIBuildStep( QWidget* parent = 0 )
		: pDockWidget( parent )
	{ setupUi( this ); }
};

class UIOutput : public pDockWidget, public Ui::UIOutput
{
public:
	UIOutput( QWidget* parent = 0 )
		: pDockWidget( parent )
	{ setupUi( this ); }
};

class UICommand : public pDockWidget, public Ui::UICommand
{
public:
	UICommand( QWidget* parent = 0 )
		: pDockWidget( parent )
	{ setupUi( this ); }
};

class UISearchResult : public pDockWidget, public Ui::UISearchResult
{
public:
	UISearchResult( QWidget* parent = 0 )
		: pDockWidget( parent )
	{ setupUi( this ); }
};

class MessageBoxDocks : public QObject
{
	Q_OBJECT
	friend class MessageBox;
	
public:
	MessageBoxDocks( QObject* parent = 0 );
	~MessageBoxDocks();
	
	QString colourText( const QString&, const QColor& = Qt::black );

protected:
	UIBuildStep* mBuildStep;
	UIOutput* mOutput;
	UICommand* mCommand;
	UISearchResult* mSearchResult;

public slots:
	void appendOutput( const QString& );
	void appendLog( const QString& );
	void appendInBox( const QString&, const QColor& = Qt::red );
	void appendStep( const pConsoleManager::Step& );
	void appendSearchResult( const pConsoleManager::Step& );
	void showBuild();
	void showOutput();
	void showLog();
	void showSearchResults();
	void showNextError();
	void clearSearchResults();

protected slots:
	void lwBuildSteps_itemPressed( QListWidgetItem* );
	void lwSearchResults_itemPressed( QListWidgetItem* );
	void cbRawCommand_returnPressed();
	void commandError( const pCommand&, QProcess::ProcessError );
	void commandFinished( const pCommand&, int, QProcess::ExitStatus );
	void commandReadyRead( const pCommand&, const QByteArray& );
	void commandStarted( const pCommand& );
	void commandStateChanged( const pCommand&, QProcess::ProcessState );
	void commandSkipped( const pCommand& );
};

#endif // MESSAGEBOXDOCKS_H
