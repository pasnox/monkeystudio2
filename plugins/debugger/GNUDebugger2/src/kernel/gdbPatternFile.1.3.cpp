/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbPatternFile
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbPatternFile class

	Read and store all QRegExp from file to GdbPatternList


	Xiantia@gmail.com

	for Debugger v1.3.0
*/



#include "gdbPatternFile.1.3.h"

#include <QDataStream>
#include <QFile>

// debug
#include <QMessageBox>


GdbPatternFile::GdbPatternFile(QObject *parent) :  QObject(parent)
{
	GdbPatternList.clear();
}

GdbPatternFile::~GdbPatternFile()
{
	// save the pattern file
	if(QFile::exists(mFullName))
	{
		QFile f(mFullName);

		f.open(QIODevice::WriteOnly);
		QDataStream s(&f);

		foreach(GdbPattern p , GdbPatternList)
			s << QString(QString::number(p.enable) + "[" + QString::number(p.id) + "]" + p.key.pattern() + "[" + p.comment + "]"  );

		f.close();

	}
	GdbPatternList.clear();
}

//
// Load QRegExp to GdbPatterList
//
bool GdbPatternFile::load(const QString & fullName)
{
	// fullNme type /home/xiantia/dev/....
	mFullName = fullName;

	QFile file(fullName);
	// try open
	if(file.open(QIODevice::ReadOnly))
	{
		QDataStream ds(&file);
		QString s;
		
		while(!ds.atEnd())
		{
			// read line : [number id]string
			// example : [10002]Can't load library 
			// [10020]can not read memorie\.
			ds >> s;
			// match ?	
			QRegExp r("(\\d)\\[(\\d+)\\](.*)\\[(.*)\\]");
			if(r.exactMatch(s))
			{
				//extract id and string
				QStringList l = r.capturedTexts();
				GdbPattern p;
				p.id = l.at(2).toInt();
				p.key = QRegExp(l.at(3));
				p.comment = l.at(4);
				p.enable = l.at(1).toInt();
				// store 
				GdbPatternList << p;
			}
			// debug mode
//			else QMessageBox::warning(NULL,"Critical erreur in GdbPatternFile class", "No correctly formated " + s);
		}
		// close file
		file.close();
		return true;
	}
	return false;
}
//
// find value in GdbPatternList
//
GdbPattern GdbPatternFile::find(const QString  & value)
{
	// init default pattern
	GdbPattern pattern={QString() , QRegExp(value),-1,false};
	
	// find of all pattern
	for(int i =0; i< GdbPatternList.count(); i++)
	{

		if(GdbPatternList.at(i).enable && GdbPatternList.at(i).key.exactMatch( value )) 
		{
			return GdbPatternList.at(i);
		}
	}
	// return no found pattern
	return pattern; 
}
//
// get id of pattern
//
int GdbPatternFile::getId(const GdbPattern & p)
{
	return p.id;
}
//
// get pattern
//
QString GdbPatternFile::getPattern(const GdbPattern & p)
{
	return p.key.pattern();
}


QString GdbPatternFile::getComment( const GdbPattern & p)
{
	return p.comment;
}
