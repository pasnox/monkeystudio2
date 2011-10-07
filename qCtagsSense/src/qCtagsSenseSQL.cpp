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
#include "qCtagsSenseSQL.h"

#include <QDir>
#include <QStringList>
#include <QSqlError>
#include <QDebug>

qCtagsSenseSQL::qCtagsSenseSQL( QObject* parent )
    : QObject( parent )
{
    mDBConnectionName = "qCtagsSenseSQL";
}

qCtagsSenseSQL::~qCtagsSenseSQL()
{
    removeCurrentDatabase();
}

bool qCtagsSenseSQL::initializeDatabase( const QString& fileName )
{
    if ( !removeCurrentDatabase() )
    {
        return false;
    }
    
    if ( QFile::exists( fileName ) )
    {
        if ( !QFile::remove( fileName ) )
        {
            return false;
        }
    }
    
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", mDBConnectionName );
    
    db.setDatabaseName( fileName.isEmpty() ? ":memory:" : fileName );
    //db.setConnectOptions( "QSQLITE_BUSY_TIMEOUT=500" ); // in milliseconds
    
    if ( !db.open() )
    {
        qWarning() << db.lastError().text();
        return false;
    }
    
    if ( !db.tables().contains( "files", Qt::CaseInsensitive ) )
    {
        if ( !initializeTables() )
        {
            db.close();
            QSqlDatabase::removeDatabase( mDBConnectionName );
            qWarning() << "Can't initialize tables";
            return false;
        }
    }
    
    return true;
}

bool qCtagsSenseSQL::initializeTables() const
{
    const QString sql_files = QString(
        "CREATE TABLE 'main'.'files' ("
        "'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
        "'fileName' TEXT NOT NULL,"
        "'language' TEXT NOT NULL"
        ");"
    );
    
    const QString sql_entries = QString(
        "CREATE TABLE 'main'.'entries' ("
        "'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
        "'file_id' INTEGER NOT NULL,"
        "'line_number_entry' INTEGER NOT NULL,"
        "'line_number' INTEGER NOT NULL,"
        "'is_file_scope' INTEGER NOT NULL,"
        "'is_file_entry' INTEGER NOT NULL,"
        "'truncate_line' INTEGER NOT NULL,"
        "'name' TEXT NOT NULL,"
        "'kind' INTEGER NOT NULL,"
        "'access' TEXT,"
        "'file_scope' TEXT,"
        "'implementation' TEXT,"
        "'inheritance' TEXT,"
        "'scope_value' TEXT,"
        "'scope_key' TEXT,"
        "'signature' TEXT,"
        "'type' TEXT,"
        "'type_name' TEXT"
        ");"
    );
    
    const QString sql_files_trigger = QString(
        "CREATE TRIGGER 'files_after_delete' AFTER DELETE ON 'files' FOR EACH ROW\n"
        "BEGIN\n"
        "DELETE FROM 'entries' WHERE 'entries'.'file_id' = 'old'.'id';\n"
        "END;"
    );
    
    const QString sql_files_filename_asc = QString(
        "CREATE INDEX 'files_filename_asc' on 'files' (fileName ASC)"
    );
    
    const QString sql_files_language_asc = QString(
        "CREATE INDEX 'files_language_asc' on 'files' (language ASC)"
    );
    
    const QString sql_entries_name_asc = QString(
        "CREATE INDEX 'entries_name_asc' on 'entries' (name ASC)"
    );
    
    const QString sql_entries_scope_key_asc = QString(
        "CREATE INDEX 'entries_scope_key_asc' on 'entries' (scope_key ASC)"
    );
    
    QSqlQuery q = query();
    
    if ( q.exec( sql_files ) )
    {
        if ( q.exec( sql_entries ) )
        {
            if ( q.exec( sql_files_trigger ) )
            {
                /*
                if ( q.exec( sql_files_filename_asc ) )
                {
                    if ( q.exec( sql_files_language_asc ) )
                    {
                        if ( q.exec( sql_entries_name_asc ) )
                        {
                            if ( q.exec( sql_entries_scope_key_asc ) )
                            {
                                return true;
                            }
                            else
                            {
                                qWarning() << "Can't create scope_key index" << q.lastError().text();
                            }
                        }
                        else
                        {
                            qWarning() << "Can't create name index" << q.lastError().text();
                        }
                    }
                    else
                    {
                        qWarning() << "Can't create language index" << q.lastError().text();
                    }
                }
                else
                {
                    qWarning() << "Can't create filename index" << q.lastError().text();
                }
                */
                
                return true;
            }
            else
            {
                qWarning() << "Can't create files trigger" << q.lastError().text();
            }
        }
        else
        {
            qWarning() << "Can't create entries table" << q.lastError().text();
        }
    }
    else
    {
        qWarning() << "Can't create files table" << q.lastError().text();
    }
    
    return false;
}

bool qCtagsSenseSQL::removeCurrentDatabase()
{
    // clear database if needed
    if ( QSqlDatabase::contains( mDBConnectionName ) )
    {
        {
            QSqlDatabase db = QSqlDatabase::database( mDBConnectionName );
            db.close();
            
            if ( QFile::exists( db.databaseName() ) )
            {
                if ( !QFile::remove( db.databaseName() ) )
                {
                    return false;
                }
            }
        }
        
        QSqlDatabase::removeDatabase( mDBConnectionName );
    }
    
    return true;
}

QSqlDatabase qCtagsSenseSQL::database() const
{
    return QSqlDatabase::database( mDBConnectionName, false );
}

QSqlQuery qCtagsSenseSQL::query() const
{
    QSqlQuery q( database() );
    q.setForwardOnly( true );
    
    return q;
}
