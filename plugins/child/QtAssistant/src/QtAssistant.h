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
#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#include <pluginsmanager.h>

#include <QPointer>

class QtAssistantDock;

class QtAssistant : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	
public:
	QtAssistant();
	virtual ~QtAssistant();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	virtual pAbstractChild* openFile( const QString& filename, const QPoint& pos = QPoint() );

protected:
	QPointer<QtAssistantDock> mAssistantDock;
};

#endif // QTASSISTANT_H
