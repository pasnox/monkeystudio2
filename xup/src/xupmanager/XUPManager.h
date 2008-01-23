#ifndef XUPMANAGER_H
#define XUPMANAGER_H

#include <QtXml>

class ProjectItem;

namespace XUPManager
{
	bool loadXUP( ProjectItem*, const QByteArray&, const QString& = QString( "1.0.0" ) );
	bool loadXUP( ProjectItem*, const QString&, const QString& = QString( "1.0.0" ) );
	bool saveXUP( ProjectItem*, const QString&, const QString& = QString( "1.0.0" ) );
	void parseRoot( ProjectItem*, QDomElement );
};

#endif // XUPMANAGER_H
