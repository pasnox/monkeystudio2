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
/*!
    \file AbstractCommandParser.h
    \author Andrei Kopats
    \brief Header for AbstractCommandParser class
*/

#ifndef ABSTRACTCOMMANDPARSER_H
#define ABSTRACTCOMMANDPARSER_H

#include "pConsoleManager.h"
#include "pConsoleManagerStep.h"

#include <QObject>

/*!
    Change value of this macro, if you need to debug parsing or parsers, 
    and also check, how much time parsing takes
    A lot of debug info will be printed
*/
#define PARSERS_DEBUG 0

/*!
    Parent class for parsers of console output.
    
    Inherit this class for create own parser. There is some description of it 
    in a wiki documentation of project
*/
class Q_MONKEY_EXPORT AbstractCommandParser : public QObject
{
    Q_OBJECT    
    
public:

    virtual QString name() const = 0;

public :
    AbstractCommandParser(QObject* parent): QObject(parent) {};
    
    
    /**
     *\brief Parse strings
     *\param strings Strings to parse
     * Parsed lines are removed from the list
     */
    virtual void processParsing(QStringList& strings) = 0;

signals:
    void newStepAvailable( const pConsoleManagerStep& );
    void newStepsAvailable( const pConsoleManagerStepList& );
};

typedef QList<AbstractCommandParser*> AbstractCommandParserList;

Q_DECLARE_METATYPE( AbstractCommandParserList );

#endif // ABSTRACTCOMMANDPARSER
