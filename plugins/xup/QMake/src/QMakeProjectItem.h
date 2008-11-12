#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	
public:
	enum ProjectType { QMakeProject = 1 };
	
	QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
	inline virtual int projectType() const { return QMakeProjectItem::QMakeProject; }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newProject() const { return new QMakeProjectItem(); }
	virtual void customRowCount( XUPItem* item ) const;
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	
	virtual QString interpretVariable( const QString& variableName, const XUPItem* callerItem = 0, const QString& defaultValue = QString::null ) const;
	virtual QString interpretValue( XUPItem* callerItem, const QString& attribute ) const;
};

#endif // QMAKEPROJECTITEM_H
