#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QDomElement>

class notesManager
{
    public:
        notesManager( QString filepath );
        ~notesManager();

        bool readDocument();
        bool writeDocument();

        void removeElement( uint id );
        void removeAllElements();

        QDomElement getElement( uint id );
        QStringList getElements();

        void addElement( QString title, QString note );
        void setElement( uint id, QString note );
        void setTitleElement( uint id, QString title );

        uint getNotesCount();

    private:
        QDomDocument mXmlDocument;
        QString mFilepath;
};

#endif // NOTESMANAGER_H
