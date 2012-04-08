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
/*!
    \file gdbSetting.h
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief Load setting from Monkey .ini and store current configuration
*/

#ifndef GDBSETTING_H
#define GDBSETTING_H

#include <QObject>
#include <QString>
#include <QHash>

/*!
    \brief Load setting from Monkey .ini and store current configuration
    \details It load setting from Monkey .ini. It configure AddOn (enable or not) 
    and load current path.
*/
class GdbSetting : public QObject
{
    Q_OBJECT


private :

    /*!
        \details Struct contains current path and AddOn list  
    */
//  GdbSettings gdbSettings;

public :

    GdbSetting(QObject * parent = 0);
    ~GdbSetting(){};

    /*! 
        \details Load setting from .ini
    */
    void load();

    /*! 
        \details Save setting in .ini
    */
    void save();
    
    
    /*! 
        \details Set enable or disable AddOn
        \param addOnName is the name of AddOn
        \param b is true if AddOn is enables from .ini
        \sa getStartUp()
    */
    void setEnable(QString , bool );
    
    /*!
        \details Get if AddOn is Enable or Disable
        \param addOnName is the name of addOn
        \retval Current state saved in .ini, if AddOn is not found in .ini, return true by default
        \sa setEnable()
    */
    bool getStartUp(QString );


    /*!
        \details Get path of Gdb
    */
    QString getPathGdb() ;

    /*!
        \details Set path of Gdb
    */
    void setPathGdb(QString s);

    /*!
        \details Get path of path script file
    */
    QString getPathScript();
    /*!
        \details Set path of path script file
    */
    void setPathScript(QString s) ;

    /*!
        \details Get list of AddOns
    */
    void setAddonsAvailable(QString , bool);
    QHash<QString, bool> getAddonsAvailable();

//  QList<GdbAddOn> * getAddOnList() { return &gdbSettings.addOnList;}
private :
    /*! 
        \details Current path of Gdb
    */
    QString pathGdb;
    /*! 
        \details Current path of parse file
    */
    QString pathParseFile;
    /*! 
        \details Current path of script file
    */
    QString pathScript;
    /*! 
        \details List of all AddOn
    */
    QHash<QString, bool> mAddonList;
};
#endif
