#ifndef XUPMANAGER_H
#define XUPMANAGER_H

#include <QtXml>
#include <QIcon>
#include <QHash>

class ProjectItem;

namespace XUPManager
{
	bool loadXUP( ProjectItem*, const QByteArray&, const QString& = QString( "1.0.0" ) );
	bool loadXUP( ProjectItem*, const QString&, const QString& = QString( "1.0.0" ) );
	bool saveXUP( ProjectItem*, const QString&, const QString& = QString( "1.0.0" ) );
	void parseRoot( ProjectItem*, QDomElement );
	
	void recursiveNodes( QDomNode&, ProjectItem* );
	QDomDocument toDomDocument( ProjectItem* );
	
	QStringList textTypes();
	void registerTextType( const QString& );
	
	QStringList fileVariables();
	void registerFileVariables( const QString& );
	
	QStringList pathVariables();
	void registerPathVariables( const QString& );
	
	QHash<QString, QString> variableLabels();
	void registerVariableLabels( const QString&, const QString& );
	
	QHash<QString, QIcon> variableIcons();
	void registerVariableIcons( const QString&, const QIcon& );
	
	QIcon getIcon( const QString&, const QString& );
	QString valueName( const QString& );
	void updateItem( ProjectItem* );
};

#endif // XUPMANAGER_H
