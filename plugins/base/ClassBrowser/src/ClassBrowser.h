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
#ifndef CLASSBROWSER_H
#define CLASSBROWSER_H

#include <BasePlugin.h>

#include <QPointer>

class pDockClassBrowser;
class pAbstractChild;
class XUPProjectItem;
class qCtagsSenseEntry;

class ClassBrowser : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	ClassBrowser();
	virtual ~ClassBrowser();
	virtual bool setEnabled( bool );
	virtual QPixmap pixmap() const;

protected:
	QPointer<pDockClassBrowser> mDock;

protected slots:
	void fileOpened( const QString& fileName );
	void currentFileChanged( pAbstractChild* child, const QString& fileName );
	void opened( XUPProjectItem* project );
	void buffersChanged( const QMap<QString, QString>& entries );
	void memberActivated( qCtagsSenseEntry* entry );
};

#endif // CLASSBROWSER_H
