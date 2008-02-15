#ifndef GDBPROCESS_H
#define GDBPROCESS_H
//

#include "pCommand.h"



#include <QProcess>
#include <QBuffer>
#include <QPoint>
#include <QHash>


class  GdbProcess : public QProcess
{
	Q_OBJECT
	
public:
	GdbProcess( QObject* = 0 );
	~GdbProcess();

	enum StepType { stUnknown = -1, stError, stWarning, stCompiling, stLinking, stFinish, stGood, stBad }; // , stState
	
	struct Step
	{
		Step() {}
		Step( GdbProcess::StepType t ) { mType = t; }
		QString mFileName;
		QPoint mPosition;
		StepType mType;
		QString mText;
		QString mFullText;
	};
	
	inline pCommand currentCommand() const { return mCommands.value( 0 ); }
//	inline QStringList parsersName() const { return mParsers.keys(); }
//	inline QAction* stopAction() const { return mStopAction; }
	
	pCommand processCommand( pCommand );
	pCommand getCommand( const pCommandList&, const QString& );
	pCommandList recursiveCommandList( const pCommandList&, pCommand );
	QString processInternalVariables( const QString& s );
	
protected:
	int mTimerId;
	QBuffer mBuffer; //All output comming to this buffer
	QString mStringBuffer; //... then by portions to this buffer
	pCommandList mCommands;
	QByteArray crlf;

	void timerEvent( QTimerEvent* );


public slots:
	void sendRawCommand( const QString& );
	void sendRawData( const QByteArray& );
	void stopCurrentCommand( bool = false );
	void addCommand( const pCommand& );
	void addCommands( const pCommandList& );
	void removeCommand( const pCommand& );
	void removeCommands( const pCommandList& );
	void executeProcess();

private slots:
	void error( QProcess::ProcessError );
	void finished( int, QProcess::ExitStatus );
	void readyRead();
	void started();
	void stateChanged( QProcess::ProcessState );

signals:
	void commandError( const pCommand&, QProcess::ProcessError );
	void commandFinished( const pCommand&, int, QProcess::ExitStatus );
	void commandReadyRead( const pCommand&, const QByteArray& );
	void commandStarted( const pCommand& );
	void commandStateChanged( const pCommand&, QProcess::ProcessState );
	void commandSkipped( const pCommand& );
	void newStepAvailable( const GdbProcess::Step& );

};


//
#endif // GDBPROCESS_H
