#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include "XUPProjectItem.h"

class PyQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	enum ProjectType { PyQtProject = 3 };

	PyQtProjectItem();
	virtual ~PyQtProjectItem();

	virtual int projectType() const;
	virtual void registerProjectType() const;
	virtual XUPProjectItem* newProject() const;
	
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;
	virtual void installCommands();
};

#endif // PYQTPROJECTITEM_H
