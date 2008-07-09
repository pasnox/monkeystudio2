/****************************************************************************
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
/**
	* \file pSettings.h
	* \date 2008-01-14T00:27:38
	* \brief A class that handle ini files
	* \details This class is used for storing/retreiving datas using ini files
	* \author Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
	*/
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
	/**
		* \brief Construct a new pSettings
		* \param parent The object parent
		* \param name The settings name
		* \param version The settings version.
		*/
	pSettings( QObject* parent = 0, const QString& name  = mProgramName, const QString& version = mProgramVersion );
	/**
		* \brief Initialize the default settings name and version
		* \param name The settings name
		* \param version The settings version
		*/
	static void setIniInformations( const QString& name = PACKAGE_NAME, const QString& version = PACKAGE_VERSION );
	/**
		* \return Return the default name
		*/
	static QString programName();
	/**
		* \return Return the default version
		*/
	static QString programVersion();
	/**
		* \brief Return a filePath for storing the ini file according to it's parameters
		* \param name The settings name
		* \param name The settings version
		* \return 
		*/
	static QString getIniFile( const QString& name, const QString& version );

	/**
		* \brief Restore a main winow state
		* \param window The main window to restore
		*/
	virtual void restoreState( QMainWindow* window );
	/**
		* \brief Save a main winow state
		* \param window The main window to save
		*/
	virtual void saveState( QMainWindow* );
	/**
		* \brief A virtual member that you can reimplement for creating a default settings member
		*/
	virtual void setDefaultSettings() {};

protected:
	static QString mProgramName;
	static QString mProgramVersion;
};

#endif // PSETTINGS_H
