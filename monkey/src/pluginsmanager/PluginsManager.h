#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "ChildPlugin.h"
#include "ProjectPlugin.h"
#include "BuilderPlugin.h"
#include "CompilerPlugin.h"
#include "DebuggerPlugin.h"
#include "InterpreterPlugin.h"

#include <QApplication>

class ProjectItem;
class pAbstractChild;

class Q_MONKEY_EXPORT PluginsManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;
	
public:
	enum StateType { stAll = -1, stDisabled, stEnabled };

	void loadsPlugins();
	
	QList<BasePlugin*> plugins() const;
	template <class T>
	QList<T> plugins( PluginsManager::StateType t, const QString& n = QString::null, const QString& v = QString::null )
	{
		// temporary list
		QList<T> l;
		// for each plugin
		foreach ( BasePlugin* bp, mPlugins )
			// plugin state
			if ( t == stAll || ( !bp->isEnabled() && t == stDisabled ) || ( bp->isEnabled() && t == stEnabled ) )
				// empty or good name
				if ( n.isEmpty() || bp->infos().Name == n )
					// no version or good version
					if ( v.isEmpty() || bp->infos().Version == v )
						// good cast
						if ( T p = qobject_cast<T>( bp ) )
							l << p;
		// return list
		return l;
	}
	template <class T>
	T plugin( PluginsManager::StateType t, const QString& n = QString::null,  const QString& v = QString::null )
	{ return plugins<T>( t, n, v ).value( 0 ); }
	
	pAbstractChild* openChildFile( const QString& );
	QHash<QString, QStringList> childSuffixes() const;
	QString childFilters() const;
	
	ProjectItem* projectItem( const QString& );
	
	void setCurrentBuilder( BuilderPlugin* );
	BuilderPlugin* currentBuilder();
	
	void setCurrentCompiler( CompilerPlugin* );
	CompilerPlugin* currentCompiler();
	
	void setCurrentDebugger( DebuggerPlugin* );
	DebuggerPlugin* currentDebugger();
	
	void setCurrentInterpreter( InterpreterPlugin* );
	InterpreterPlugin* currentInterpreter();
	
protected:
	QList<BasePlugin*> mPlugins;
	BuilderPlugin* mBuilder;
	CompilerPlugin* mCompiler;
	DebuggerPlugin* mDebugger;
	InterpreterPlugin* mInterpreter;

	PluginsManager( QObject* = QApplication::instance() );
	bool addPlugin( QObject* );
	void enableUserPlugins();
	
public slots:
	void manageRequested();
};

#endif // PLUGINSMANAGER_H
