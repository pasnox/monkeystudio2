#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_SearchWidget.h"
#include "SearchAndReplaceV2.h"

class SearchWidget : public QFrame, public Ui::SearchWidget
{
	Q_OBJECT

public:
	struct Properties
	{
		Properties()
		{
			mode = SearchAndReplaceV2::ModeNo;
			options = SearchAndReplaceV2::OptionNo;
			project = 0;
		}
		
		QString searchText;
		QString replaceText;
		QString searchPath;
		SearchAndReplaceV2::Mode mode;
		QStringList mask;
		QString codec;
		SearchAndReplaceV2::Options options;
		QMap<QString, QString> openedFiles; // filename, content
		class XUPProjectItem* project;
		QStringList sourcesFiles;
	};
	
	SearchWidget( QWidget* parent = 0 );
	virtual ~SearchWidget();
	
	SearchAndReplaceV2::Mode mode() const;

public slots:
	void setMode( SearchAndReplaceV2::Mode mode );

protected:
	SearchAndReplaceV2::Mode mMode;
	QMap<SearchAndReplaceV2::Mode, QAction*> mModeActions;
	QMap<SearchAndReplaceV2::Option, QAction*> mOptionActions;
	SearchWidget::Properties mProperties;
	
	void updateLabels();
	void updateWidgets();
	void initializeProperties();

protected slots:
	void groupMode_triggered( QAction* action );
	void on_pbPrevious_clicked();
	void on_pbNext_clicked();
	void on_pbSearch_clicked();
	void on_pbReplace_clicked();
	void on_pbReplaceAll_clicked();
	void on_pbReplaceChecked_clicked();
	void on_pbBrowse_clicked();
};

#endif // SEARCHWIDGET_H
