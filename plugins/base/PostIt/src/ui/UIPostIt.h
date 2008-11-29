#ifndef UIPOSTIT_H
#define UIPOSTIT_H

#include "ui_UIPostIt.h"
#include "notesManager.h"


class UIPostIt : public QMainWindow, public Ui::UIPostIt
{
	Q_OBJECT

public:
	UIPostIt( QWidget* parent = 0 );
	~UIPostIt();

private:
        void loadNotes();

        notesManager *mNotesManager;
        bool mTextChanged;


protected slots:
        void on_tbAdd_clicked();
        void on_lstNotes_itemActivated ( QListWidgetItem *item );
        void on_lstNotes_itemDoubleClicked ( QListWidgetItem *item );
        void on_tbDelete_clicked();
        void closeEvent( QCloseEvent *event );
        void on_txtContent_textChanged();
        void on_tbClear_clicked();

protected:
        bool eventFilter(QObject *obj, QEvent *event);
};

#endif // UIPOSTIT
