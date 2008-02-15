#ifndef DOCKFINDINFILES_H
#define DOCKFINDINFILES_H

#include <QDockWidget>

#include "QSingleton.h"

#include "ui_DockFindInFiles.h"

class QTreeWidget;
class QTreeWidgetItem;
class FifThread;

class DockFindInFiles : public QDockWidget, public QSingleton <DockFindInFiles>, public Ui::DockFindInFiles
{
	Q_OBJECT
	friend class QSingleton <DockFindInFiles>;
public:
	DockFindInFiles(QWidget *parent = 0);
	~DockFindInFiles();
private:
	QString mCurDir;
	QString mCurFile;
	QStringList files;
	QString text;
	QTreeWidgetItem* dirTwi;
	QTreeWidgetItem* fileTwi;
	QTreeWidget* twResult;
	FifThread* fifTh;
private:
	void findFiles();
	void openFile(const QString&,int);
protected:
	void showEvent(QShowEvent*);
private slots:
	void accept();
	void updateResult(uint, const QString&, const QString&);
	void itemDbClick(QTreeWidgetItem*, int);
	void searchFinished();
	void on_tbPath_clicked();
	void changeProgress(int val);
	void stopThread();
};

#endif //DOCKFINDINFILES_H
