#ifndef QMAKEPARSER_H
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
