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


typedef struct GdbPattern
{
	QString comment;
	QRegExp key;
	int id;
	bool enable;
	bool show;
};


class GdbPatternFile : public QObject, public QSingleton<GdbPatternFile>
{

	Q_OBJECT
	friend class QSingleton<GdbPatternFile>;

public:

	GdbPatternFile(QObject *parent = 0);
	~GdbPatternFile();
	bool load(const QString & );
	GdbPattern find(const QString &);

	QList<GdbPattern> * get(){ return &GdbPatternList;}

	int getId(const GdbPattern & p);
	QString getPattern(const GdbPattern & p );
	QString getComment( const GdbPattern & p);

	QString getFileName() { return mFullName; }

private :


	QList<GdbPattern> GdbPatternList;
	QString mFullName;
};
	 
#endif
