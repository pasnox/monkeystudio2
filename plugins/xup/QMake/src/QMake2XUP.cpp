/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "QMake2XUP.h"
#include "QMakeProjectItem.h"

#include <coremanager/MonkeyCore.h>

/**************************
WARNING :
if "operator" does not exists it imply "="
if "nested" does not exists it imply "false"
**************************/

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtXml>
#include <QTextCodec>

#include <exception>

class MksException : public std::exception
{
public:
    MksException( const QString& p_s ) throw()
    {
        s = p_s;
    };
    virtual ~MksException() throw() {};
    virtual const char* what() const throw()
    {
        return s.toLocal8Bit().constData();
    };
private:
    QString s;
};

const QString GENERATED_XUP_VERSION = "1.1.0";

QString QMake2XUP::convertFromPro( const QString& s, const QString& codec )
{
    // check if file exists
    if ( !QFile::exists( s ) )
        return QString();
    
    const DocumentFilterMap& filters = QMakeProjectItem().documentFilters();
    const QStringList fileVariables = filters.fileVariables();
    const QStringList pathVariables = filters.pathVariables();
    
    QFile f( s );
    if( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return QString();
    QString data = QTextCodec::codecForName( codec.toUtf8() )->toUnicode( f.readAll() );
    
    QVector<QString> temp_v = data.split( '\n' ).toVector();
    QVector<QString> v;
    foreach(QString b,temp_v)
    {// used to trim the vector's data
        v.append(b.trimmed());
    }
    QStack<bool> isNested;
    QStack<QString> pile;
    QString file;
    QString inVarComment;
    int nbEmptyLine = 0;
    
    QRegExp Variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([*+-]?=)[ \\t]*((?:\\\\'|\\\\\\$|\\\\\\\\\\\\\\\"|\\\\\\\"|[^#])+)?[ \\t]*(#.*)?");
    QRegExp RegexVar("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*~=([^#]+)(#.*)?");
    //QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[ \\t]*[:|][ \\t]*)+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?");
    QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?))[:]*[ \\t]*(\\{)[ \\t]*(#.*)?");
    QRegExp function_call("^((?:!?[-\\.a-zA-Z0-9*!_|+]+(?:[ \\t]*\\((?:.*)\\))?[ \\t]*[|:][ \\t]*)+)?([a-zA-Z]+[ \\t]*\\((.*)\\))[ \\t]*(#.*)?");
    QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
    QRegExp end_bloc_continuing("^(\\})[ \\t]*(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\'|\\\\\\$|\\\\\\\\\\\\\\\"|\\\\\\\"|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
    QRegExp comments("^\\s*#(.*)");
    QRegExp varLine("^\\s*[^}]?(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
    
    file.append( QString( "<!DOCTYPE XUPProject>\n<project name=\"%1\" version=\"%2\" expanded=\"false\">\n" ).arg( QFileInfo( s ).fileName() ).arg( GENERATED_XUP_VERSION ) );
    try
    {
        for(int i = 0;i < v.size();i++)
        {
            if(bloc.exactMatch(v[i]))
            {
                QString tmp_end;
                QStringList liste = bloc.capturedTexts();
                if(liste[1] == "}")
                {
                    while(!isNested.isEmpty() && isNested.top())
                    {
                        file.append(pile.pop());
                        isNested.pop();
                    }
                    // pop the last scope of the bloc
                    file.append(pile.pop());
                    isNested.pop();
                    // pop the nested scope of the bloc
                    while(!isNested.isEmpty() && isNested.top())
                    {
                        file.append(pile.pop());
                        isNested.pop();
                    }
                }
                if(liste[0].trimmed()[0] == '}')
                    liste[0] = liste[0].trimmed().right(liste[0].trimmed().length()-2);
                liste[0] = liste[0].left(liste[0].lastIndexOf(QChar('{'))+1);
                QStringList liste2 = liste[0].split(QChar(':'),QString::SkipEmptyParts);
                
                foreach(QString s,liste2)
                {
                    if(s[s.length()-1] == '{')
                    {
                        file.append("<scope name=\""+escape(s.left(s.length()-1).trimmed())+"\""+(liste[5].trimmed() != "" ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">\n");
                    }
                    else
                    {
                        file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
                        tmp_end += "</scope>\n";
                    }
                }
                if(tmp_end != "")
                {
                    pile += tmp_end;
                    isNested.push(true);
                }
                pile += "</scope>\n";
                isNested.push(false);
            }
            else if(RegexVar.exactMatch(v[i]))
            {
                QStringList liste = RegexVar.capturedTexts();
                // liste[1] = scopes
                // liste[2] = la variable
                // liste[3] = la ligne (ne pas oublier trimmed())
                // liste[4] = le commentaire
                file.append("<comment value=\"#"+escape(liste[2])+"\" />\n");
            }
            else if(Variable.exactMatch(v[i]))
            {
                QString tmp_end;
                QStringList liste = Variable.capturedTexts();
                QStringList liste2 = liste[1].trimmed().split(QChar(':'), QString::SkipEmptyParts);
                if(liste[1] == "else" || (liste2.size() > 0 && liste2[0] == "else"))
                {
                    if(isNested.size())
                    {
                        // pop the last scope of the bloc
                        file.append(pile.pop());
                        isNested.pop();
                        // pop the nested scope of the bloc
                        while(!isNested.isEmpty() && isNested.top())
                        {
                            file.append(pile.pop());
                            isNested.pop();
                        }
                    }
                }
                else
                {
                    while(!isNested.isEmpty() && isNested.top())
                    {
                        file.append(pile.pop());
                        isNested.pop();
                    }
                }
                if(liste2.size() > 0)
                {
                    foreach(QString s,liste2)
                    {
                        file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
                        pile += "</scope>\n";
                        isNested.push(true);
                    }
                }
                
                bool isMulti = false;
                
                if ( liste[4].trimmed() == "\\" )
                {
                    const bool hasNextLine = i +1 < v.size();
                    isMulti = hasNextLine ? varLine.exactMatch( v[i +1] ) : true;
                    
                    // a multine variable composed of 2 lines so the next is not a varLine
                    if ( !isMulti && hasNextLine
                        && !Variable.exactMatch( v[i +1] )
                        && !RegexVar.exactMatch( v[i +1] )
                        && !bloc.exactMatch( v[i +1] )
                        && !function_call.exactMatch( v[i +1] )
                        && !end_bloc.exactMatch( v[i +1] )
                        && !end_bloc_continuing.exactMatch( v[i +1] )
                        && !comments.exactMatch( v[i +1] )
                    ) {
                        isMulti = true;
                    }
                    
                    if ( isMulti ) {
                        liste[4].clear();
                    }
                    else {
                        liste[4] = "\\";
                    }
                }
                else if(liste[4].trimmed().endsWith("\\"))
                {
                    isMulti = true;
                    QString tmppp = liste[4].trimmed();
                    tmppp.chop(1);
                    liste[4] = tmppp;
                }
                QString theOp = (liste[3].trimmed() == "=" ? "" : " operator=\""+liste[3].trimmed()+"\"");
                file.append("<variable name=\""+escape(liste[2].trimmed())+"\""+theOp+">\n");
                bool isFile = fileVariables.contains(liste[2].trimmed());
                bool isPath = pathVariables.contains(liste[2].trimmed());
                if ( isFile || isPath )
                {
                    QStringList tmpValues = liste[4].trimmed().split(" ");
                    bool inStr = false;
                    QStringList multivalues;
                    QString ajout;
                    for(int ku = 0;ku < tmpValues.size();ku++)
                    {
                        if(tmpValues.value(ku).startsWith('"') )
                            inStr = true;
                        if(inStr)
                        {
                            if(ajout != "")
                                ajout += " ";
                            ajout += tmpValues.value(ku);
                            if(tmpValues.value(ku).endsWith('"') )
                            {
                                multivalues += ajout;
                                ajout = "";
                                inStr = false;
                            }
                        }
                        else
                        {
                            multivalues += tmpValues.value(ku);
                        }
                    }
                    for(int ku = 0;ku < multivalues.size();ku++)
                    {
                        inVarComment = multivalues.value(ku).trimmed();
                        if ( inVarComment.startsWith( "#" ) )
                        {
                            if ( inVarComment == "#" && ku < multivalues.size() )
                            {
                                ku++;
                                inVarComment = "# " +multivalues.value(ku).trimmed();
                            }
                            file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
                        }
                        else
                        {
                            if ( isFile )
                            {
                                file.append("<file"+(liste[5].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
                            }
                            else if ( isPath )
                            {
                                file.append("<path"+(liste[5].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
                            }
                        }
                    }
                }
                else
                    file.append("<value"+(liste[5].trimmed() != "" ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(liste[4].trimmed())+"</value>\n");
                if(isMulti)
                {
                    i++;
                    while(varLine.exactMatch(v[i]))
                    {
                        QStringList liste3 = varLine.capturedTexts();
                        bool isFile = fileVariables.contains(liste[2].trimmed());
                        bool isPath = pathVariables.contains(liste[2].trimmed());
                        if ( isFile || isPath )
                        {
                            QStringList tmpValues = liste3[1].trimmed().split(" ");
                            QStringList multivalues = QStringList();
                            QString ajout = "";
                            bool inStr = false;
                            for(int ku = 0;ku < tmpValues.size();ku++)
                            {
                                if(tmpValues.value(ku).startsWith('"') )
                                    inStr = true;
                                if(inStr)
                                {
                                    if(ajout != "")
                                        ajout += " ";
                                    ajout += tmpValues.value(ku);
                                    if(tmpValues.value(ku).endsWith('"') )
                                    {
                                        multivalues += ajout;
                                        ajout = "";
                                        inStr = false;
                                    }
                                }
                                else
                                {
                                    multivalues += tmpValues.value(ku);
                                }
                            }
                            for(int ku = 0;ku < multivalues.size();ku++)
                            {
                                inVarComment = multivalues.value(ku).trimmed();
                                if ( inVarComment.startsWith( "#" ) )
                                {
                                    if ( inVarComment == "#" && ku < multivalues.size() )
                                    {
                                        ku++;
                                        inVarComment = "# " +multivalues.value(ku).trimmed();
                                    }
                                    file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
                                }
                                else
                                {
                                    if ( isFile )
                                    {
                                        file.append("<file"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
                                    }
                                    else if ( isPath )
                                    {
                                        file.append("<path"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
                                    }
                                }
                            }
                        }
                        else
                            file.append("<value"+(liste3[2].trimmed() != "" ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(liste3[1].trimmed())+"</value>\n");
                        i++;
                    }
                    QStringList liste3 = v[i].split( "#" );
                    QString comment;
                    if(liste3.size() == 2)
                        comment = "#"+liste3[1];
                    bool isFile = fileVariables.contains(liste[2].trimmed());
                    bool isPath = pathVariables.contains(liste[2].trimmed());
                    if ( isFile || isPath )
                    {
                        QStringList tmpValues = liste3[0].trimmed().split(" ");
                        QStringList multivalues = QStringList();
                        QString ajout = "";
                        bool inStr = false;
                        for(int ku = 0;ku < tmpValues.size();ku++)
                        {
                            if(tmpValues.value(ku).startsWith('"') )
                                inStr = true;
                            if(inStr)
                            {
                                if(ajout != "")
                                    ajout += " ";
                                ajout += tmpValues.value(ku);
                                if(tmpValues.value(ku).endsWith('"') )
                                {
                                    multivalues += ajout;
                                    ajout = "";
                                    inStr = false;
                                }
                            }
                            else
                            {
                                multivalues += tmpValues.value(ku);
                            }
                        }
                        for(int ku = 0;ku < multivalues.size();ku++)
                        {
                            inVarComment = multivalues.value(ku).trimmed();
                            if ( inVarComment.startsWith( "#" ) )
                            {
                                if ( inVarComment == "#" && ku < multivalues.size() )
                                {
                                    ku++;
                                    inVarComment = "# " +escape(multivalues.value(ku).trimmed());
                                }
                                file.append( QString( "<comment content=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
                            }
                            else
                            {
                                if ( isFile )
                                {
                                    file.append("<file"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
                                }
                                else if ( isPath )
                                {
                                    file.append("<path"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
                                }
                            }
                        }
                    }
                    else
                        file.append("<value"+(comment.trimmed() != "" ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(liste3[0].trimmed())+"</value>\n");
                }
                file.append("</variable>\n");
            }
            else if(function_call.exactMatch(v[i]))
            {
                QString tmp_end;
                QStringList liste = function_call.capturedTexts();
                QStringList liste2 = liste[1].split(QChar(':'),QString::SkipEmptyParts);
                foreach(QString s,liste2)
                {
                    file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
                    tmp_end += "</scope>\n";
                }
                QStringList explode_params = liste[2].split("(");
                QString func_name = explode_params[0].trimmed();
                QString params;
                if(explode_params.size() > 2)
                    params = explode_params.join("(");
                else
                    params = explode_params[1];
                params = params.trimmed(); // to be sure that the last char is the last ")"
                params.chop(1); // pop the last ")"
                params = params.trimmed(); // to pop off the ending spaces
                file.append("<function"+(liste[4].trimmed() != "" ? " comment=\""+escape(liste[4].trimmed())+"\"" : "")+" name=\""+escape(func_name)+"\" parameters=\""+escape(params)+"\" />\n");
                file.append(tmp_end);
            }
            else if(end_bloc_continuing.exactMatch(v[i]))
            {
                QStringList liste = end_bloc_continuing.capturedTexts();
                while(!isNested.isEmpty() && isNested.top())
                {
                    file.append(pile.pop());
                    isNested.pop();
                }
                // pop the last scope of the bloc
                file.append(pile.pop());
                isNested.pop();
                // pop the nested scope of the bloc
                while(!isNested.isEmpty() && isNested.top())
                {
                    file.append(pile.pop());
                    isNested.pop();
                }
                
                QStringList liste2 = liste[2].split(QChar(':'),QString::SkipEmptyParts);
                foreach(QString s,liste2)
                {
                    file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
                    pile += "</scope>\n";
                    isNested.push(true);
                }
                bool isMulti = (liste[6].trimmed() == "\\" ? true : false);
                const bool hasNextLine = i +1 < v.size();
                isMulti = isMulti && hasNextLine ? varLine.exactMatch(v[i +1]) : isMulti;
                
                // a multine variable composed of 2 lines so the next is not a varLine
                if ( !isMulti && hasNextLine
                    && !Variable.exactMatch( v[i +1] )
                    && !RegexVar.exactMatch( v[i +1] )
                    && !bloc.exactMatch( v[i +1] )
                    && !function_call.exactMatch( v[i +1] )
                    && !end_bloc.exactMatch( v[i +1] )
                    && !end_bloc_continuing.exactMatch( v[i +1] )
                    && !comments.exactMatch( v[i +1] )
                ) {
                    isMulti = true;
                }
                
                QString theOp = (liste[4].trimmed() == "=" ? "" : " operator=\""+liste[4].trimmed()+"\"");
                file.append("<variable name=\""+escape(liste[3].trimmed())+"\""+theOp+">\n");
                
                if ( !isMulti && liste[5].trimmed().isEmpty() && liste[6].trimmed() == "\\" ) {
                    liste[5] = "\\";
                }
                
                if ( liste[7].trimmed().startsWith( "#" ) )
                    file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( liste[7].trimmed() ) ) );
                else
                    file.append("<value"+(liste[7].trimmed() != "" ? " comment=\""+escape(liste[7].trimmed())+"\"" : "")+">"+escape(liste[5].trimmed())+"</value>\n");
                if(isMulti)
                {
                    i++;
                    while(varLine.exactMatch(v[i]))
                    {
                        QStringList liste3 = varLine.capturedTexts();
                        bool isFile = fileVariables.contains(liste[2].trimmed());
                        bool isPath = pathVariables.contains(liste[2].trimmed());
                        if ( isFile || isPath )
                        {
                            QStringList tmpValues = liste3[1].trimmed().split(" ");
                            QStringList multivalues = QStringList();
                            QString ajout = "";
                            bool inStr = false;
                            for(int ku = 0;ku < tmpValues.size();ku++)
                            {
                                if(tmpValues.value(ku).startsWith('"') )
                                    inStr = true;
                                if(inStr)
                                {
                                    if(ajout != "")
                                        ajout += " ";
                                    ajout += tmpValues.value(ku);
                                    if(tmpValues.value(ku).endsWith('"') )
                                    {
                                        multivalues += ajout;
                                        ajout = "";
                                        inStr = false;
                                    }
                                }
                                else
                                {
                                    multivalues += tmpValues.value(ku);
                                }
                            }
                            for(int ku = 0;ku < multivalues.size();ku++)
                            {
                                inVarComment = multivalues.value(ku).trimmed();
                                if ( inVarComment.startsWith( "#" ) )
                                {
                                    if ( inVarComment == "#" && ku < multivalues.size() )
                                    {
                                        ku++;
                                        inVarComment = "# " +multivalues.value(ku).trimmed();
                                    }
                                    file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
                                }
                                else
                                {
                                    if ( isFile )
                                    {
                                        file.append("<file"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
                                    }
                                    else if ( isPath )
                                    {
                                        file.append("<path"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
                                    }
                                }
                            }
                        }
                        else
                            file.append("<value"+(liste3[2].trimmed() != "" ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(liste3[1].trimmed())+"</value>\n");
                        i++;
                    }
                    QStringList liste3 = v[i].split( "#" );
                    QString comment;
                    if(liste3.size() == 2)
                        comment = "#"+liste3[1];
                    bool isFile = fileVariables.contains(liste[2].trimmed());
                    bool isPath = pathVariables.contains(liste[2].trimmed());
                    if ( isFile || isPath )
                    {
                        QStringList tmpValues = liste3[0].trimmed().split(" ");
                        QStringList multivalues = QStringList();
                        QString ajout = "";
                        bool inStr = false;
                        for(int ku = 0;ku < tmpValues.size();ku++)
                        {
                            if(tmpValues.value(ku).startsWith('"') )
                                inStr = true;
                            if(inStr)
                            {
                                if(ajout != "")
                                    ajout += " ";
                                ajout += tmpValues.value(ku);
                                if(tmpValues.value(ku).endsWith('"') )
                                {
                                    multivalues += ajout;
                                    ajout = "";
                                    inStr = false;
                                }
                            }
                            else
                            {
                                multivalues += tmpValues.value(ku);
                            }
                        }
                        for(int ku = 0;ku < multivalues.size();ku++)
                        {
                            inVarComment = multivalues.value(ku).trimmed();
                            if ( inVarComment.startsWith( "#" ) )
                            {
                                if ( inVarComment == "#" && ku < multivalues.size() )
                                {
                                    ku++;
                                    inVarComment = "# " +escape(multivalues.value(ku).trimmed());
                                }
                                file.append( QString( "<comment content=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
                            }
                            else
                            {
                                if ( isFile )
                                {
                                    file.append("<file"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
                                }
                                else if ( isPath )
                                {
                                    file.append("<path"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
                                }
                            }
                        }
                    }
                    else
                        file.append("<value"+(comment.trimmed() != "" ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(liste3[0].trimmed())+"</value>\n");
                }
                file.append("</variable>\n");
            }
            else if(end_bloc.exactMatch(v[i]))
            {
                while(!isNested.isEmpty() && isNested.top())
                {
                    isNested.top();
                    file.append(pile.pop());
                    isNested.pop();
                }
                isNested.top();
                file.append(pile.pop());
                isNested.pop();
                if(!isNested.isEmpty() && isNested.top())
                {
                    isNested.top();
                    file.append(pile.pop());
                    isNested.pop();
                }
            }
            else if(comments.exactMatch(v[i]))
            {
                QStringList liste = comments.capturedTexts();
                file.append("<comment value=\"#"+escape(liste[1])+"\" />\n");
            }
            else if(v[i] == "")
            {
                nbEmptyLine++;
                while(!isNested.isEmpty() && isNested.top())
                {
                    file.append(pile.pop());
                    isNested.pop();
                }
                if(i+1 < v.size())
                {
                    if(v[i+1] != "")
                    {
                        file.append("<emptyline count=\""+QString::number(nbEmptyLine)+"\" />\n");
                        nbEmptyLine = 0;
                    }
                }
                else
                    file.append("<emptyline count=\""+QString::number(nbEmptyLine)+"\" />\n");
            }
            else
            {
                qWarning( "%s didn't match", v[i].toLocal8Bit().constData());
                throw MksException( QString("Erreur parsing project file: %1").arg( s ) );
            }
        }
        while(!isNested.isEmpty() && isNested.top())
        {
            file.append(pile.pop());
            isNested.pop();
        }
    }
    catch(const std::exception & e)
    {
        // re-init the XML output
        file.append( QString( "<!DOCTYPE XUPProject>\n<project name=\"%1\" version=\"%2\" expanded=\"false\">\n" ).arg( QFileInfo( s ).fileName() ).arg( GENERATED_XUP_VERSION ) );
        // empty both stacks
        isNested.clear();
        pile.clear();
        qWarning("%s", e.what() );
    }
    
    file.append( "</project>\n" );
    // to output the xml in a file
    /*QFile apt(s+".xml");
    if( apt.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        apt.write(file.toAscii());
        apt.close();
    }*/
    
    return file;
}

QString QMake2XUP::convertToPro( const QDomDocument& document )
{
    // get project node
    QDomElement element  = document.firstChildElement( "project" ).toElement();
    
    // check project available
    if ( element.isNull() ) {
        return QString::null;
    }
    
    // parse project scope
    QString contents = convertNodeToPro( element );
    
    // remove last eol
    if ( contents.length() > 0 ) {
        contents.chop( 1 );
    }
    
    // return buffer
    return contents;
}

QString QMake2XUP::escape( const QString& string )
{
    return Qt::escape( string ).replace( "\"" , "&quot;" );
}

QString QMake2XUP::convertNodeToPro( const QDomNode& node, int weight, bool multiline, bool nested, const QString& EOL )
{
    const QStringList notContainers = QStringList()
        << "comment"
        << "emptyline"
        << "value"
        << "file"
        << "path"
        ;
    QString data; // the data to return
    
    if ( !isProject( node ) ) {
        if ( isComment( node ) ) {
            const QString cmt = nodeAttribute( node, "value" );
            int cweight = weight;
            
            if ( multiline ) {
                cweight++;
            }
            
            data.append( tabbedString( cweight, cmt, EOL ) );
        }
        else if ( isEmptyLine( node ) ) {
            const int count = node.attributes().namedItem( "count" ).nodeValue().toInt();
            for ( int i = 0; i < count; i++ ) {
                data.append( EOL );
            }
        }
        else if ( isVariable( node ) ) {
            const QString variable = QString( "%1 %2 " )
                .arg( nodeAttribute( node, "name" ) )
                .arg( nodeAttribute( node, "operator", "=" ) )
                ;
            multiline = isMultiline( node );
            int vweight = weight;
            
            if ( isBlock( node.parentNode() ) && nested ) {
                vweight = 0;
            }
            
            data.append( tabbedString( vweight, variable ) );
        }
        else if ( isValue( node ) ) {
            const QString value = node.toElement().text();
            const QString comment = nodeAttribute( node, "comment" );
            int vweight = weight;
            
            if ( node.previousSibling().isNull() || !multiline ) {
                vweight = 0;
            }
            else if ( multiline ) {
                vweight++;
            }
            
            data.append( tabbedString( vweight, value ) );
            
            if ( multiline ) {
                if ( !isLastValue( node ) ) {
                    data.append( " \\" );
                }
            }
            
            if ( multiline || node.nextSibling().isNull() ) {
                if ( !comment.isEmpty() ) {
                    data.append( ' ' +comment );
                }
                
                data.append( EOL );
            }
            else {
                data.append( ' ' );
            }
        }
        else if ( isBlock( node ) ) {
            const QString content = node.nodeName().compare( "function", Qt::CaseInsensitive ) == 0
                ? QString( "%1( %2 )" ).arg( nodeAttribute( node, "name" ) ).arg( nodeAttribute( node, "parameters" ) )
                : nodeAttribute( node, "name" );
            const QString comment = nodeAttribute( node, "comment" );
            int bweight = weight;
            
            if ( isBlock( node.parentNode() ) && nested ) {
                bweight = 0;
            }
            
            nested = isNested( node );
            
            data.append( tabbedString( bweight, content ) );
            
            if ( nested ) {
                data.append( ':' );
            }
            else {
                //if ( node.hasChildNodes() ) {
                    data.append( " {" );
                    weight++;
                //}
                
                if ( !comment.isEmpty() ) {
                    data.append( ' ' +comment );
                }
                
                data.append( EOL );
            }
        }
    }
    
    if ( node.hasChildNodes() && !notContainers.contains( node.nodeName() ) ) {
        QDomNodeList nodes = node.childNodes();
        
        for ( int i = 0; i < nodes.count(); i++ ) {
            data.append( convertNodeToPro( nodes.at( i ), weight, multiline, nested, EOL ) );
        }
    }
    
    if ( isBlock( node ) && !nested ) {
        const QString comment = nodeAttribute( node, "closing-comment" );
        const QDomNode sibling = node.nextSibling();
        
        //if ( node.hasChildNodes() ) {
            weight--;
            data.append( tabbedString( weight, "}" ) );
        //}
        
        if ( !( isBlock( sibling ) && ( nodeAttribute( sibling, "name" ).compare( "else", Qt::CaseInsensitive ) == 0 ) ) ) {
            if ( node.hasChildNodes() ) {
                if ( !comment.isEmpty() ) {
                    data.append( ' ' +comment );
                }
                
                data.append( EOL );
            }
        }
        else {
            data.append( ' ' );
        }
    }

    return data;
}

QString QMake2XUP::tabbedString( int weight, const QString& string, const QString& eol )
{
    return QString( weight *4, ' ' ).append( string ).append( eol );
}

QString QMake2XUP::nodeAttribute( const QDomNode& node, const QString& attribute, const QString& defaultValue )
{
    QString value = node.attributes().namedItem( attribute ).nodeValue();
    
    if ( value.isEmpty() ) {
        value = defaultValue;
    }
    
    return value;
}

bool QMake2XUP::isMultiline( const QDomNode& node )
{
    return node.childNodes().count() > 1;
}

bool QMake2XUP::isNested( const QDomNode& node )
{
    QString string = node.attributes().namedItem( "nested" ).nodeValue();
    
    if ( string.isEmpty() ) {
        string = "false";
    }
    
    bool nested = QVariant( string ).toBool();
    
    if ( nested && node.childNodes().count() > 1 ) {
        nested = false;
    }
    
    return nested;
}

bool QMake2XUP::isBlock( const QDomNode& node )
{
    const QString name = node.nodeName();
    
    return name.compare( "function", Qt::CaseInsensitive ) == 0
        || name.compare( "scope", Qt::CaseInsensitive ) == 0
        ;
}

bool QMake2XUP::isProject( const QDomNode& node )
{
    return node.nodeName().compare( "project", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isComment( const QDomNode& node )
{
    return node.nodeName().compare( "comment", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isEmptyLine( const QDomNode& node )
{
    return node.nodeName().compare( "emptyline", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isVariable( const QDomNode& node )
{
    return node.nodeName().compare( "variable", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isValue( const QDomNode& node )
{
    const QString name = node.nodeName();
    
    return name.compare( "value", Qt::CaseInsensitive ) == 0
        || name.compare( "file", Qt::CaseInsensitive ) == 0
        || name.compare( "path", Qt::CaseInsensitive ) == 0
        ;
}

bool QMake2XUP::isLastValue( const QDomNode& node )
{
    QDomNode sibling = node;
    
    while ( !( sibling = sibling.nextSibling() ).isNull() ) {
        if ( isValue( sibling ) ) {
            return false;
        }
    }
    
    return true;
}
