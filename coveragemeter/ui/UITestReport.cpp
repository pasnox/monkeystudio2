/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Coverage Meter
** FileName  : UITestReport.cpp
** Date      : 2008-01-14T00:35:51
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
#include "UITestReport.h"
#include "PluginsManager.h"
#include "main.h"
//
#include <QFile>
#include <QDesktopServices>
#include <stdlib.h>
#include <QDateTime>
//
QPointer<UITestReport> UITestReport::mSelf = 0L;
//
UITestReport* UITestReport::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UITestReport( p );
	return mSelf;
}
//
UITestReport::UITestReport( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	QString descriptionText = "Unavailable";
#ifdef __COVERAGESCANNER__
	QFileInfo coverageFile(BasePlugin::codeCoverageFile());
	descriptionText = "<HTML><BODY>" 
                      "<H1>Test Report Generation for MonkeyStudio</H1>"
                      "<P>"
                      "Process as follows:"
                      "<UL>"
                        "<LI>Do some test operations.</LI>"
                        "<LI>Press the <TT>PAUSE</TT> key or exit MonkeyStudio. This dialog appears.</LI>"
                        "<LI>Enter a title for your test and its state (Passed or Failed).</LI>"
                        "<LI>Press OK to save the report.</LI>"
                      "</UL>"
                      "</P><P>"
                      "The report contains code coverage data generated using "
                      "<I>CoverageMeter</I>&nbsp;(<TT>http://www.coveragemeter.com</TT>). "
                      "This report does not contain any kind of information about "
                      "the project developped using MonkeyStudio."
                      "</P><P>"
                      "When the testing phase is finished, send the contain of the directory "
                      "'<TT>"+coverageFile.dir().absolutePath()+"</TT>' "
                      "to xxxx@xxx.xx" 
                      "</P>"
                      "</BODY></HTML>";
#endif
	description->setText(descriptionText);
	//
	connect( ok, SIGNAL( clicked( ) ), this, SLOT( okClicked( ) ) );
	connect( cancel, SIGNAL( clicked( ) ), this, SLOT( cancelClicked( ) ) );
}
//
void UITestReport::okClicked( )
{
#ifdef __COVERAGESCANNER__
    QString testState;
	__coveragescanner_filename( BasePlugin::codeCoverageFile().toAscii() );
	switch ( status->currentIndex() )
    {
      case 1:
        testState = ( "PASSED" );
        break;
      case 2:
        testState = ( "FAILED" );
        break;
      default:
        testState = ( "UNKNOWN" );
        break;
    }
    QString testName=testTitle(title->text());
    QString monkeyTestName=testName+"/MonkeyStudio";
    __coveragescanner_teststate( testState.toAscii() );
	__coveragescanner_testname( monkeyTestName.toAscii() );
	__coveragescanner_save();
	for ( int i = 0; i < PluginsManager::self()->plugins().count(); i++ )
	{
      BasePlugin *plg=PluginsManager::self()->plugins().at(i);
      plg->saveCodeCoverage(testName,testState);
    }
#endif
	accept();
}

void UITestReport::cancelClicked( )
{
	reject();
}

QString UITestReport::testTitle( const QString& s ) const
{
	QString userName = QString( getenv( "USERNAME" ) );
	QString user = QString( getenv( "USER" ) );
	QString testName;
	testName = user;
	if ( !userName.isEmpty() )
		testName += "(" +userName +")";
	//
	testName += "/";
	if ( s.isEmpty() )
		testName += QString( "unnamed" );
	else
		testName += s;
    return testName;
}
