#ifndef GDBPROCESS_H
#define GDBPROCESS_H
//
#include "fresh.h"

#include <QProcess>
#include <QBuffer>
#include <QPoint>
#include <QHash>
#include <QTimer>

#include "gdbParser.1.3.h"

//! Remote Gdb
/**
\author xiantia
\version 1.3.2

This Class is QProcess. It use for remote Gdb, send and receive data.

For send command to Gdb use sendRawData() 
*/
class  GdbProcess : public QProcess, public QSingleton<GdbProcess>
{

	Q_OBJECT
	friend class QSingleton<GdbProcess>;

public:

	GdbProcess( QObject *parent = 0 );
	~GdbProcess();

private:
	
	//! Current crlf
	QString crlf;
	//! Current command
	QString mCommand;
	//! Current work directorie
	QString mDir;
	//! List of commands
	QList<QString> mCmdList;
	//! 
	QTimer t;

public slots:

	//! Send command to Gdb, the command not have crlf at end.
	void sendRawData( const QString& );

	//! Stop Gdb
	void stopProcess();
	//! Try stop target
	void stopTarget();

	//! Add new command, if Gdb execute other command, this new command is standbye
	void setCommand( const QString & );
	//! Set work directorie
	void setWorkingDirectorie(const QString &);
	
	//! Start Gdb
	void startProcess();
	//! Clear command in standbye
	void clearAllCommand();

private slots:

	//! New data is avaible
	void readyRead();
	//! Execute new command only if GdbParser is ready and have a command
	void onTimer();
	
signals:

	//! Data can be read from Gdb
	void commandReadyRead(  const QString& );
};


//
#endif // GDBPROCESS_H
