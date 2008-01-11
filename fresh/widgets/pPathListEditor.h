/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pPathListEditor.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PPATHLISTEDITOR_H
#define PPATHLISTEDITOR_H

#include "MonkeyExport.h"
#include "pFileListEditor.h"

class Q_MONKEY_EXPORT pPathListEditor : public pFileListEditor
{
    Q_OBJECT
	
public:
	pPathListEditor( QWidget* = 0, const QString& = QString(), const QString& = QString() );

protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
	
};

#endif // PPATHLISTEDITOR_H
