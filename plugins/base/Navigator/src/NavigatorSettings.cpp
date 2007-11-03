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
