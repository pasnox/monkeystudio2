/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_SearchWidget.h"
#include "SearchAndReplace.h"
#include "SearchResultsModel.h"

#include <QFile>

class SearchAndReplace;
class SearchThread;
class ReplaceThread;
class SearchResultsDock;
class QProgressBar;
class QToolButton;

class SearchWidget : public QFrame, public Ui::SearchWidget
{
    Q_OBJECT

public:
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

    SearchWidget( SearchAndReplace* plugin, QWidget* parent = 0 );
    virtual ~SearchWidget();

    SearchAndReplace::Mode mode() const;
    SearchThread* searchThread() const;

    void setResultsDock( SearchResultsDock* dock );
    
    static bool isBinary( QFile& file );

public slots:
    void setMode( SearchAndReplace::Mode mode );

protected:
    SearchAndReplace* mPlugin;
    SearchAndReplace::Mode mMode;
    QMap<SearchAndReplace::Option, QAction*> mOptionActions;
    SearchAndReplace::Properties mProperties;
    SearchThread* mSearchThread;
    ReplaceThread* mReplaceThread;
    SearchResultsDock* mDock;
    QToolButton* tbMode;
    QToolButton* tbCdUp;
    QProgressBar* mProgress;

    virtual bool eventFilter( QObject* object, QEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );

    void updateLabels();
    void updateWidgets();
    void updateComboBoxes();
    void initializeProperties( bool currentDocumentOnly );
    void showMessage( const QString& status );
    void setState( SearchWidget::InputField field, SearchWidget::State state );
    bool searchFile( bool forward, bool incremental );
    bool replaceFile( bool all );

protected slots:
    void searchThread_stateChanged();
    void searchThread_progressChanged( int value, int total );
    void replaceThread_stateChanged();
    void replaceThread_openedFileHandled( const QString& fileName, const QString& content, const QString& codec );
    void replaceThread_error( const QString& error );
    void search_textChanged();
    void cdUp_clicked();
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
