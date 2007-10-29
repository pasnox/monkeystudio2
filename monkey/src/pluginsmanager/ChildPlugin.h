#ifndef CHILDPLUGIN_H
#define CHILDPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"

#include <QHash>
#include <QStringList>
#include <QDir>

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
	virtual bool openFile( const QString&, const QPoint& = QPoint() ) = 0;
	
	
protected:
	QHash<QString, QStringList> mSuffixes;
	
};

Q_DECLARE_INTERFACE( ChildPlugin, "org.monkeystudio.MonkeyStudio.ChildPlugin/1.0" )

#endif // CHILDPLUGIN_H
