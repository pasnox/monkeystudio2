/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : NavigatorSettings.cpp
** Date      : 2008-01-14T00:40:09
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "NavigatorSettings.h"
#include "Navigator.h"

NavigatorSettings::NavigatorSettings ()
{
	names <<"Namespaces"<<	"Classes"<<"Constructors"<<"Destrutors";
	names <<"Enums"<<"Enumerators"<<"Externs"<<"Functions";
	names <<"Macroses"<<"Members"<<"Prototypes"<<"Structures";
	names <<"Typedefs"<<"Unions"<<"Variables"<<	"Uncknown";

	QVBoxLayout* vbox = new QVBoxLayout (this);
	
	QHBoxLayout* settingsBox = new QHBoxLayout ();
	QVBoxLayout* displayBox = new QVBoxLayout ();
	QVBoxLayout* expandBox = new QVBoxLayout ();
	displayBox->addWidget ( new QLabel ("Display:"));
	expandBox->addWidget ( new QLabel ("Automaticaly expand:"));
	
	for (int i = 0; i < names.size(); i++)
	{
		displayFlags.append (new QCheckBox(names[i],this));
		displayBox->addWidget (displayFlags[i]);
		displayFlags[i]->setChecked ( Navigator::instance()->getDisplayMask() & (1<<i));
		expandFlags.append(new QCheckBox(names[i],this));
		expandBox->addWidget (expandFlags[i]);
		expandFlags[i]->setChecked ( Navigator::instance()->getExpandMask() & (1<<i));
	}

	settingsBox->addLayout (displayBox);
	settingsBox->addLayout (expandBox);

	QHBoxLayout* applyBox = new QHBoxLayout (this);
	QPushButton* applyBtn = new QPushButton (tr("Apply"), this);
	applyBox->addWidget (applyBtn, 0, Qt::AlignRight);
	
	vbox->addLayout (settingsBox);
	vbox->addLayout (applyBox);
	
	connect ( applyBtn, SIGNAL ( clicked()), this, SLOT (setSettings()));
}

NavigatorSettings::~NavigatorSettings ()
{
}

void NavigatorSettings::setSettings()
{
	int displayMask = 0;
	int expandMask = 0;
	for (int i = 0; i < names.size(); i++)
	{
		displayMask |=  (displayFlags[i]->isChecked())<<i;
		expandMask |=  (expandFlags[i]->isChecked())<<i;
	}
	Navigator::instance()->setDisplayMask (displayMask);
	Navigator::instance()->setExpandMask (expandMask);	
}
