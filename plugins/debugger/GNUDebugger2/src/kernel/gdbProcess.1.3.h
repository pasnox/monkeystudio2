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


class  GdbProcess : public QProcess, public QSingleton<GdbProcess>
{

	Q_OBJECT
	friend class QSingleton<GdbProcess>;

public:

	GdbProcess( QObject *parent = 0 );
	~GdbProcess();

private:
	
	QString crlf;
	QString mCommand;
	QString mDir;
	QList<QString> mCmdList;
	QTimer t;

public slots:

	void sendRawData( const QString& );

	void stopProcess();
	void stopTarget();

	void setCommand( const QString & );
	void setWorkingDirectorie(const QString &);
	
	void startProcess();
	void clearAllCommand();

private slots:

	void readyRead();
	void onTimer();
	
signals:

	void commandReadyRead(  const QString& );
};


//
#endif // GDBPROCESS_H
