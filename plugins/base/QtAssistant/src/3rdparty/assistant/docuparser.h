/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef DOCUPARSER_H
#define DOCUPARSER_H

#include <QtXml>
#include <QList>
#include <QMap>

class Profile;

struct ContentItem {
    ContentItem()
	: title( QString() ), reference( QString() ), depth( 0 ) {}
    ContentItem( const QString &t, const QString &r, int d )
	: title( t ), reference( r ), depth( d ) {}
    QString title;
    QString reference;
    int depth;
    Q_DUMMY_COMPARISON_OPERATOR(ContentItem)
};

QDataStream &operator>>( QDataStream &s, ContentItem &ci );
QDataStream &operator<<( QDataStream &s, const ContentItem &ci );

struct IndexItem {
    IndexItem( const QString &k, const QString &r )
	: keyword( k ), reference( r ) {}
    QString keyword;
    QString reference;
};



class DocuParser : public QXmlDefaultHandler
{
public:
    enum ParserVersion { Qt310, Qt320 };
    // Since We don't want problems with documentation
    // from version to version, this string stores the correct
    // version string to save documents.
    static const QString DocumentKey;

    static DocuParser *createParser( const QString &fileName );

    virtual bool parse( QFile *file );
    
    QList<ContentItem> getContentItems();
    QList<IndexItem*> getIndexItems();

    QString errorProtocol() const;
    QString contentsURL() const { return conURL; }

    virtual ParserVersion parserVersion() const = 0;
    virtual void addTo( Profile *p ) = 0;

    QString fileName() const { return fname; }
    void setFileName( const QString &file ) { fname = file; }

protected:
    QString absolutify( const QString &input, bool makeUrl = true ) const;
    
    QString contentRef, indexRef, errorProt, conURL;
    QString docTitle, title, iconName;
    QList<ContentItem> contentList;
    QList<IndexItem*> indexList;
    QString fname;
};


class DocuParser310 : public DocuParser
{
public:
    enum States{ StateInit, StateContent, StateSect, StateKeyword };
    
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString & );
    bool fatalError( const QXmlParseException& exception );

    virtual ParserVersion parserVersion() const { return Qt310; }
    virtual void addTo( Profile *p );
    
private:
    States state;
    int depth;
};


class DocuParser320 : public DocuParser
{
public:
    enum States { StateInit, StateDocRoot, StateProfile, StateProperty,
		  StateContent, StateSect, StateKeyword };

    DocuParser320();    
    
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString & );
    bool fatalError( const QXmlParseException& exception );

    virtual ParserVersion parserVersion() const { return Qt320; }
    virtual void addTo( Profile *p );
    Profile *profile() const { return prof; }    

private:
    
    States state;
    int depth;
    int docfileCounter;
    QString propertyValue;
    QString propertyName;
    Profile *prof;
};

#endif // DOCUPARSER_H
