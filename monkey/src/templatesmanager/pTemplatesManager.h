/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.h
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#ifndef PTEMPLATESMANAGER_H
#define PTEMPLATESMANAGER_H

#include "MonkeyExport.h"

#include <QApplication>
#include <QStringList>
#include <QComboBox>

class UITemplatesWizard;
class pTemplatePreviewer;

struct Q_MONKEY_EXPORT pTemplate
{
	enum TemplateType { ttUnknow = 0, ttFiles, ttProjects, ttExtras, ttEnd };
	pTemplate() {}
	pTemplate( const QString& l, TemplateType t, const QString& n, const QString& d, const QString& i, const QStringList& f )
		: Language( l ), Type( t ), Name( n ), Description( d ), Icon( i ), FileNames( f ) {}

	static void fillComboBox( QComboBox* cb )
	{
		// if no cb, cancel
		if ( !cb )
			return;
		// clear combo
		cb->clear();
		// add entries
		for ( int i = ttUnknow +1; i < pTemplate::ttEnd; i++ )
			cb->addItem( stringForType( (pTemplate::TemplateType)i ), i );
	}

	static QString stringForType( pTemplate::TemplateType t )
	{
		switch( t )
		{
			case ttFiles:
				return QObject::tr( "Files" );
			case ttExtras:
				return QObject::tr( "Extras" );
			case ttProjects:
				return QObject::tr( "Projects" );
			default:
				return QString();
		}
	}

	QString Language;
	TemplateType Type;
	QString Name;
	QString Description;
	QString Icon;
	QStringList FileNames;
};

struct Q_MONKEY_EXPORT pTemplateContent
{
	QString Name;
	QString Author;
	QString License;
	QString Project;
	QString FileName;
	QString Comment;
	QString Content;
};

namespace pTemplatesManager
{
	const QList<pTemplate> defaultTemplates();
	const QList<pTemplate> availableTemplates();

	void setTemplatesPath( const QString& );
	const QString templatesPath();

	void setTemplatesHeader( const QString& );
	const QString templatesHeader();

	const QString tokenize( QString );
	const QString unTokenize( QString );

	const QStringList tokenize( QStringList );
	const QStringList unTokenize( QStringList );

	const QString processContent( pTemplateContent );
};

#endif // PTEMPLATESMANAGER_H
