/********************************************************************************************************
	* PROGRAM      : Debugger (PARSER)
	* DATE - TIME  : mardi 01 janvier 2008 - 18h48
	* AUTHOR       :  (  )
	* FILENAME     : GdbRestoreLine
	* LICENSE      : 
	* COMMENTARY   : 
	********************************************************************************************************/

#ifndef GDBRESTORELINE_H
#define GDBRESTORELINE_H



#include <QObject>
#include <QRegExp>
#include <QStringList>


class GdbRestoreLine : public QObject
{

	typedef struct GdbLines
	{
		QString className;
		QRegExp l1;
		QRegExp l2;
	};


public:

	GdbRestoreLine(QObject *parent = 0);
	~GdbRestoreLine();
	
	void add(const QString & ,const QRegExp &, const QRegExp &);
	void add(const QString &, const QString &, const QString &);
	bool tryRestore(const QString &, QStringList * );
//	bool find(const QString &, const QString &);

	int begin(/*const QString &,*/ const int &, const QStringList & , const QRegExp &);
	int end(/*const QString &, */const int & , const QStringList &, const QRegExp &);

private:

	QList<GdbLines> GdbRestoreLineList;

};

#endif
