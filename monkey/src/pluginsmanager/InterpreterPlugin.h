/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : InterpreterPlugin.h
** Date      : 2008-01-14T00:37:01
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
#ifndef INTERPRETERPLUGIN_H
#define INTERPRETERPLUGIN_H

#include "CLIToolPlugin.h"
#include "ui/UIInterpreterSettings.h"

class Q_MONKEY_EXPORT InterpreterPlugin : public BasePlugin, public CLIToolPlugin
{
public:
	InterpreterPlugin() : BasePlugin(), CLIToolPlugin( this ) {}
	virtual pCommand defaultInterpretCommand() const = 0;
	virtual pCommand interpretCommand() const = 0;
	virtual void setInterpretCommand( const pCommand& ) = 0;
	virtual QWidget* interpreterSettingsWidget() { return new UIInterpreterSettings( this, QApplication::activeWindow() ); }
};

Q_DECLARE_INTERFACE( InterpreterPlugin, "org.monkeystudio.MonkeyStudio.InterpreterPlugin/1.0" )

#endif // INTERPRETERPLUGIN_H
