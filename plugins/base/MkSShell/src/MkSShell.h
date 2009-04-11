#ifndef MKSSHELL_H
#define MKSSHELL_H

#include <BasePlugin.h>

class MkSShellDock;

class MkSShell : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	MkSShell();
	~MkSShell();
	virtual bool setEnabled( bool );
	virtual QPixmap pixmap() const;

protected:
	QPointer<MkSShellDock> mDock;
};

#endif // MKSSHELL_H
