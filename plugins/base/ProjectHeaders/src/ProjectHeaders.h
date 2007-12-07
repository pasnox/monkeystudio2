/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : ProjectHeaders
** FileName  : ProjectHeaders.h
** Date      : 2007-11-04T01:45:50
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PROJECTHEADERS_H
#define PROJECTHEADERS_H

#include "BasePlugin.h"

class ProjectHeaders : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	ProjectHeaders();
	virtual ~ProjectHeaders();
	virtual bool setEnabled( bool );

	void setTemplatesHeaderRegExp( const QString&, const QString& );
	QString templatesHeaderRegExp( const QString& ) const;
	QString defaultTemplatesHeaderRegExp( const QString& ) const;

	void setTemplatesHeader( const QString&, const QString& );
	QString templatesHeader( const QString& ) const;
	QString defaultTemplatesHeader( const QString& ) const;

protected slots:
	void processLicensing();

};

#endif // PROJECTHEADERS_H
