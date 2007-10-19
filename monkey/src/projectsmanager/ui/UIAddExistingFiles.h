/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIAddExistingFiles
** FileName  : UIAddExistingFiles.h
** Date      : 2007-10-13T20:00:33
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIADDEXISTINGFILES_H
#define UIADDEXISTINGFILES_H

#include <QFileDialog>
#include <QGroupBox>

#include "ProjectsModel.h"
#include "ProjectsProxy.h"
#include "ProjectItem.h"
#include "pTreeComboBox.h"
#include <QComboBox>

class UIAddExistingFiles : public QFileDialog
{
	Q_OBJECT

public:
	UIAddExistingFiles( ProjectsProxy*, ProjectItem*, const QStringList& = QStringList( "=" ), QWidget* = 0 );

	inline ProjectItem* currentItem() const { return qobject_cast<ProjectsModel*>( mProxy->sourceModel() )->itemFromIndex( mProxy->mapToSource( tcbProjects->currentIndex() ) ); }
	inline QString currentOperator() const { return cbOperators->currentText(); }

protected:
	ProjectsProxy* mProxy;
	ProjectItem* mItem;
	pTreeComboBox* tcbProjects;
	QComboBox* cbOperators;

};

#endif // UIADDEXISTINGFILES_H
