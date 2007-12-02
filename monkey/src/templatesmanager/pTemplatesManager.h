/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.h
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#ifndef PTEMPLATESMANAGER_H
#define PTEMPLATESMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "VariablesManager.h"

#include <QApplication>
#include <QStringList>

class ProjectItem;

typedef QHash <QString, QStringList> VarList;

/*
	NOTE Filenames can contain variables. Example: Project$name$.pro
	This files will be renamed
*/
struct Q_MONKEY_EXPORT pTemplate
{
    QString Name;
    QString Language;
    QString Type;
    QString Description;
    QString Icon;
	QString Script;
    QString DirPath; // with '/' at end
    QStringList Files;
    QStringList FilesToOpen;
    QStringList ProjectsToOpen;
	QStringList FilesToAdd;
    VarList Variables;
	
	bool operator==( const pTemplate& t )
	{ return Name == t.Name && Language == t.Language && Type == t.Type &&
			Description == t.Description && Icon == t.Icon && Script == t.Script &&
			DirPath == t.DirPath && Files == t.Files && FilesToOpen == t.FilesToOpen &&
			ProjectsToOpen == t.ProjectsToOpen && FilesToAdd == t.FilesToAdd && Variables == t.Variables; }

};

typedef QList<pTemplate> TemplateList;

class Q_MONKEY_EXPORT pTemplatesManager : public QObject, public QSingleton<pTemplatesManager>
{
    Q_OBJECT
    friend class QSingleton<pTemplatesManager>;
	
private:
    pTemplatesManager( QObject* = QApplication::instance() );

public:
	void setTemplatesPath( const QStringList& );
	QStringList templatesPath() const;

	void setTemplatesHeader( const QString&, const QString& );
	QString templatesHeader( const QString& ) const;
	QString defaultTemplatesHeader( const QString& ) const;

	pTemplate getTemplate( const QString& );
	TemplateList getTemplates();

	bool realiseTemplate( ProjectItem*, const QString&, const pTemplate&, const VariablesManager::Dictionary& = VariablesManager::Dictionary() );
    
};

#endif // PTEMPLATESMANAGER_H
