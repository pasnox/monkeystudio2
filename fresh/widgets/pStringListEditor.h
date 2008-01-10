/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pStringListEditor.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PSTRINGLISTEDITOR_H
#define PSTRINGLISTEDITOR_H

#include "MonkeyExport.h"

#include <QGroupBox>

class QListWidget;

class Q_MONKEY_EXPORT pStringListEditor : public QGroupBox
{
    Q_OBJECT
	
public:
	pStringListEditor( QWidget* = 0, const QString& = QString() );

	void setValues( const QStringList& );
	QStringList values() const;

protected:
    QListWidget* mList;

protected slots:
	virtual void onAddItem();
	virtual void onRemoveItem();
	virtual void onClearItem();
	virtual void onMoveUpItem();
	virtual void onMoveDownItem();
	virtual void onEditItem();

signals:
	void edited();

};

#endif // PSTRINGLISTEDITOR_H
