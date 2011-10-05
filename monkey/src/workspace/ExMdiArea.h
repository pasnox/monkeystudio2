#ifndef EXMDIAREA_H
#define EXMDIAREA_H

#include <QMdiArea>

class ExMdiArea : public QMdiArea
{
    Q_OBJECT

public:
    enum DocumentViewMode {
        Undefined = -1, // do not use
        MDI,
        NoTabs,
        TopTabs,
        BottomTabs,
        LeftTabs,
        RightTabs
    };
    
    ExMdiArea( QWidget* parent = 0 );
    
    ExMdiArea::DocumentViewMode documentViewMode() const;
    void setDocumentViewMode( ExMdiArea::DocumentViewMode mode );

protected:
    ExMdiArea::DocumentViewMode mMode;
    
    Qt::WindowFlags documentViewModeWindowFlags( ExMdiArea::DocumentViewMode mode ) const;
    
protected slots:
    void updateWindowFlags( QMdiSubWindow* window );
    void updateWindowsFlags();
};

#endif // EXMDIAREA_H
