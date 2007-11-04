/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QtAssistant.h
** Date      : 2007-11-04T22:52:58
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#include "ChildPlugin.h"

class QtAssistant : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	
public:
	QtAssistant();
	virtual ~QtAssistant();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	virtual pAbstractChild* openFile( const QString&, const QPoint& = QPoint() ) { return 0; }
	
};

#endif // QTASSISTANT_H
