#include <QDir>
#include <QTreeWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QProgressBar>

#include "FindInFiles.h"
#include "FifThread.h"
#include "../Tools.h"
#include "../editor/Editor.h"
#include "../project/ProjectContainer.h"
#include "../project/ProjectItemModel.h"
#include "../output/Output.h"
#include "../StatusBar.h"
//
QPointer<FindInFiles> FindInFiles::mSelf = 0L;
//
FindInFiles* FindInFiles::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new FindInFiles( p );
	return mSelf;
}
//
FindInFiles::FindInFiles(QWidget *parent) : QDialog(parent), fifTh(0)
{
	mSelf = this;
	setupUi(this);
	twResult = new QTreeWidget();

	twResult->setItemHidden(twResult->headerItem(), true);
	twResult->setColumnCount(1);
	twResult->setRootIsDecorated(false);
	connect (twResult, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
			 this, SLOT(itemDbClick(QTreeWidgetItem*, int)));
	connect (StatusBar::self()->getStopButton(), SIGNAL(clicked()), SLOT(stopThread()));

	Output::self()->addTab(twResult, tr("Find in files"));
	Output::self()->setTabIcon(Output::self()->count()-1, QIcon(":/Icons_Tabs/Icons_Tabs/find.png"));
}
//
FindInFiles::~FindInFiles()
{
}
//
void FindInFiles::accept()
{
	text = cbText->currentText();
	if (text.isEmpty())
		return;

	Output::self()->setCurrentWidget(twResult);
	StatusBar::self()->showProgress(true);

	twResult->clear();
	QStringList mask = leMask->text().simplified().split(" ", QString::SkipEmptyParts);

	QString path = lePath->text();
	QString filePath;

	QDir directory = QDir(path);
	QList<QFileInfo> all;

	if (mask.isEmpty())
		mask << "*";

	if (rbOpen->isChecked())
		files = Editor::self()->getOpenedNames();
	else if (rbPro->isChecked())
		files = ProjectContainer::self()->getProFiles(true);
	else if (!chBoxRec->isChecked() || !chBoxRec->isEnabled())
		all = directory.entryInfoList(mask, QDir::Files|QDir::NoSymLinks);
	else
		all = directory.entryInfoList(mask, QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);

	QFileInfo info;
	while (!all.isEmpty())
	{
		info = all.first();
		filePath = info.canonicalFilePath();

		if (info.isFile())
			files<<filePath;
		if (info.isDir())
			all<<QDir(filePath).entryInfoList(mask,
											  QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);

		all.removeFirst();
	}

	StatusBar::self()->getProgress()->setRange(0, files.count());

	findFiles();
	close();
}
//
void FindInFiles::showEvent(QShowEvent* e)
{
	int wasChecked = 0;

	if (rbOpen->isChecked() )
		wasChecked = 1;
	if (rbSearch->isChecked())
		wasChecked = 2;

	if (!ProjectContainer::self()->currentProject())
	{
		rbPro->setEnabled(false);
		if (wasChecked < 2)
			rbOpen->setChecked(true);
	}
	else
		rbPro->setEnabled(true);

	if(!Editor::self()->count())
	{
		rbOpen->setEnabled(false);
		if (ProjectContainer::self()->currentProject() && wasChecked != 2)
			rbPro->setChecked(true);
		else
			rbSearch->setChecked(true);
	}
	else
		rbOpen->setEnabled(true);

	if (lePath->text().simplified().isEmpty())
		lePath->setText( Tools::lastDir(Tools::OpenFileDir) );

	QDialog::showEvent(e);
}
//
void FindInFiles::findFiles()
{
	mCurDir = QString();
	mCurFile = QString();
	dirTwi = 0;
	fileTwi = 0;

	//twResult->setRootIsDecorated(rbSearch->isChecked());

	fifTh = new FifThread(files, text,
						  chBoxWhole->isChecked(),
						  chBoxMatch->isChecked(),
						  chBoxReg->isChecked(),this);

	connect(fifTh, SIGNAL(sig_updateResult(uint, const QString&, const QString&)),
			this, SLOT(updateResult(uint, const QString&, const QString&)));
	connect(fifTh, SIGNAL(sig_changeProgress(int)), SLOT(changeProgress(int)));
	connect(fifTh, SIGNAL(finished ()), SLOT(searchFinished()));

	files.clear();
	text.clear();

	pbOk->setEnabled(false);
	fifTh->start();
}
//
void FindInFiles::updateResult(uint num, const QString& filePath, const QString& line)
{
	QString fileName = filePath;
	QString dir = QFileInfo(fileName).canonicalPath();
	if (rbPro->isChecked())
		fileName= Tools::makeRelative( fileName, ProjectContainer::self()->currentProject()->path() );
	else
		fileName =  QFileInfo(fileName).fileName();

	if (chBoxRec->isChecked() && chBoxRec->isEnabled())
	{
		if (mCurDir!=dir)
		{
			dirTwi = new QTreeWidgetItem(QStringList()<<dir);
			dirTwi->setFlags(Qt::ItemIsEnabled);
			dirTwi->setBackgroundColor(0, "#9b9bc8");//"#e1ffe1");#9bc89b
			twResult->addTopLevelItem(dirTwi);
			dirTwi->setExpanded(true);
			mCurDir = dir;
		}
		if (mCurFile!=fileName)
		{
			fileTwi = new QTreeWidgetItem(QStringList()<<fileName);
			fileTwi->setFlags(Qt::ItemIsEnabled);
			fileTwi->setBackgroundColor(0, "#e1e1ff");
			dirTwi->addChild(fileTwi);
			fileTwi->setExpanded(true);
			mCurFile=fileName;
		}
		QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<QString::number(num)+" : "+ line);
		item->setFlags(Qt::ItemIsEnabled);
		item->setData(0, 1025, filePath);
		item->setData(0, 1026, num);
		item->setBackgroundColor(0, "#fffff5");
		fileTwi->addChild(item);
	}
	else
	{
		if (mCurFile!=fileName)
		{
			fileTwi = new QTreeWidgetItem(QStringList()<<fileName);
			fileTwi->setFlags(Qt::ItemIsEnabled);
			fileTwi->setBackgroundColor(0, "#e1e1ff");
			twResult->addTopLevelItem(fileTwi);
			fileTwi->setExpanded(true);
			mCurFile=fileName;
		}
		QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<QString::number(num)+" : "+ line);
		item->setFlags(Qt::ItemIsEnabled);
		item->setData(0, 1025, filePath);
		item->setData(0, 1026, num);
		item->setBackgroundColor(0, "#fffff5");
		fileTwi->addChild(item);
	}
}
//
void FindInFiles::itemDbClick( QTreeWidgetItem * item, int /*column*/)
{
	QString fileName = item->data(0, 1025).toString();

	if (fileName.isEmpty())
		return;

	openFile(fileName, item->data(0, 1026).toInt());
}
//
void FindInFiles::openFile(const QString& filePath, int line)
{
	if (Editor::self()->openFile(QStringList()<<filePath))
	{
		Editor::self()->setErrorLine(line, true);
		//Editor::self()->setOpened();
	}
}
//
void FindInFiles::searchFinished()
{
	if (!twResult->topLevelItemCount())
		twResult	->addTopLevelItem(new QTreeWidgetItem(QStringList()<<tr("Files not found")));

	pbOk->setEnabled(true);
	StatusBar::self()->showProgress(false);

	fifTh->deleteLater();
	fifTh = 0;
}
//
void FindInFiles::on_tbPath_clicked()
{
	QString pathName = QFileDialog::getExistingDirectory( this, tr( "Choose a path" ), lePath->text());
	if ( !pathName.isEmpty() )
		lePath->setText( pathName );
}
//
void FindInFiles::changeProgress(int val)
{
	StatusBar::self()->getProgress()->setValue(val);
}
//
void FindInFiles::stopThread()
{
	fifTh->setTermEnabled(true);
}
