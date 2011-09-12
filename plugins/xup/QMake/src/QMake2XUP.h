#ifndef QMAKE2XUP_H
#define QMAKE2XUP_H

#include <QDomDocument>

#include <pMonkeyStudio.h>

namespace QMake2XUP
{
    QString convertFromPro( const QString& fileName, const QString& codec );
    QString convertToPro( const QDomDocument& project );
    
    QString escape( const QString& string );

    QString convertNodeToPro( const QDomNode& node, int weight = 0, bool multiline = false, bool nested = false, const QString& EOL = pMonkeyStudio::getEol() );
    QString tabbedString( int weight, const QString& string, const QString& eol = QString::null );
    QString nodeAttribute( const QDomNode& node, const QString& attribute, const QString& defaultValue = QString::null );
    bool isMultiline( const QDomNode& node );
    bool isNested( const QDomNode& node );
    bool isBlock( const QDomNode& node );
    bool isProject( const QDomNode& node );
    bool isComment( const QDomNode& node );
    bool isEmptyLine( const QDomNode& node );
    bool isVariable( const QDomNode& node );
    bool isValue( const QDomNode& node );
    bool isLastValue( const QDomNode& node );
};

#endif // QMAKE2XUP_H
