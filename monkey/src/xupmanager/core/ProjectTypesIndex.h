#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QPair>
#include <QStringList>
#include <QMap>
#include <QHash>

typedef QPair<QString, QStringList> Pair_String_StringList;
typedef QList<Pair_String_StringList> Pair_String_StringList_List;

typedef QPair<QString, QString> Pair_String_String;
typedef QList<Pair_String_String> Pair_String_String_List;

class XUPProjectItem;

struct Q_MONKEY_EXPORT DocumentFilter
{
    enum Type {
        Standard = 0,
        Project,
        File,
        Path
    };
    
    DocumentFilter( const DocumentFilter::Type& type = DocumentFilter::Standard )
    {
        this->type = type;
        filtered = false;
        weight = -1;
    }
    
    Type type;
    bool filtered;
    int weight; // the weight is used for sorting items in model
    QString label;
    QString icon;
    QStringList filters;
};

class Q_MONKEY_EXPORT DocumentFilterMap : public QMap<QString, DocumentFilter> // filter name, filter
{
public:
    DocumentFilterMap( const QString& iconsPath = QString::null );
    virtual ~DocumentFilterMap();
    // return the display text of a project variable name. Used by XUPItem::displayText()
    virtual QString variableDisplayText( const QString& variableName ) const;
    // return the display icon name of a project variable name. Used by XUPItem::displayIcon()
    virtual QString variableDisplayIcon( const QString& variableName ) const;
    // return a splited value
    virtual QStringList splitValue( const QString& value ) const;
    // return variable that can handle fileName
    QString fileNameVariable( const QString& fileName ) const;
    // return a list of known variables name
    QStringList knownVariables() const;
    // return the default icons path
    QString defaultIconsPath() const;
    // return the document icons path
    QString iconsPath() const;
    // return a QFileDialog filter from the File filters
    QString sourceFileNameFilter() const;
    // return the filtered variables ( the ones visible in the project tree )
    QStringList filteredVariables() const;
    // return the file based variables
    QStringList fileVariables() const;
    // return the file based variables
    QStringList pathVariables() const;

protected:
    QString mIconsPath;
};

class Q_MONKEY_EXPORT ProjectTypesIndex : public QObject
{
    Q_OBJECT

public:
    ProjectTypesIndex( QObject* parent = 0 );
    
    // register the proejct type
    void registerType( const QString& projectType, const QMetaObject* projectMetaObject, const DocumentFilterMap& filters );
    
    // unregister the projecttype
    void unRegisterType( const QString& projectType );
    
    // return the registered filters for project type
    const DocumentFilterMap& documentFilters( const QString& projectType ) const;
    
    // check if filename matches to some project type
    bool fileIsAProject( const QString& fileName ) const;
    
    // return a valid project item for fileName
    XUPProjectItem* newProjectItem( const QString& fileName ) const;
    
    // return a project for project type type
    XUPProjectItem* newProjectItemByType( const QString& type ) const;
    
    // return a map that can be used to create file dialog filter
    QMap<QString, QStringList> suffixes() const;
    
protected:
    QHash<QString, const QMetaObject*> mRegisteredProjectItems; // project type, project item
    QHash<QString, const DocumentFilterMap*> mFilters; // project type, filters
};

#endif // XUPPROJECTITEMINFOS_H
