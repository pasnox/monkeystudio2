/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
/*!
	\file gdbPatternFile.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Load and store all patterns from gdbparing.txt file in list
*/


/*

	GdbPatternFile class

	Read and store all QRegExp from file to GdbPatternList


	Xiantia@gmail.com

	for Debugger v1.3.0
*/



#include "gdbPatternFile.h"

#include <QDataStream>
#include <QFile>

// debug
#include <QMessageBox>

/*!
	\details Create new object
	\param parent of this object
*/
GdbPatternFile::GdbPatternFile(QObject *parent) :  QObject(parent)
{
	GdbPatternList.clear();
}

/*!
	\details Save current pattern list in file. This list is composed of GdbPattern struct. 
*/
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


/*!
	\details Load QRegExp from file to GdbPatternList
	\param fullName is the path and name of file
	\retval true if the file is loaded correctly, else false
*/
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
//				p.show = l.at(2).toInt();
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


/*!
	\details Find value in GdbPatternList list
	\param value is the value that you search
	\retval GdbPattern struct contains the value or a empty GdbPattern if not found.  
*/
GdbPattern GdbPatternFile::find(const QString  & value)
{
	// init default pattern
	GdbPattern pattern={QString() , QRegExp(value),-1,false};
	
	// find of all pattern
	for(int i =0; i< GdbPatternList.count(); i++)
	{

		if((GdbPatternList.at(i).enable != 0) && GdbPatternList.at(i).key.exactMatch( value )) 
		{
			return GdbPatternList.at(i);
		}
	}
	// return no found pattern
	return pattern; 
}


/*!
	\details Get id of pattern.
	\param p is GdbPattern struct.
	\retval id of pattern.
*/
int GdbPatternFile::getId(const GdbPattern & p)
{
	return p.id;
}

/*!
	\details Get QRegExp of pattern
	\param p is GdbPattern struct.
	\retval string of pattern.
*/
QString GdbPatternFile::getPattern(const GdbPattern & p)
{
	return p.key.pattern();
}

/*!
	\details Get comments of pattern
	\param p is GdbPattern struct.
	\retval string of comment.
*/
QString GdbPatternFile::getComment( const GdbPattern & p)
{
	return p.comment;
}
