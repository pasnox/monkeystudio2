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
#ifndef PENVIRONMENTVARIABLESEDITOR_H
#define PENVIRONMENTVARIABLESEDITOR_H

#include "MonkeyExport.h"
#include "ui_pEnvironmentVariablesEditor.h"
#include "pEnvironmentVariablesModel.h"

class Q_MONKEY_EXPORT pEnvironmentVariablesEditor : public QWidget, public Ui::pEnvironmentVariablesEditor
{
    Q_OBJECT

public:
    pEnvironmentVariablesEditor( QWidget* parent = 0 );

    pEnvironmentVariablesModel::Variables variables() const;
    pEnvironmentVariablesModel::Variables defaultVariables() const;
    QStringList variables( bool keepDisabled ) const;
    pEnvironmentVariablesModel::Variable variable( const QString& name ) const;
    bool contains( const QString& name ) const;
    bool isEmpty() const;

public slots:
    void setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault );
    void setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables );
    void setVariables( const QStringList& variables, bool setDefault );
    void setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable );
    void removeVariable( const QString& name );
    void clearVariables();
    void resetVariablesToDefault();
    void resetVariablesToSystem( bool setDefault );

protected:
    pEnvironmentVariablesModel* mModel;

protected slots:
    void model_view_changed();
    void on_aAdd_triggered();
    void on_aEdit_triggered();
    void on_aRemove_triggered();
    void on_aClear_triggered();
    void on_aResetDefault_triggered();
    void on_aResetSystem_triggered();
    void on_tvVariables_activated( const QModelIndex& index );
};

#endif // PENVIRONMENTVARIABLESEDITOR_H
