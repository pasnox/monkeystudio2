/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pTemplatesManager.h
** Date      : 2008-01-14T00:37:13
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#ifndef PTEMPLATESMANAGER_H
#define PTEMPLATESMANAGER_H

#include <fresh.h>

#include "../variablesmanager/VariablesManager.h"

#include <QApplication>
#include <QStringList>

class XUPItem;

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

	pTemplate getTemplate( const QString& );
	TemplateList getTemplates();

	bool realiseTemplate( XUPItem*, const QString&, const pTemplate&, const VariablesManager::Dictionary& = VariablesManager::Dictionary() );
    
};

#endif // PTEMPLATESMANAGER_H
