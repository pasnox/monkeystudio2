/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbPatternFile
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/



#ifndef GDBPATTERNFILE_H
#define GDBPATTERNFILE_H


/* 

	read pattern file and store this to listPattern

*/
#include <fresh.h>

#include <QObject>
#include <QRegExp>

//! Struct for strore all informations of pattern
/**
\author xiantia
\version 1.3.2
*/
typedef struct GdbPattern
{
	//! Comment of pattern
	QString comment;
	//! String from Gdb in QRegExp format
	QRegExp key;
	//! If of string. 0 = prompt event, 10000 to 19999 is generique information, 20000 and more is an error found
	int id;
	//! pattern is enable. enable = 0 the pattern is not use, enable = 1 the pattern is not use but the message is show, enable = 2 the parttern is use 
	int enable;
};


//! Load and store all patterns from gdbparing.txt file in list

/**
\author xiantia
\version 1.3.2

Load file and store all information in list of GdbPattern struct.

*/

class GdbPatternFile : public QObject, public QSingleton<GdbPatternFile>
{

	Q_OBJECT
	friend class QSingleton<GdbPatternFile>;

public:

	GdbPatternFile(QObject *parent = 0);
	~GdbPatternFile();
	//! load file
	bool load(const QString & );
	//! Find if string is in file
	GdbPattern find(const QString &);

	//! get the list
	QList<GdbPattern> * get(){ return &GdbPatternList;}

	//! Get id of string if is found
	int getId(const GdbPattern & p);
	//! Get pattern of string if is found
	QString getPattern(const GdbPattern & p );
	//! Get comment of string if is found
	QString getComment( const GdbPattern & p);

	//! get path and name of file
	QString getFileName() { return mFullName; }

private :

	//! List of all patterns
	QList<GdbPattern> GdbPatternList;
	//! Current parse file use
	QString mFullName;
};
	 
#endif
