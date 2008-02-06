

/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 dcembre 2007 - 18h04
 * AUTHOR       :  (  )
 * FILENAME     : gdbBaseParser
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/

#ifndef GdbParser_H
#define GdbParser_H

#include <QObject>
#include <QString>

#include <QStringList>
#include <QRegExp>
#include <QFile>




class GdbParser : public QObject
{
	Q_OBJECT

	struct base_list
	{
		QList<QRegExp> pattern;
		QList<int> id;
	};

	struct base_interpreter
	{
		QList<QString> name;
		QList<QRegExp> cmd;
		QList<QRegExp> regexp;
		QList<QString> answer;
		QList<int> id;
	};

public:
	GdbParser (QObject* p);
	~GdbParser(){};


public slots:

	bool processParsing(const QByteArray&);
	void setLastCommand(QString);

	int addInterpreter(QString,QString, QRegExp , QRegExp , QString);
	int removeInterpreter(int id);
	void addRestorLine(QRegExp, QRegExp);


private :
	// var
	QString crlf;
	QString waitingCmd;
	QString currentCmd;

	QString gdbBuffer;
	QFile file;
	
	typedef base_list QBaseList;
	QBaseList list;
	
	typedef base_interpreter QBaseInterpreter;
	QBaseInterpreter interpreter;

	// parser tools
	bool gotoSection(QString section);
	int getId(QString line);
	QString getRexp(QString line);
	bool loadingPattern(QString fileName);
	int check(QString error);
	int checkCmd(QString cmd);
	int checkInterpreter(QString oneLine);

	QList<QRegExp> restoringList;
	bool checkRestoring(QString , QString);
	void restorLine(QStringList *list);

	bool checkEndParsing(const QString data);

	// base parser
	void onDone(int , QString);
	void onError(int , QString);
	void onInfo(int , QString);


 signals:

	void targetLoaded(int , QString);
	void targetExited(int , QString);
	void targetStopped(int , QString);
	void targetRunning(int , QString);

	void done(int , QString);
	void error(int , QString);
	void info(int, QString);
};


#endif 
