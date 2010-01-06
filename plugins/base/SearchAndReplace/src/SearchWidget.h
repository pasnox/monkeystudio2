#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_SearchWidget.h"
#include "SearchAndReplace.h"

#include <QFile>

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
			mode = SearchAndReplace::ModeNo;
			options = SearchAndReplace::OptionNo;
			project = 0;
		}

		QString searchText;
		QString replaceText;
		QString searchPath;
		SearchAndReplace::Mode mode;
		QStringList mask;
		QString codec;
		SearchAndReplace::Options options;
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

	SearchAndReplace::Mode mode() const;
	SearchThread* searchThread() const;

	void setResultsDock( SearchResultsDock* dock );
	
	static bool isBinary( QFile& file );

public slots:
	void setMode( SearchAndReplace::Mode mode );

protected:
	SearchAndReplace::Mode mMode;
	QMap<SearchAndReplace::Mode, QAction*> mModeActions;
	QMap<SearchAndReplace::Option, QAction*> mOptionActions;
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
	bool searchFile( bool forward, bool incremental );
	bool replaceFile( bool all );

protected slots:
	void searchThread_stateChanged();
	void replaceThread_stateChanged();
	void replaceThread_openedFileHandled( const QString& fileName, const QString& content, const QString& codec );
	void replaceThread_error( const QString& error );
	void search_textChanged();
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
};

#endif // SEARCHWIDGET_H
