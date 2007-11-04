/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeParser.h
** Date      : 2007-11-04T22:54:25
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef QMAKEPARSER_H
#define QMAKEPARSER_H

#include <QString>

class QMakeItem;

class QMakeParser
{
public:
	QMakeParser( const QString&, QMakeItem* );
	~QMakeParser();
	
	bool isOpen() const;
	
protected:
	bool loadFile( const QString&, QMakeItem* );
	int parseBuffer( int, QMakeItem* );
	
	QMakeItem* processNested( const QString&, QMakeItem* );
	QMakeItem* processValues( const QString&, QMakeItem* );
	QMakeItem* addScope( const QString&, const QString&, bool, QMakeItem* );
	QMakeItem* addVariable( const QString&, const QString&, QMakeItem* );
	QMakeItem* addFunction( const QString&, const QString&, QMakeItem* );
	QMakeItem* addValue( const QString&, QMakeItem* );
	QMakeItem* addComment( const QString&, QMakeItem* );
	QMakeItem* addEmpty( QMakeItem* );
	
	bool mIsOpen;
	QMakeItem* mRoot;

};

#endif // QMAKEPARSER_H
