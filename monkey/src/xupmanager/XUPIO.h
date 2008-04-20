#ifndef XUPIO_H
#define XUPIO_H

#include <fresh.h>

#include <QDomElement>

class XUPItem;

namespace XUPIO
{
	Q_MONKEY_EXPORT bool parseXUP( XUPItem* project, const QString& buffer, const QString& version = QString( "1.0.0" ) );
	Q_MONKEY_EXPORT bool loadXUP( XUPItem* project, const QString& filename, const QString& version = QString( "1.0.0" ) );
	Q_MONKEY_EXPORT bool saveXUP( XUPItem* project, const QString& filename, const QString& version = QString( "1.0.0" ) );
	Q_MONKEY_EXPORT void parseRoot( XUPItem* item, const QDomElement& node );
};

#endif // XUPIO_H
