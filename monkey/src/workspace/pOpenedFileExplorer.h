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
#ifndef POPENEDFILEEXPLORER_H
#define POPENEDFILEEXPLORER_H

#include <MonkeyExport.h>

#include "ui_pOpenedFileExplorer.h"
#include "pOpenedFileModel.h"

class pWorkspace;
class pAbstractChild;
class pOpenedFileAction;

class Q_MONKEY_EXPORT pOpenedFileExplorer : public pDockWidget, public Ui::pOpenedFileExplorer
{
    Q_OBJECT
    friend class pOpenedFileAction;

public:
    pOpenedFileExplorer( pWorkspace* workspace );
    
    pOpenedFileModel* model() const;
    QAction* comboBoxAction() const;
    
    pOpenedFileModel::SortMode sortMode() const;
    void setSortMode( pOpenedFileModel::SortMode mode );

protected:
    pWorkspace* mWorkspace;
    pOpenedFileModel* mModel;
    QMenu* mSortMenu;
    pOpenedFileAction* aComboBox;

protected slots:
    void syncViewsIndex( const QModelIndex& index, bool syncOnly = false );
    void sortTriggered( QAction* action );
    void documentChanged( pAbstractChild* document );
    void currentDocumentChanged( pAbstractChild* document );
    void sortModeChanged( pOpenedFileModel::SortMode mode );
    void documentsSorted();
    void selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
    void on_tvFiles_customContextMenuRequested( const QPoint& pos );
};

#endif // POPENEDFILEEXPLORER_H
