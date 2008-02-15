#ifndef FINDINFILES_H
#define FINDINFILES_H

#include <QDialog>
#include <QPointer>
#include "ui_FindInFiles.h"

class QTreeWidget;
class QTreeWidgetItem;
class FifThread;

class FindInFiles : public QDialog, public Ui::FindInFiles
{
	Q_OBJECT
public:
	static FindInFiles* self( QWidget* = 0 );
	FindInFiles(QWidget *parent = 0);
	~FindInFiles();
private:
	static QPointer<FindInFiles> mSelf;
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

#endif //FINDINFILES_H
