#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "ProjectItem.h"

class QMakeProjectItem : public ProjectItem
{
	Q_OBJECT
	
public:
	QMakeProjectItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false, ProjectItem* = 0 );

	static void registerItem();
	virtual QStringList filteredVariables() const;
	virtual QMakeProjectItem* clone( bool = true ) const;
	virtual QString interpretedVariable( const QString&, const ProjectItem* = 0, const QString& = QString() ) const;
	virtual QString defaultInterpretedValue() const;
	virtual void checkChildrenProjects();
	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual QString filePath( const QString& = QString() );
	virtual QStringList splitFiles( const QString& ) const;
};

#endif // QMAKEPROJECTITEM_H
