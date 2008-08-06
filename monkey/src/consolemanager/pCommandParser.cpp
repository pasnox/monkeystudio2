/****************************************************************************
**
**         Created using Monkey Studio v1.8.1.0
** Authors   : Andrei Kopats aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio IDE
** FileName  : pCommandParser.cpp
** Date      : 2008-01-14T00:36:50
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
/*!
	\file pCommandParser.h
	\date 2008-01-14T00:36:50
	\author Andrei Kopats
	\brief Implementation of pCommandParser class
*/
#include "pCommandParser.h"

/*!
	Change value of this macro, if you need to debug parsing or parsers, 
	and also check, how much time parsing takes
	A lot of debug info will be printed
*/
#define PARSERS_DEBUG 0

/*!
	Destructor of class
*/
pCommandParser::~pCommandParser()
{
}

#if PARSERS_DEBUG
 #include <QDebug>
 #include <QTime>
#endif

/*!
	Try to parse string buffer.
	
	If parsing will successfull, signals with results will be emited
	\param buf string buffer
	\return count of chars, which are recognised
	\retval 0 - String not recognised
*/
int pCommandParser::processParsing(QString* buf)
{
#if PARSERS_DEBUG
	static int allTime;
	static int total;
	QTime time1;
	time1.start();
#endif
	foreach ( Pattern p, patterns)
	{
		int pos = p.regExp.indexIn(*buf);
#if PARSERS_DEBUG
		qDebug () << "parser " << name();
		qDebug () << "parsing  " << *buf << "with" << p.regExp.pattern();
#endif
		if (pos != -1)
		{
			pConsoleManager::Step m;
			m.mFileName = replaceWithMatch(p.regExp,p.FileName);
			m.mPosition = QPoint( replaceWithMatch(p.regExp,p.col).toInt(), replaceWithMatch(p.regExp,p.row).toInt());
			m.mType = p.Type;
			m.mText = replaceWithMatch(p.regExp,p.Text);
			m.mFullText = replaceWithMatch(p.regExp,p.FullText);
			// emit signal
			emit newStepAvailable( m );
#if PARSERS_DEBUG
			qDebug () << "Capture :" << p.regExp.cap();
			qDebug () << "CaptureS :" << p.regExp.capturedTexts ();
			qDebug () << "Returning " << p.regExp.cap().count ('\n');
#endif
			return p.regExp.cap().count ('\n');
		}
#if PARSERS_DEBUG
			qDebug () << "Not matching";
#endif
	}
#if PARSERS_DEBUG
	allTime += time1.elapsed ();
	qDebug () << "All time" <<allTime;
	qDebug () << "Total count" <<total++;
	
	qDebug () << "Returning false";
#endif
	return 0;
}

/*!
	Get name of parser
	\return Name of parser
*/
QString pCommandParser::name() const
{
	return mName;
}

/*!
	Function replaces sequences "%d" (where d is number) with matches 
	from regular expression. 

	%0 mean wall match, %n - nth submatch.
	Function is using for generating results of parsing
	\param rex Already matched regular expression (searching of a pattern 
		should be called before this function)
	\param s String, which contains %d (where d is number) patterns.
		Patterns will be replaced with according submatch of string
	\return Resulting string
*/
QString pCommandParser::replaceWithMatch(QRegExp& rex, QString s)
{
	int pos = 0; 
	int i = 0;
	while ( (i = s.indexOf("%", pos)) != -1)
	{
	pos = i;
	if ( ! s[i+1].isDigit () )
		continue;
	QString cap = rex.cap(s[i+1].digitValue ());
	if (cap.endsWith ("\n"))
		cap.chop(1);
	s.replace (i,2,cap);
	pos += cap.size ();
	}
	return s;
}
