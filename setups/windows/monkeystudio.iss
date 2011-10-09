; This script need to use a special Inno Setup version located here : http://jrsoftware.org/isdl.php#qsp

[InnoIDE_Settings]
LogFileOverwrite=false

[_ISTool]
UseAbsolutePaths=false

[CustomMessages]
APP_NAME=Monkey Studio IDE
APP_COPYRIGHTS=2005 - 2011 Azevedo Filipe & The Monkey Studio Team
APP_URL=http://monkeystudio.org
APP_FORUMS_URL=http://monkeystudio.org/forum
APP_ISSUES_URL=https://bugs.launchpad.net/monkeystudio/+filebug

#define APP_VERSION GetEnv("VERSION")
#define APP_VERSION_STR GetEnv("VERSION_STR")
#define APP_REVISION GetEnv( "SVN_REVISION" )
#define APP_SETUP_NAME "setup_mks_" +APP_VERSION_STR +"-win32"
#define QT_PATH "Z:\" +GetEnv("CROSS_WIN32_QT_PATH")
#define DLLS_PATH "Z:\" +GetEnv("DLLS_PATH")

[Setup]
SourceDir=..\..\bin
OutputDir=..\setups
OutputBaseFilename={#APP_SETUP_NAME}
VersionInfoProductName={cm:APP_NAME}
VersionInfoVersion={#APP_VERSION}
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible cross-platform IDE
VersionInfoTextVersion={#APP_VERSION}
VersionInfoCopyright={cm:APP_COPYRIGHTS}
VersionInfoProductVersion={#APP_VERSION}
AppCopyright={cm:APP_COPYRIGHTS}
AppName={cm:APP_NAME}
AppVerName={cm:APP_NAME} {#APP_VERSION}
InfoAfterFile=..\dev-readme
InfoBeforeFile=..\readme.txt
LicenseFile=..\GPL-3
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\{cm:APP_NAME}
EnableDirDoesntExistWarning=false
AllowNoIcons=true
DefaultGroupName={cm:APP_NAME}
AlwaysUsePersonalGroup=true
;SetupIconFile=..\monkey\src\resources\icons\application\monkey2.ico
AppPublisher={cm:APP_COPYRIGHTS}
AppPublisherURL={cm:APP_URL}
AppSupportURL={cm:APP_FORUMS_URL}
AppVersion={#APP_VERSION}
AppComments=Thanks using {cm:APP_NAME}
AppContact={cm:APP_FORUMS_URL}
UninstallDisplayName={cm:APP_NAME}
ShowLanguageDialog=yes
UsePreviousLanguage=no

[Files]
; MkS related files
Source: monkeystudio.exe; DestDir: {app}; Flags: promptifolder
Source: ..\setups\windows\qt.conf; DestDir: {app}; Flags: promptifolder
Source: ..\GPL-2; DestDir: {app}; Flags: promptifolder
Source: ..\GPL-3; DestDir: {app}; Flags: promptifolder
Source: ..\LGPL-3; DestDir: {app}; Flags: promptifolder
Source: ..\dev-readme; DestDir: {app}; Flags: promptifolder
Source: ..\readme.txt; DestDir: {app}; Flags: promptifolder
Source: plugins\*.dll; DestDir: {app}\plugins; Flags: promptifolder recursesubdirs
Source: ..\datas\scripts\*.*; DestDir: {app}\scripts; Flags: promptifolder
Source: ..\datas\translations\*.*; DestDir: {app}\translations; Flags: promptifolder
Source: ..\datas\templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs
Source: ..\datas\apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs
; Qt related files
Source: {#QT_PATH}\bin\*.dll; DestDir: {app}; Flags: promptifolder; Excludes: *d4.dll
Source: {#QT_PATH}\doc\qch\*.qch; DestDir: {app}\qt\doc\qch; Flags: promptifolder recursesubdirs
Source: {#QT_PATH}\plugins\*.dll; DestDir: {app}\qt\plugins; Flags: promptifolder recursesubdirs; Excludes: *d4.dll
Source: {#QT_PATH}\translations\*.*; DestDir: {app}\qt\translations; Flags: promptifolder recursesubdirs; Excludes: *.pri *.pro *.qph *.pl *.xq README
; MinGW related files
Source: {#DLLS_PATH}\mingwm10.dll; DestDir: {app}; Flags: promptifolder
Source: {#DLLS_PATH}\libgcc_s_dw2-1.dll; DestDir: {app}; Flags: promptifolder skipifsourcedoesntexist
Source: {#DLLS_PATH}\libstdc++-6.dll; DestDir: {app}; Flags: promptifolder skipifsourcedoesntexist
Source: {#DLLS_PATH}\libeay32.dll; DestDir: {app}; Flags: promptifolder
Source: {#DLLS_PATH}\libssl32.dll; DestDir: {app}; Flags: promptifolder
Source: {#DLLS_PATH}\ssleay32.dll; DestDir: {app}; Flags: promptifolder

[Icons]
Name: {group}\{cm:APP_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\{cm:APP_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home page.url; WorkingDir: {app}
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}
Name: {group}\Issues Tracker; Filename: {app}\Issues tracker.url; WorkingDir: {app}
Name: {group}\{cm:UninstallProgram, {cm:APP_NAME}}; Filename: {uninstallexe}; WorkingDir: {app}
Name: {group}\Downloads Qt Libraries; Filename: {app}\Downloads Qt Libraries.url; WorkingDir: {app}
Name: {group}\Downloads MinGW; Filename: {app}\Downloads MinGW.url; WorkingDir: {app}
Name: {group}\Qt downloads page; Filename: {app}\Qt downloads page.url; WorkingDir: {app}

[INI]
Filename: {app}\Home page.url; Section: InternetShortcut; Key: URL; String: {cm:APP_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: {cm:APP_FORUMS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Issues tracker.url; Section: InternetShortcut; Key: URL; String: {cm:APP_ISSUES_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Downloads Qt Libraries.url; Section: InternetShortcut; Key: URL; String: http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.4-mingw.exe; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Languages: 
Filename: {app}\Downloads MinGW.url; Section: InternetShortcut; Key: URL; String: http://get.qt.nokia.com/misc/MinGW-gcc440_1.zip; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Qt downloads page.url; Section: InternetShortcut; Key: URL; String: http://qt.nokia.com/downloads; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty

[UninstallDelete]
Name: {app}\Home page.url; Type: files
Name: {app}\Forums.url; Type: files
Name: {app}\Issues tracker.url; Type: files
Name: {app}\*.ini; Type: files
Name: {app}\scripts-*; Type: filesandordirs
Name: {app}\*; Type: filesandordirs
Name: {app}; Type: dirifempty

[Run]
Filename: {app}\monkeystudio.exe; WorkingDir: {app}; Flags: postinstall skipifsilent; Description: {cm:APP_NAME}

[Languages]
Name: catalan; MessagesFile: compiler:Languages\Catalan.isl
Name: czech; MessagesFile: compiler:Languages\Czech.isl
Name: danish; MessagesFile: compiler:Languages\Danish.isl
Name: english; MessagesFile: compiler:Default.isl
Name: french; MessagesFile: compiler:Languages\French.isl
Name: german; MessagesFile: compiler:Languages\German.isl
Name: hebrew; MessagesFile: compiler:Languages\Hebrew.isl
Name: hungarian; MessagesFile: compiler:Languages\Hungarian.isl
Name: italian; MessagesFile: compiler:Languages\Italian.isl
Name: japanese; MessagesFile: compiler:Languages\Japanese.isl
Name: polish; MessagesFile: compiler:Languages\Polish.isl
Name: portuguese; MessagesFile: compiler:Languages\Portuguese.isl
Name: russian; MessagesFile: compiler:Languages\Russian.isl
Name: slovak; MessagesFile: compiler:Languages\Slovak.isl
Name: slovenian; MessagesFile: compiler:Languages\Slovenian.isl
Name: spanish; MessagesFile: compiler:Languages\Spanish.isl
