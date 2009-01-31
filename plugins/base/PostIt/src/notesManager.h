#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QDomElement>

class notesManager
{
    public:
        notesManager( const QString &filepath );
        ~notesManager();

        bool readDocument();
        bool writeDocument();

        void removeElement( const uint id );
        void removeAllElements();

		QString getTitleElement( const uint id ) const;
        QDomElement getElement( const uint id ) const;
        QStringList getElements() const;

        void addElement( const QString &title, const QString &note );
        void setElement( uint id, const QString &note );
        void setTitleElement( uint id, const QString &title );

        uint getNotesCount() const;

    private:
        QDomDocument mXmlDocument;
        QString mFilepath;
};

#endif // NOTESMANAGER_H
