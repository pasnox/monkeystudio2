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
#ifndef QMAKE_H
#define QMAKE_H

#include <XUPPlugin.h>

#include <QPointer>

class QMake : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	QMake();
	virtual ~QMake();
	
	virtual bool setEnabled( bool enabled );
	virtual QWidget* settingsWidget();
	virtual bool editProject( XUPProjectItem* project );

protected:
	QPointer<XUPProjectItem> mItem;
};

#endif // QMAKE_H
