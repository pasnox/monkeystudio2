/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICtags2Api
** FileName  : UICtags2Api.h
** Date      : 2007-10-23T21:26:13
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UICTAGS2API_H
#define UICTAGS2API_H

#include "ui_UICtags2Api.h"

#include <QHash>
#include <QByteArray>

struct CtagsEntity
{
	CtagsEntity( const QString& s ) { mList = s.split( '\t' ); }
	
	QString getName() const { return mList.value( 0 ).trimmed(); }
	QString getFile() const { return mList.value( 1 ).trimmed(); }
	QString getAddress() const { return mList.value( 2 ).trimmed(); }
	QString getFieldValue( const QString& s ) const
	{
		// field are new format, and old is only 3 part separated by '\t'
		if ( mList.count() == 3 || s.isEmpty() )
			return QString();
		
		for ( int i = 3; i < mList.count(); i++ )
		{
			QString f = mList.at( i );
			// special case kind
			if ( !f.contains( ':' ) && s == "kind" )
				return f.trimmed();
			// special case file
			if ( f == "file:" )
				return getFile();
			// generic way
			QStringList l = f.split( ':' );
			if ( l.at( 0 ) == s )
				return l.value( 1 ).trimmed().replace( "\\t", "\t" ).replace( "\\r", "\r" ).replace( "\\n", "\n" ).replace( "\\\\", "\\" );
		}
		return QString();
	}
	QString getKindValue() const { return getFieldValue( "kind" ); }
	QString getKindDefaultField() const
	{
		// get kind
		const QString s = getKindValue();
		// if empty return null string
		if ( s.isEmpty() )
			return QString();
		// get value
		if ( s == "c" )
			return "class";
		else if ( s == "d" )
			return QString();
		else if ( s == "e" )
			return "enum";
		else if ( s == "f" )
			return "function";
		else if ( s == "F" )
			return "file";
		else if ( s == "g" )
			return QString();
		else if ( s == "m" )
		{
			if ( !getFieldValue( "class" ).isEmpty() )
				return "class";
			else if ( !getFieldValue( "struct" ).isEmpty() )
				return "struct";
			return QString();
		}
		else if ( s == "p" )
			return QString();
		else if ( s == "s" )
			return "struct";
		else if ( s == "t" )
			return "typeref";
		else if ( s == "u" )
			return "union";
		else if ( s == "v" )
			return QString();
		return QString();
	}
	QString getKindDefaultValue() const { return getFieldValue( getKindDefaultField() ); }
	
	QStringList mList;
};

class UICtags2Api : public QDialog, public Ui::UICtags2Api
{
	Q_OBJECT

public:
	UICtags2Api( QWidget* = 0 );
	~UICtags2Api();

	QList<QByteArray> getFileContent( const QString& );

protected:
	QHash<QString, QList<QByteArray> > mFileCache;

protected slots:
	void on_tbCtagsBinary_clicked();
	void on_cbGenerateFrom_currentIndexChanged( int );
	void on_tbBrowse_clicked();
	void on_tbSrcPathBrowse_clicked();
	bool processCtagsBuffer( const QByteArray& );
	bool processCtags( const QString& );
	bool processCtags2Api( const QString& );
	void accept();

};

#endif // UICTAGS2API_H
