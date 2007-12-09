#ifndef UITEMPLATESWIZARD
#define UITEMPLATESWIZARD

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UITemplatesWizard.h"
#include "pTemplatesManager.h"

class ProjectsModel;

class Q_MONKEY_EXPORT UITemplatesWizard : public QDialog, public Ui::UITemplatesWizard, public QSingleton<UITemplatesWizard>
{
	friend class QSingleton<UITemplatesWizard>;
    Q_OBJECT
	
public:
	void setType( const QString& );
    
private:
	QGridLayout* gridLayout;
	QList<QLabel*> mLabels;
	QList<QComboBox*> mCombos;
    TemplateList mTemplates;
    ProjectsModel* mProjects;

    UITemplatesWizard( QWidget* = 0 );
    QStringList getLanguages();
    QStringList getTypes();

private slots:
    void onFiltersChanged();
	void on_lwTemplates_itemPressed( QListWidgetItem* );
    void on_tbDestination_clicked();

    bool checkTemplate();
    void on_pbCreate_clicked();
};

#endif // UITEMPLATESWIZARD
 
