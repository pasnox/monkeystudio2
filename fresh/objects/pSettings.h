/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pSettings.h
** Date      : 2008-01-14T00:27:38
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
#ifndef PSETTINGS_H
#define PSETTINGS_H

#include "MonkeyExport.h"

#include <QSettings>

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "My Application"
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "1.0.0"
#endif

class QMainWindow;

class Q_MONKEY_EXPORT pSettings : public QSettings
{
	Q_OBJECT

public:
	pSettings( QObject* = 0, const QString& = mProgramName, const QString& = mProgramVersion );
	
	static void setIniInformations( const QString& = PACKAGE_NAME, const QString& = PACKAGE_VERSION );
	static QString programName();
	static QString programVersion();
	static QString getIniFile( const QString& name, const QString& version );

	virtual void restoreState( QMainWindow* );
	virtual void saveState( QMainWindow* );
	virtual void setDefaultSettings() {};

protected:
	static QString mProgramName;
	static QString mProgramVersion;
};

#endif // PSETTINGS_H
