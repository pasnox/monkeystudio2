#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "ProjectItem.h"

class QMakeProjectItem : public ProjectItem
{
	Q_OBJECT
public:
	QMakeProjectItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false );

	static void registerItem();
	virtual QMakeProjectItem* clone() const;
	virtual void appendRow( QMakeProjectItem* );
	virtual void checkChildrenProjects();
	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );

};

#endif // QMAKEPROJECTITEM_H
