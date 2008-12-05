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
****************************************************************************/
/*!
	\file main.cpp
	\date 2008-01-14T00:37:22
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief The main file of Monkey Studio IDE application
*/
#include <QApplication>
#include <QMessageBox>

#include "coremanager/MonkeyCore.h"
#include "pluginsmanager/PluginsManager.h"
#include "settingsmanager/Settings.h"

int main( int argc, char** argv )
{
	// check qt version
	QT_REQUIRE_VERSION( argc, argv, "4.4.0" );
	// create application
	QApplication a( argc, argv );
	a.setApplicationName( PACKAGE_NAME );
	a.setOrganizationName( PACKAGE_NAME );
	a.setOrganizationDomain( PACKAGE_DOMAIN );
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
#ifdef Q_OS_MAC
	// qt plugins path for mac os x
	QStringList pluginsPaths = QStringList() << QApplication::applicationDirPath().append( "/../plugins/qt" )
		<< QApplication::libraryPaths();
	QApplication::setLibraryPaths( pluginsPaths );
#endif
	// init monkey studio core
	MonkeyCore::init();
	// execute application
	int result = a.exec();
	// some cleanup
	MonkeyCore::pluginsManager()->clearPlugins();
	delete MonkeyCore::settings();
	// return app exit code
	return result;
}
