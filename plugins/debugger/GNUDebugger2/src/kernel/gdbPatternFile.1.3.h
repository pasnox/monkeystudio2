/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbPatternFile
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/



#ifndef GDBPATTERNFILE_H
#define GDBPATTERNFILE_h


/* 

	read pattern file and store this to listPattern

*/

#include <QObject>
#include <QRegExp>


typedef struct GdbPattern
{
	QRegExp key;
	int id;
};


class GdbPatternFile : public QObject
{

	Q_OBJECT

public:

	GdbPatternFile(QObject *parent = 0);
	~GdbPatternFile();
	bool load(const QString & );
	GdbPattern find(const QString &);
	int getId(const GdbPattern & p);
	QString getPattern(const GdbPattern & p );


private :

	QList<GdbPattern> GdbPatternList;

};
	 
#endif
