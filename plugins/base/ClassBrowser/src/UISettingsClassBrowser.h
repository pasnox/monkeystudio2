#ifndef UISETTINGSCLASSBROWSER_H
#define UISETTINGSCLASSBROWSER_H

#include <QWidget>

#include <qCtagsSense.h>

class ClassBrowser;
class Ui_UISettingsClassBrowser;

class QAbstractButton;

class UISettingsClassBrowser : public QWidget
{
    Q_OBJECT

public:
    UISettingsClassBrowser( ClassBrowser* plugin, QWidget* parent = 0 );
    virtual ~UISettingsClassBrowser();

protected:
    ClassBrowser* mPlugin;
    Ui_UISettingsClassBrowser* ui;

protected slots:
    void on_tbDBFileName_clicked();
    void on_dbbButtons_clicked( QAbstractButton* button );

signals:
    void propertiesChanged( const qCtagsSenseProperties& properties );
};

#endif // UISETTINGSCLASSBROWSER_H
