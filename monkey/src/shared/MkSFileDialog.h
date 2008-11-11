#ifndef MKSFILEDIALOG_H
#define MKSFILEDIALOG_H

#include "pFileDialog.h"

class XUPAddFiles;
class XUPItem;

class MkSFileDialog : public pFileDialog
{
	Q_OBJECT
	
public:
	MkSFileDialog( QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), bool textCodecEnabled = true, bool openReadOnlyEnabled = false );
	
	static pFileDialogResult getOpenProjects( QWidget* parent = 0 );
	static pFileDialogResult getProjectAddFiles( QWidget* parent = 0 );

protected:
	XUPAddFiles* mAddFiles;

protected slots:
	void currentScopeChanged( XUPItem* scope );
};

#endif // MKSFILEDIALOG_H
