#ifndef QMAKE2XUP_H
#define QMAKE2XUP_H

#include <QDomDocument>

namespace QMake2XUP
{
	QString convertFromPro( const QString&, const QString& = QString( "1.0.0" ) );
	QString convertToPro( const QDomDocument&, const QString& = QString( "1.0.0" ) );
};

#endif // QMAKE2XUP_H
