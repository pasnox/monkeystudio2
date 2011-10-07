; This script need to use a special Inno Setup version located here : http://jrsoftware.org/isdl.php#qsp

[CustomMessages]
MKS_NAME=Monkey Studio IDE
MKS_COPYRIGHTS=2005 - 2011 Azevedo Filipe & The Monkey Studio Team
MKS_URL=http://monkeystudio.org
MKS_FORUMS_URL=http://monkeystudio.org/forum

#define MKS_VERSION "1.9.0.0"
#define MKS_REVISION "3482"
#define MKS_SETUP_NAME "setup_mks_" +MKS_VERSION +"-svn" +MKS_REVISION +"-win32"
#define QT_PATH "C:\Development\Qt\4.5.1"

[Setup]
OutputDir=..
SourceDir=..\..\bin
OutputBaseFilename=setups\{#MKS_SETUP_NAME}
VersionInfoVersion=2.0.0.0
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible cross-platform IDE
VersionInfoTextVersion={#MKS_VERSION}
VersionInfoCopyright={cm:MKS_COPYRIGHTS}
AppCopyright={cm:MKS_COPYRIGHTS}
AppName={cm:MKS_NAME}
AppVerName={cm:MKS_NAME} {#MKS_VERSION}
InfoAfterFile=..\dev-readme
InfoBeforeFile=..\readme.txt
LicenseFile=..\GPL-3
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\{cm:MKS_NAME}
EnableDirDoesntExistWarning=true
AllowNoIcons=true
DefaultGroupName={cm:MKS_NAME}
AlwaysUsePersonalGroup=true
;SetupIconFile=..\monkey\src\resources\icons\application\monkey2.ico
AppPublisher={cm:MKS_COPYRIGHTS}
AppPublisherURL={cm:MKS_URL}
AppSupportURL={cm:MKS_FORUMS_URL}
AppVersion={#MKS_VERSION}
AppComments=Thanks using {cm:MKS_NAME}
AppContact={cm:MKS_FORUMS_URL}
UninstallDisplayName={cm:MKS_NAME}
ShowLanguageDialog=yes

[_ISTool]
UseAbsolutePaths=false

[Files]
; MkS related files
Source: monkeystudio.exe; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\setups\windows\qt.conf; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\GPL-2; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\GPL-3; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\LGPL-3; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\dev-readme; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\readme.txt; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: plugins\*.dll; DestDir: {app}\plugins; Flags: promptifolder recursesubdirs
Source: ..\datas\scripts\*.*; DestDir: {app}\scripts; Flags: promptifolder
Source: ..\datas\translations\*.*; DestDir: {app}\translations; Flags: promptifolder
Source: ..\datas\templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs
Source: ..\datas\apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs
; Qt related files
Source: {#QT_PATH}\bin\*.dll; DestDir: {app}; Flags: confirmoverwrite promptifolder; Excludes: *d4.dll
Source: {#QT_PATH}\doc\qch\*.qch; DestDir: {app}\qt\doc\qch; Flags: promptifolder recursesubdirs
Source: {#QT_PATH}\plugins\*.dll; DestDir: {app}\qt\plugins; Flags: promptifolder recursesubdirs; Excludes: *d4.dll
Source: {#QT_PATH}\translations\*.*; DestDir: {app}\qt\translations; Flags: promptifolder recursesubdirs; Excludes: *.pri README
; MinGW related files
;Source: D:\pasnox\Disk Wine\windows\system32\mingwm10.dll; DestDir: {app}; Flags: confirmoverwrite promptifolder

[Icons]
Name: {group}\{cm:MKS_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\{cm:MKS_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home Page.url; WorkingDir: {app}
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}

[INI]
Filename: {app}\Home Page.url; Section: InternetShortcut; Key: URL; String: {cm:MKS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Components:
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: {cm:MKS_FORUMS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty

[UninstallDelete]
Name: {app}\Home Page.url; Type: files
Name: {app}\Forums.url; Type: files
Name: {app}\*.ini; Type: files
Name: {app}\scripts-*; Type: filesandordirs
Name: {app}; Type: dirifempty
