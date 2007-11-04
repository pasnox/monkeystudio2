/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UITemplatesWizard.h
** Date      : 2007-11-04T22:46:25
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef UITEMPLATESWIZARD
#define UITEMPLATESWIZARD

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UITemplatesWizard.h"
#include "pTemplatesManager.h"

class ProjectsModel;

class Q_MONKEY_EXPORT UITemplatesWizard : public QDialog, public Ui::UITemplatesWizard, public QSingleton<UITemplatesWizard>
{
	Q_OBJECT
	friend class QSingleton<UITemplatesWizard>;
	
public:
	void setType( pTemplate::TemplateType );

private:
	ProjectsModel* mProjects;
	UITemplatesWizard( QWidget* = 0 );

private slots:
	void on_cbLanguages_currentIndexChanged( const QString& );
	void on_cbTypes_currentIndexChanged( const QString& );
	void on_swPages_currentChanged( int );
	void on_tbDestination_clicked();
	void on_pbPrevious_clicked();
	void on_pbNext_clicked();
	bool checkTemplates();
	void generatePreview();
	void accept();
};

#endif // UITEMPLATESWIZARD
 
