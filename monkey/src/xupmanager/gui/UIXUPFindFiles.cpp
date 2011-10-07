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
#include "UIXUPFindFiles.h"

#include <QDir>

UIXUPFindFiles::UIXUPFindFiles( const QString& findFile, QWidget* parent )
    : QDialog( parent )
{
    setupUi( this );
    lFindFile->setText( findFile );
}

UIXUPFindFiles::~UIXUPFindFiles()
{
}

void UIXUPFindFiles::setFiles( const QFileInfoList& files, const QString rootPath )
{
    QDir dir( rootPath );
    foreach ( const QFileInfo& fi, files )
    {
        QString text = rootPath.isEmpty() ? fi.fileName() : dir.relativeFilePath( fi.absoluteFilePath() );
        QListWidgetItem* item = new QListWidgetItem( lwFiles );
        item->setText( text );
        item->setToolTip( fi.absoluteFilePath() );
        lwFiles->addItem( item );
    }
    
    lwFiles->setCurrentRow( 0 );
}

void UIXUPFindFiles::on_lwFiles_itemSelectionChanged()
{
    QListWidgetItem* item = lwFiles->selectedItems().value( 0 );
    if ( item )
    {
        lAbsoluteFilePath->setText( item->toolTip() );
    }
}

void UIXUPFindFiles::on_lwFiles_itemActivated( QListWidgetItem* item )
{
    Q_UNUSED( item );
    accept();
}

QString UIXUPFindFiles::selectedFile() const
{
    QListWidgetItem* item = lwFiles->selectedItems().value( 0 );
    if ( item )
    {
        return item->toolTip();
    }
    
    return QString::null;
}

void UIXUPFindFiles::accept()
{
    if ( lwFiles->selectedItems().count() == 1 )
    {
        QDialog::accept();
    }
}
