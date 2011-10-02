#ifndef QMAKETRANSLATIONSEDITOR_H
#define QMAKETRANSLATIONSEDITOR_H

#include <xupmanager/gui/XUPPageEditor.h>

#include <QHash>

class Ui_QMakeTranslationsEditor;
class XUPItem;
class pLocaleModel;

class QMakeTranslationsEditor : public XUPPageEditor
{
	Q_OBJECT

public:
	QMakeTranslationsEditor( QWidget* parent = 0 );
	virtual ~QMakeTranslationsEditor();

	virtual void setup( XUPProjectItem* project );
	virtual void finalize();

protected:
	Ui_QMakeTranslationsEditor* ui;
	XUPProjectItem* mProject;
    pLocaleModel* mModel;
    QHash<QString, XUPItem*> mCurrentTranslations;

protected slots:
    void on_tbDirectory_clicked();
};

#endif // QMAKETRANSLATIONSEDITOR_H
