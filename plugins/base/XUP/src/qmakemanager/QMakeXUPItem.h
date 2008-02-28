#ifndef QMAKEXUPITEM_H
#define QMAKEXUPITEM_H

#include "XUPItem.h"

class QMakeXUPItem : public XUPItem
{
	Q_OBJECT
	
public:
	QMakeXUPItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false );

	virtual void registerItem();
	virtual QStringList operators() const;
	virtual void registerOperator( const QString& op );
	virtual QStringList filteredVariables() const;
	virtual void registerFilteredVariables( const QString& );
	virtual QStringList textTypes() const;
	virtual void registerTextType( const QString& );
	virtual QStringList fileVariables() const;
	virtual void registerFileVariables( const QString& );
	virtual QStringList pathVariables() const;
	virtual void registerPathVariables( const QString& );
	virtual QHash<QString, QStringList> suffixes() const;
	virtual void registerSuffixes( const QString& label, const QStringList& suffixes );
	virtual QHash<QString, QString> variableLabels() const;
	virtual void registerVariableLabels( const QString&, const QString& );
	virtual QHash<QString, QIcon> variableIcons() const;
	virtual void registerVariableIcons( const QString&, const QIcon& );
	virtual QHash<QString, QStringList> variableSuffixes() const;
	virtual void registerVariableSuffixes( const QString& varname, const QStringList& suffixes );
	virtual QIcon getIcon( const QString&, const QString& ) const;
	
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
	static XUPItemInfos mQMakeXUPItemInfos;
};

#endif // QMAKEXUPITEM_H
