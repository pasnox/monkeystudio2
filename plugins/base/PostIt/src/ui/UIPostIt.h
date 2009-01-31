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


protected slots:
        void on_tbAdd_clicked();
		void on_tbEdit_clicked();
        void on_tbDelete_clicked();
        void on_tbClear_clicked();
        void on_lstNotes_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
        void closeEvent( QCloseEvent *event );
};

#endif // UIPOSTIT
