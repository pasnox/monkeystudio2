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

//! Restore line if it is splited in more lines

/**
\author xiantia
\version 1.3.2

This class can restore line if it is splited in more lines

\code
Breakpoint 2, 0x3ed4a5 at
	main.cpp,line 12
(gdb)
\endcode

For restore this line just add a new configuration by

\code
GdbRestoreLine::add(yourClassName, QRegExp("^Breakpoint\\s+\\d+,\\s+.*"), QRegExp("*.,\\s+line\\s+\\d+"));
\endcode


Now if this line is found, GdbRestoreLine can restore it and you have now

\code
Breakpoint 2, 0x3ed4a5 at main.cpp,line 12
(gdb)
\endcode
*/

class GdbRestoreLine : public QObject
{

	//! Struct for store data
	typedef struct GdbLines
	{
		//! class name
		QString className;
		//! Your line must starts with ...
		QRegExp l1;
		//! Your line must ends with
		QRegExp l2;
	};


public:

	GdbRestoreLine(QObject *parent = 0);
	~GdbRestoreLine();
	
	//! Add new restoring
	void add(const QString & ,const QRegExp &, const QRegExp &);
	//! Add new restoring
	void add(const QString &, const QString &, const QString &);
	//! Try restoe line in string block
	bool tryRestore(const QString &, QStringList * );

	//! Get the first line
	int begin(/*const QString &,*/ const int &, const QStringList & , const QRegExp &);
	//! Get the last line
	int end(/*const QString &, */const int & , const QStringList &, const QRegExp &);

private:

	//! List of all restore line
	QList<GdbLines> GdbRestoreLineList;

};

#endif
