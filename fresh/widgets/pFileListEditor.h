/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pFileListEditor.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PFILELISTEDITOR_H
#define PFILELISTEDITOR_H

#include "MonkeyExport.h"
#include "pStringListEditor.h"

class Q_MONKEY_EXPORT pFileListEditor : public pStringListEditor
{
    Q_OBJECT
	
public:
	pFileListEditor( QWidget* = 0, const QString& = QString(), const QString& = QString(), const QString& = QString() );
	void setPath( const QString& );

protected slots:
	virtual void onAddItem();
	virtual void onEditItem();

protected:
	QString mPath;
	QString mFilter;
	
};

#endif // PFILELISTEDITOR_H
