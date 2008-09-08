/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
#include "QtAssistantChild.h"

QtAssistantChild::QtAssistantChild( QWidget* parent )
	: pAbstractChild( parent )
{
}

QtAssistantChild::~QtAssistantChild()
{
}

QPoint QtAssistantChild::cursorPosition() const
{
}

QString QtAssistantChild::currentFile() const
{
}

QString QtAssistantChild::currentFileName() const
{
}

pEditor* QtAssistantChild::currentEditor() const
{
}

bool QtAssistantChild::isModified() const
{
}

bool QtAssistantChild::isUndoAvailable() const
{
}

bool QtAssistantChild::isRedoAvailable() const
{
}

bool QtAssistantChild::isCopyAvailable() const
{
}

bool QtAssistantChild::isPasteAvailable() const
{
}

bool QtAssistantChild::isGoToAvailable() const
{
}

bool QtAssistantChild::isModified( const QString& filename ) const
{
}

bool QtAssistantChild::isPrintAvailable() const
{
}

void QtAssistantChild::showFile( const QString& filename )
{
}

void QtAssistantChild::undo()
{
}

void QtAssistantChild::redo()
{
}

void QtAssistantChild::cut()
{
}

void QtAssistantChild::copy()
{
}

void QtAssistantChild::paste()
{
}

void QtAssistantChild::goTo()
{
}

void QtAssistantChild::goTo( const QString& filename, const QPoint& pos, bool highlight )
{
}

void QtAssistantChild::invokeSearch()
{
}

void QtAssistantChild::saveFile( const QString& filename )
{
}

void QtAssistantChild::backupCurrentFile( const QString& filename )
{
}

void QtAssistantChild::saveFiles()
{
}

bool QtAssistantChild::openFile( const QString& filename, QTextCodec* codec )
{
}

void QtAssistantChild::closeFile( const QString& filename )
{
}

void QtAssistantChild::closeFiles()
{
}

void QtAssistantChild::printFile( const QString& filename )
{
}

void QtAssistantChild::quickPrintFile( const QString& filename )
{
}
