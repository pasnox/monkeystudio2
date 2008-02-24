#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPItem.h"

class QMakeProjectItem : public XUPItem
{
	Q_OBJECT
	
public:
	QMakeProjectItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false );

	virtual void registerItem();
	virtual QMakeProjectItem* clone( bool = true ) const;
	virtual QString interpretedVariable( const QString&, const XUPItem* = 0, const QString& = QString() ) const;
	virtual QString defaultInterpretedValue() const;
	virtual void checkChildrenProjects();
	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual QString filePath( const QString& = QString() );
	virtual QStringList splitFiles( const QString& ) const;
};

#endif // QMAKEPROJECTITEM_H
