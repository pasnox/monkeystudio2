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

	void readyRead();

signals:

	void commandReadyRead(  const QByteArray & );
};


//
#endif // GDBPROCESS_H
