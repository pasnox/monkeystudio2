/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com ), Kopats Andrei ( hlamer@tut.by )
 * FILENAME     : pAction.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to create action that can be manage by the shortcut manager
 ********************************************************************************************************/
#ifndef PACTION_H
#define PACTION_H

#include "MonkeyExport.h"

#include <QAction>

#include "pActionManager.h"

class Q_MONKEY_EXPORT pAction : public QAction
{
	Q_OBJECT

public:
	pAction( const QString&, const QString&, const QKeySequence&, const QString& = pActionManager::globalGroup() );
	pAction( const QString&, const QIcon&, const QString&, const QKeySequence&, const QString& = pActionManager::globalGroup() );

	static QString fixedString( const QString& );

	static const char* _DEFAULT_SHORTCUT_PROPERTY_;
	static const char* _GROUP_PROPERTY_;
	static const char* _SETTINGS_SCOPE_;

};

#endif // PACTION_H
