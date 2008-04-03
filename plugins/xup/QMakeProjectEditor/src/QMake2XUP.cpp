#include "QMake2XUP.h"
#include "QMakeXUPItem.h"

/**************************
WARNING :
si "operator" n'existe pas, il vaut "="
si "multiline" n'existe pas, il vaut "false"
si "nested" n'existe pas, il vaut "false"
**************************/

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtXml>

#include <exception>

#include <monkey.h>

QString tabsString( int i )
{ return QString().fill( '\t', i ); }

QString MyEscape( QString b )
{ return Qt::escape( b ).replace( "\"" , "&quot;" ); }

QByteArray QMake2XUP::convertFromPro( const QString& s, const QString& version )
{
	// check if file exists
	if ( !QFile::exists( s ) )
		return QByteArray();
	
	QFile f( s );
	if( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return QByteArray();
	QByteArray data = f.readAll();
	
	QVector<QByteArray> temp_v = data.split( '\n' ).toVector();
	QVector<QString> v;
	foreach(QByteArray b,temp_v)
	{// used to trim the vector's data
		v.append(QString(b).trimmed());
	}
	QStack<bool> isNested;
	QStack<QString> pile;
	QString file;
	QString inVarComment;
	int nbEmptyLine = 0;
	
	QRegExp Variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\\\\\\\\\\\\"|\\\\\\\"|\\\\.|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
	//QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[ \\t]*[:|][ \\t]*)+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?))[:]*[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp function_call("^((?:!?[a-zA-Z0-9\\.]+(?:[ \\t]*\\((?:.*)\\))?[ \\t]*[|:][ \\t]*)+)?([a-zA-Z]+[ \\t]*\\((.*)\\))[ \\t]*(#.*)?");
	QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
	QRegExp end_bloc_continuing("^(\\})[ \\t]*(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\\\\\\\\\\\\"|\\\\\\\"|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
	QRegExp comments("^#(.*)");
	QRegExp varLine("^(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
	
	QMakeXUPItem xi;
	const QStringList fileVariables = xi.fileVariables();
	const QStringList pathVariables = xi.pathVariables();
	
	file.append( QString( "<!DOCTYPE XUPProject>\n<project version=\"%1\" name=\"%2\" expanded=\"false\">\n" ).arg( version ).arg( QFileInfo( s ).fileName() ) );
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
						file.append(pile.pop().toUtf8());
						isNested.pop();
					}
					// pop the last scope of the bloc
					file.append(pile.pop().toUtf8());
					isNested.pop();
					// pop the nested scope of the bloc
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop().toUtf8());
						isNested.pop();
					}
				}
				if(liste[0].trimmed()[0] == '}')
					liste[0] = liste[0].trimmed().right(liste[0].trimmed().length()-2);
				liste[0] = liste[0].left(liste[0].indexOf(QChar('{'))+1);
				QStringList liste2 = liste[0].split(QChar(':'),QString::SkipEmptyParts);
				
				foreach(QString s,liste2)
				{
					if(s[s.length()-1] == '{')
					{
						file.append("<scope name=\""+MyEscape(s.left(s.length()-1).trimmed().toUtf8())+"\""+(liste[5].trimmed().toUtf8() != "" ? " comment=\""+MyEscape(liste[5].trimmed().toUtf8())+"\"" : "")+">\n");
					}
					else
					{
						file.append("<scope name=\""+MyEscape(s.trimmed().toUtf8())+"\" nested=\"true\">\n");
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
			else if(Variable.exactMatch(v[i]))
			{
				QString tmp_end;
				QStringList liste = Variable.capturedTexts();
				QStringList liste2 = liste[1].trimmed().split(QChar(':'),QString::SkipEmptyParts);
				if(liste[1] == "else" || (liste2.size() > 0 && liste2[0] == "else"))
				{
					// pop the last scope of the bloc
					file.append(pile.pop().toUtf8());
					isNested.pop();
					// pop the nested scope of the bloc
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop().toUtf8());
						isNested.pop();
					}
				}
				else
				{
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop().toUtf8());
						isNested.pop();
					}
				}
				if(liste2.size() > 0)
				{
					foreach(QString s,liste2)
					{
						file.append("<scope name=\""+MyEscape(s.trimmed().toUtf8())+"\" nested=\"true\">\n");
						pile += "</scope>\n";
						isNested.push(true);
					}
				}
				QByteArray isMulti = (liste[5].trimmed() == "\\" ? " multiline=\"true\"" : "");
				QByteArray theOp = (liste[3].trimmed().toUtf8() == "=" ? "" : " operator=\""+liste[3].trimmed().toUtf8()+"\"");
				file.append("<variable name=\""+MyEscape(liste[2].trimmed().toUtf8())+"\""+theOp+isMulti+">\n");
				
				if(fileVariables.contains(liste[2].trimmed().toUtf8()) || pathVariables.contains(liste[2].trimmed().toUtf8()))
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
						inVarComment = multivalues.value(ku).toUtf8().trimmed();
						if ( inVarComment.startsWith( "#" ) )
						{
							if ( inVarComment == "#" && ku < multivalues.size() )
							{
								ku++;
								inVarComment = "# " +multivalues.value(ku).trimmed();
							}
							file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
						}
						else
							file.append("<value"+(liste[6].trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+MyEscape(liste[6].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(multivalues.value(ku).toUtf8())+"</value>\n");
					}
				}
				else
					file.append("<value"+(liste[6].trimmed().toUtf8() != "" ? " comment=\""+MyEscape(liste[6].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(liste[4].trimmed().toUtf8())+"</value>\n");
				
				if(isMulti == " multiline=\"true\"")
				{
					i++;
					while(varLine.exactMatch(v[i]))
					{
						QStringList liste3 = varLine.capturedTexts();
						if(fileVariables.contains(liste[2].trimmed().toUtf8()) || pathVariables.contains(liste[2].trimmed().toUtf8()))
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
								inVarComment = multivalues.value(ku).toUtf8().trimmed();
								if ( inVarComment.startsWith( "#" ) )
								{
									if ( inVarComment == "#" && ku < multivalues.size() )
									{
										ku++;
										inVarComment = "# " +multivalues.value(ku).trimmed();
									}
									file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
								}
								else
									file.append("<value"+(liste3[2].trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+MyEscape(liste3[2].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(multivalues.value(ku).toUtf8())+"</value>\n");
							}
						}
						else
							file.append("<value"+(liste3[2].trimmed().toUtf8() != "" ? " comment=\""+MyEscape(liste3[2].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(liste3[1].trimmed().toUtf8())+"</value>\n");
						i++;
					}
					QStringList liste3 = v[i].split( "#" );
					QString comment;
					if(liste3.size() == 2)
						comment = "#"+liste3[1];
					if(fileVariables.contains(liste[2].trimmed().toUtf8()) || pathVariables.contains(liste[2].trimmed().toUtf8()))
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
							inVarComment = multivalues.value(ku).toUtf8().trimmed();
							if ( inVarComment.startsWith( "#" ) )
							{
								if ( inVarComment == "#" && ku < multivalues.size() )
								{
									ku++;
									inVarComment = "# " +MyEscape(multivalues.value(ku).trimmed());
								}
								file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
							}
							else
								file.append("<value"+(comment.trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+MyEscape(comment.trimmed().toUtf8())+"\"" : "")+">"+MyEscape(multivalues.value(ku).toUtf8())+"</value>\n");
						}
					}
					else
						file.append("<value"+(comment.trimmed().toUtf8() != "" ? " comment=\""+MyEscape(comment.trimmed().toUtf8())+"\"" : "")+">"+MyEscape(liste3[0].trimmed().toUtf8())+"</value>\n");
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
					file.append("<scope name=\""+MyEscape(s.trimmed().toUtf8())+"\" nested=\"true\">\n");
					tmp_end += "</scope>\n";
				}
				file.append("<function"+(liste[4].trimmed() != "" ? " comment=\""+MyEscape(liste[4].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(liste[2].trimmed().toUtf8())+"</function>\n");
				file.append(tmp_end.toUtf8());
			}
			else if(end_bloc_continuing.exactMatch(v[i]))
			{
				QStringList liste = end_bloc_continuing.capturedTexts();
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
				// pop the last scope of the bloc
				file.append(pile.pop().toUtf8());
				isNested.pop();
				// pop the nested scope of the bloc
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
				
				QStringList liste2 = liste[2].split(QChar(':'),QString::SkipEmptyParts);
				foreach(QString s,liste2)
				{
					file.append("<scope name=\""+MyEscape(s.trimmed().toUtf8())+"\" nested=\"true\">\n");
					pile += "</scope>\n";
					isNested.push(true);
				}
				QByteArray theOp = (liste[4].trimmed().toUtf8() == "=" ? "" : " operator=\""+liste[4].trimmed().toUtf8()+"\"");
				file.append("<variable name=\""+MyEscape(liste[3].trimmed().toUtf8())+"\""+theOp+">\n");
				if ( liste[7].trimmed().toUtf8().startsWith( "#" ) )
					file.append( QString( "<comment>%1</comment>" ).arg( QString( liste[7].trimmed().toUtf8() ) ) );
				else
					file.append("<value"+(liste[7].trimmed().toUtf8() != "" ? " comment=\""+MyEscape(liste[7].trimmed().toUtf8())+"\"" : "")+">"+MyEscape(liste[5].trimmed().toUtf8())+"</value>\n");
				file.append("</variable>\n");
			}
			else if(end_bloc.exactMatch(v[i]))
			{
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
				file.append(pile.pop().toUtf8());
				isNested.pop();
				if(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
			}
			else if(comments.exactMatch(v[i]))
			{
				QStringList liste = comments.capturedTexts();
				file.append("<comment>#"+MyEscape(liste[1].toUtf8())+"</comment>\n");
			}
			else if(v[i] == "")
			{
				nbEmptyLine++;
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
				if(i+1 < v.size())
				{
					if(v[i+1] != "")
					{
						file.append("<emptyline>"+QString::number(nbEmptyLine)+"</emptyline>\n");
						nbEmptyLine = 0;
					}
				}
				else
					file.append("<emptyline>"+QString::number(nbEmptyLine)+"</emptyline>\n");
			}
			else
			{
				qWarning("%s didn't match", qPrintable(v[i]));
				qWarning("Variable matched length : %d", Variable.matchedLength());
				qWarning("Bloc matched length : %d", bloc.matchedLength());
				qWarning("function call matched length : %d", function_call.matchedLength());
				throw QString( "Erreur parsing project file: %1" ).arg( s );
			}
		}
		while(!isNested.isEmpty() && isNested.top())
		{
			file.append(pile.pop().toUtf8());
			isNested.pop();
		}
	}
	catch ( const QString& s )
	{
		// re-init the XML output
		file.append( QString( "<!DOCTYPE XUPProject>\n<project version=\"%1\" name=\"%2\" expanded=\"false\">\n" ).arg( version ).arg( QFileInfo( s ).fileName() ) );
		// empty both stacks
		isNested.clear();
		pile.pop();
		qWarning( qPrintable( s ) );
	}
	
	file.append( "</project>\n" );
	// to output the xml in a file
	/*QFile apt("debug.xml");
	if( apt.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		apt.write(file.toUtf8());
		apt.close();
	}*/
	
	return file.toUtf8();
}

QByteArray convertNodeToPro( const QDomElement& e, const QString& v, const QString& EOL = pMonkeyStudio::getEol() )
{
	static int tabs = 0; // tabs indentation
	static bool isMultiline = false; // tell if last variable is multiline or not
	bool isNested = false; // tell if scope is nested or not
	QString comment; // comment of item if available
	QString data; // the data to return
	const QString tn = e.tagName(); // current node tag name

	if ( tn != "project" )
	{
		if ( tn == "function" )
		{
			data.append( tabsString( tabs ) +e.firstChild().toText().data() );
			comment = e.attribute( "comment" );
			if ( !comment.isEmpty() )
				data.append( ' ' +comment );
			data.append( EOL );
		}
		else if ( tn == "emptyline" )
		{
			for ( int i = 0; i < e.firstChild().toText().data().toInt(); i++ )
				data.append( EOL );
		}
		else if ( tn == "variable" )
		{
			int vtabs = tabs;
			QDomElement n = e.parentNode().toElement();
			if ( n.tagName() == "scope" && QVariant( n.attribute( "nested", "false" ) ).toBool() )
				vtabs--;
			data.append( tabsString( vtabs ) +QString( "%1\t%2 " ).arg( e.attribute( "name" ) ).arg( e.attribute( "operator", "=" ) ) );
			isMultiline = QVariant( e.attribute( "multiline", "false" ) ).toBool();
		}
		else if ( tn == "value" )
		{
			int vtabs = tabs;
			if ( !e.previousSibling().isNull() && isMultiline )
				vtabs++;
			else if ( e.previousSibling().isNull() || !isMultiline )
				vtabs = 0;
			data.append( tabsString( vtabs ) +e.firstChild().toText().data() );
			comment = e.attribute( "comment" );
			if ( isMultiline )
			{
				if ( !e.nextSibling().isNull() )
					data.append( " \\" );
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
			}
			else if ( e.nextSibling().isNull() )
			{
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
			}
			else
				data.append( ' ' );
		}
		else if ( tn == "scope" )
		{
			int vtabs = tabs;
			QDomElement pe = e.parentNode().toElement();
			if ( ( e.attribute( "name" ) == "else" && !QVariant( e.previousSibling().toElement().attribute( "nested", "false" ) ).toBool() ) ||
					( pe.tagName() == "scope" && QVariant( pe.attribute( "nested", "false" ) ).toBool() ) )
				vtabs = 0;
			isNested = QVariant( e.attribute( "nested", "false" ) ).toBool();
			comment = e.attribute( "comment" );
			data.append( tabsString( vtabs ) +e.attribute( "name" ) );
			if ( !isNested )
			{
				data.append( " {" );
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
				tabs++;
			}
			else
				data.append( ':' );
		}
		else if ( tn == "comment" )
		{
			int vtabs = tabs;
			if ( e.parentNode().toElement().tagName() == "variable" && isMultiline )
				vtabs++;
			data.append( tabsString( vtabs ) +e.firstChild().toText().data() +EOL );
		}
	}
	else
		tabs = 0;
	
	const QStringList tv = QStringList() << "function" << "emptyline" << "value" << "comment";
	if ( e.hasChildNodes() && !tv.contains( tn ) )
	{
		QDomNodeList l = e.childNodes();
		for ( int i = 0; i < l.count(); i++ )
			data.append( convertNodeToPro( l.at( i ).toElement(), v, EOL ) );
		
		if ( tn == "scope" && !isNested )
		{
			tabs--;
			data.append( tabsString( tabs ) +"}" );
			QDomElement n = e.nextSibling().toElement();
			if ( !( n.tagName() == "scope" && n.attribute( "name" ) == "else" ) )
				data.append( EOL );
			else
				data.append( ' ' );
		}
	}

	return data.toAscii();
}

QByteArray QMake2XUP::convertToPro( const QDomDocument& d, const QString& v )
{
	// get project node
	QDomElement e  = d.firstChildElement( "project" ).toElement();
	// check project available
	if ( e.isNull() )
		return QByteArray();
	// parse project scope
	QByteArray contents = convertNodeToPro( e, v );
	// remove last eol
	if ( contents.length() > 0 )
		contents.chop( 1 );
	// return buffer
	return contents;
}
