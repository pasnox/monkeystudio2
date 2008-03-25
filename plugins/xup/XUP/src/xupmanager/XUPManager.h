#ifndef XUPMANAGER_H
#define XUPMANAGER_H

#include <QtXml>

class XUPItem;

namespace XUPManager
{
	bool loadXUP( XUPItem*, const QByteArray&, const QString& = QString( "1.0.0" ) );
	bool loadXUP( XUPItem*, const QString&, const QString& = QString( "1.0.0" ) );
	bool saveXUP( XUPItem*, const QString&, const QString& = QString( "1.0.0" ) );
	void parseRoot( XUPItem*, QDomElement );
};

#endif // XUPMANAGER_H
