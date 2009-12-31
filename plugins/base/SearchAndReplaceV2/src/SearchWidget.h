#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_SearchWidget.h"
#include "SearchAndReplaceV2.h"

class SearchThread;
class ReplaceThread;
class SearchResultsDock;

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
	
	enum InputField
	{
		Search,
		Replace
	};
	
	enum State
	{
		Normal,
		Good,
		Bad
	};
	
	SearchWidget( QWidget* parent = 0 );
	virtual ~SearchWidget();
	
	SearchAndReplaceV2::Mode mode() const;
	SearchThread* searchThread() const;
	
	void setResultsDock( SearchResultsDock* dock );

public slots:
	void setMode( SearchAndReplaceV2::Mode mode );

protected:
	SearchAndReplaceV2::Mode mMode;
	QMap<SearchAndReplaceV2::Mode, QAction*> mModeActions;
	QMap<SearchAndReplaceV2::Option, QAction*> mOptionActions;
	SearchWidget::Properties mProperties;
	SearchThread* mSearchThread;
	ReplaceThread* mReplaceThread;
	SearchResultsDock* mDock;
	
	virtual void keyPressEvent( QKeyEvent* event );
	
	void updateLabels();
	void updateWidgets();
	void initializeProperties();
	void showMessage( const QString& status );
	void setState( SearchWidget::InputField field, SearchWidget::State state );
	bool searchFile( bool forward );
	bool replaceFile( bool all );

protected slots:
	void searchThread_stateChanged();
	void replaceThread_stateChanged();
	void replaceThread_openedFileHandled( const QString& fileName, const QString& content, const QString& codec );
	void replaceThread_error( const QString& error );
	void groupMode_triggered( QAction* action );
	void on_pbPrevious_clicked();
	void on_pbNext_clicked();
	void on_pbSearch_clicked();
	void on_pbSearchStop_clicked();
	void on_pbReplace_clicked();
	void on_pbReplaceAll_clicked();
	void on_pbReplaceChecked_clicked();
	void on_pbReplaceCheckedStop_clicked();
	void on_pbBrowse_clicked();
	
	void on_pbDebug_clicked();
};

#endif // SEARCHWIDGET_H
