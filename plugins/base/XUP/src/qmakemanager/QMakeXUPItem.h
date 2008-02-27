#ifndef QMAKEXUPITEM_H
#define QMAKEXUPITEM_H

#include "XUPItem.h"

class QMakeXUPItem : public XUPItem
{
	Q_OBJECT
	
public:
	QMakeXUPItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false );

	virtual QHash<QString, QStringList> variableSuffixes() const;
	virtual void registerVariableSuffixes( const QString& varname, const QStringList& suffixes );
	virtual QIcon getIcon( const QString&, const QString& ) const;
	virtual void registerItem();
	virtual QMakeXUPItem* clone( bool = true ) const;
	virtual QString interpretedVariable( const QString&, const XUPItem* = 0, const QString& = QString() ) const;
	virtual QString defaultInterpretedValue() const;
	virtual void checkChildrenProjects();
	virtual bool isProjectContainer() const;

	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0, const QString& op = QString( "=" ) );
	virtual void removeFiles( const QStringList& files, XUPItem* scope= 0, const QString& op = QString( "=" ) );

	virtual QString filePath( const QString& = QString() );
	virtual QStringList splitFiles( const QString& ) const;

protected:
	QHash<QString, QStringList> mVariableSuffixes;
};

#endif // QMAKEXUPITEM_H
