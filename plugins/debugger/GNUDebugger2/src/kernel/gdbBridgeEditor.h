/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBridgeEditor
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbInterpreter class

	Store all interpreter 


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBBRIDGEEDITOR_H
#define GDBBRIDGEEDITOR_H

#include <QObject>

#include <fresh.h>
#include <workspacemanager.h>

#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>


class GdbBridgeEditor : public QObject , public QSingleton<GdbBridgeEditor>
{

	Q_OBJECT
	friend class QSingleton<GdbBridgeEditor>;

	typedef struct Editor
	{
		QString fileName;
		pEditor * pointeur;
	};


public :
	GdbBridgeEditor(QObject * parent = 0);
	~GdbBridgeEditor();

public slots:

	void add(const QString &);
	void remove(const int &);
	void removeAllBreakpoints();
	void removeBacktrace();

	void fileOpenedBeforeDebugger();

	// slot from Qsi
	void onMarginClicked(int, int, Qt::KeyboardModifiers);

	void onToggleBreakpoint(const QString & fileName, const int & line, const bool & b);
	void onToggleBacktrace(const QString & filename, const int & line);

private :

	pEditor * findFile(const QString &);

	QList<Editor> editorList;

signals :

	void userToggleBreakpoint(const QString & , const int &);
	void requestBreakpoint(const QString &);
	void requestBacktrace(const QString &);
};
#endif
