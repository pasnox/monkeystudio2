#ifndef NAVIGATORSETTINGS_H
#define NAVIGATORSETTINGS_H

#include <QCheckBox>

class NavigatorSettings : public QWidget
{
Q_OBJECT
public:
	NavigatorSettings ();
	~NavigatorSettings ();	
private:
	QList<QCheckBox*>  displayFlags, expandFlags;
	QStringList names;
private slots:
	void setSettings();
};

#endif //NAVIGATORSETTINGS_H
