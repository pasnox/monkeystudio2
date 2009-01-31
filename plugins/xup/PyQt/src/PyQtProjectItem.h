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

	inline virtual int projectType() const { return PyQtProjectItem::PyQtProject; }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newProject() const { return new PyQtProjectItem(); }

	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual CompilerPlugin* compiler( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;
	virtual void installCommands();
};

#endif // PYQTPROJECTITEM_H
