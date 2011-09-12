#ifndef DEBUGDOCKWIDGET_H
#define DEBUGDOCKWIDGET_H

#include "ui_DebugDockWidget.h"

class XUPItem;

class DebugDockWidget : public pDockWidget, public Ui::DebugDockWidget
{
    Q_OBJECT
    
public:
    DebugDockWidget( QWidget* parent = 0 );
    virtual ~DebugDockWidget();

public slots:
    void qtMessageHandler( QtMsgType type, const char* msg );

protected:
    void createAllScopes( XUPItem* parent );
    void createScope( XUPItem* parent, bool nested1, bool nested2, bool nested3, bool nested4 = false, bool nested5 = false );

protected slots:
    void currentProjectChanged();
    void showXml();
    void showNativeString();
    void generateFakeProject();
    void childInfos();
};

#endif // DEBUGDOCKWIDGET_H
