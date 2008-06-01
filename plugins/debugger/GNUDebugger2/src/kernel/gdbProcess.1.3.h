#ifndef GDBPROCESS_H
#define GDBPROCESS_H
//

#include <QProcess>
#include <QBuffer>
#include <QPoint>
#include <QHash>


class  GdbProcess : public QProcess
{
	Q_OBJECT
	
public:

	GdbProcess( QObject *parent = 0 );
	~GdbProcess();

private:
	
	QByteArray crlf;
	QString mCommand;
	QString mDir;

public slots:

	void sendRawData( const QByteArray & );

	void stopProcess();

	void setCommand( const QString & );
	void setWorkingDirectorie(const QString &);
	void startProcess();

private slots:

//	void error( QProcess::ProcessError );
//	void finished( int, QProcess::ExitStatus );
	void readyRead();
//	void started();
//	void stateChanged( QProcess::ProcessState );

signals:

	void commandReadyRead(  const QByteArray & );

/*	void commandError( const pCommand&, QProcess::ProcessError );
	void commandFinished( const pCommand&, int, QProcess::ExitStatus );
	void commandReadyRead( const pCommand&, const QByteArray& );
	void commandStarted( const pCommand& );
	void commandStateChanged( const pCommand&, QProcess::ProcessState );
	void commandSkipped( const pCommand& );
	void newStepAvailable( const GdbProcess::Step& );
*/};


//
#endif // GDBPROCESS_H
