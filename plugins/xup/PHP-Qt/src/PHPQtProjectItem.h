#ifndef PHPQTPROJECTITEM_H
#define PHPQTPROJECTITEM_H

#include "XUPProjectItem.h"

class PHPQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	enum ProjectType { PHPQtProject = 2 };

	PHPQtProjectItem();
	virtual ~PHPQtProjectItem();

	inline virtual int projectType() const { return PHPQtProjectItem::PHPQtProject; }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newProject() const { return new PHPQtProjectItem(); }

	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual CompilerPlugin* compiler( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;
	virtual void installCommands();
};

#endif // PHPQTPROJECTITEM_H
