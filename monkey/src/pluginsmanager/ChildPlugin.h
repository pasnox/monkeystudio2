/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : ChildPlugin.h
** Date      : 2007-11-04T22:45:29
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef CHILDPLUGIN_H
#define CHILDPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"

#include <QHash>
#include <QStringList>
#include <QDir>

class pAbstractChild;

class Q_MONKEY_EXPORT ChildPlugin : public BasePlugin
{
public:
	// the suffixes this project can manage
	virtual QHash<QString, QStringList> suffixes() const
	{ return mSuffixes; }
	// tell if this plugin can open this file
	virtual bool canOpen( const QString& f )
	{
		foreach ( const QString k, mSuffixes.keys() )
			foreach ( const QString s, mSuffixes[k] )
				if ( QDir::match( s, f ) )
					return true;
		return false;
	}
	// try opening this file
	virtual pAbstractChild* openFile( const QString&, const QPoint& = QPoint() ) = 0;
	
protected:
	QHash<QString, QStringList> mSuffixes;
	
};

Q_DECLARE_INTERFACE( ChildPlugin, "org.monkeystudio.MonkeyStudio.ChildPlugin/1.0" )

#endif // CHILDPLUGIN_H
