#ifndef XUPADDFILES_H
#define XUPADDFILES_H

#include "ui_XUPAddFiles.h"

class XUPProjectModelProxy;
class XUPProjectModel;
class XUPItem;

class XUPAddFiles : public QWidget, public Ui::XUPAddFiles
{
	Q_OBJECT

public:
	XUPAddFiles( QWidget* parent = 0 );
	virtual ~XUPAddFiles();
	
	void setModel( XUPProjectModel* model );
	XUPProjectModel* model() const;
	
	void setCurrentScope( XUPItem* item );
	XUPItem* currentScope() const;
	
	void setOperators( const QStringList& operators );
	void setCurrentOperator( const QString& op );
	QString currentOperator() const;
	
	void setImportExternalFiles( bool import );
	bool importExternalFiles() const;
	
	void setImportExternalFilesPath( const QString& path );
	QString importExternalFilesPath() const;

protected:
	XUPProjectModelProxy* mProxy;
	XUPProjectModel* mModel;

protected slots:
	void on_tcbScopes_currentChanged( const QModelIndex& index );

signals:
	void currentScopeChanged( XUPItem* scope );
};

#endif // XUPADDFILES_H
