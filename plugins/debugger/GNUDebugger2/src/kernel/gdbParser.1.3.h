/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 dcembre 2007 - 18h04
 * AUTHOR       :  (  )
 * FILENAME     : gdbParser
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/

#ifndef GDBPARSER_H
#define GDBPARSER_H

#include "fresh.h"

#include <QObject>
#include <QString>

#include <QStringList>
#include <QRegExp>
#include <QFile>

#include "gdbInterpreter.1.3.h"
#include "gdbPatternFile.1.3.h"
#include "gdbRestoreLine.1.3.h"



class GdbParser : public QObject, public QSingleton<GdbParser>
{

	Q_OBJECT
	friend class QSingleton<GdbParser>;

public:

	GdbParser (QObject * parent = 0);
	~GdbParser();

public slots:

	bool processParsing(const QByteArray&);
	void setNextCommand(QString);
	void clearAllCommand();

	// gateAway restoreLine
	void addRestoreLine(const QString &, const QString &);

	// gatAway interpreter
	QPointer<BaseInterpreter> addInterpreter(const QString & cName, const QString & cGdb, const QRegExp & cRegExp,
		const QRegExp & aRegExp, const QString & aExtention);

	bool removeInterpreter( const QPointer<BaseInterpreter> & );
	void changeAnswerInterpreter(const QPointer<BaseInterpreter> &, const QString &);

	bool isReady() { return mIsReady;}

private :

	QPointer<GdbInterpreter> gdbInterpreter;
	QPointer<GdbPatternFile> gdbPatternFile;
	QPointer<GdbRestoreLine> gdbRestoreLine;

	QList<QRegExp> mEndParsing;

	QStringList mCmdList;
	QString mCurrentCmd;

	QString gdbBuffer;
	
	bool checkEndParsing(const QString data);

	// var
	QString crlf;

	// base parser
	void onDone(int , QString);
	void onError(int , QString);
	void onInfo(int , QString);

	void getCommand();
	void switchCommand(const QString &);

	bool mIsReady;

 signals:

	void onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &);

	void targetLoaded(int , QString);
	void targetNoLoaded(int, QString);

	void targetExited(int , QString);
	void targetStopped(int , QString);
	void targetRunning(int , QString);
	
	void done(int , QString);
	void error(int , QString);
	void info(int, QString);

	void parserReady();
};


#endif 
