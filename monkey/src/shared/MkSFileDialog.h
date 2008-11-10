#ifndef MKSFILEDIALOG_H
#define MKSFILEDIALOG_H

#include "pFileDialog.h"

class MkSFileDialog : public pFileDialog
{
	Q_OBJECT
	
public:
	MkSFileDialog( QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), bool textCodecEnabled = true, bool openReadOnlyEnabled = false );
	
	static pFileDialogResult getOpenProjects( QWidget* parent = 0 );
};

#endif // MKSFILEDIALOG_H
