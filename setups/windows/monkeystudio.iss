[CustomMessages]
MKS_VERSION=1.8.4.0b1
MKS_REVISION=3083
MKS_SETUP_NAME={cm:setup_mks_{cm:MKS_VERSION}-svn{cm:MKS_REVISION}-win32.exe}
MKS_NAME=Monkey Studio IDE
MKS_COPYRIGHTS=2005 - 2009 Azevedo Filipe & The Monkey Studio Team
MKS_URL=http://monkeystudio.org
MKS_FORUMS_URL=http://monkeystudio.org/forum
QT_PATH=C:\Development\Qt\4.5.1

[Setup]
OutputDir=..
SourceDir=..\..\bin
OutputBaseFilename=setups\setup_mks_VERSION-svnREVISION-win32.exe
VersionInfoVersion=2.0.0.0
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible cross-platform IDE
VersionInfoTextVersion={cm:MKS_VERSION}
VersionInfoCopyright={cm:MKS_COPYRIGHTS}
AppCopyright={cm:MKS_COPYRIGHTS}
AppName={cm:MKS_NAME}
AppVerName={cm:MKS_NAME} {cm:MKS_VERSION}
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
AppVersion={cm:MKS_VERSION}
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
Source: scripts\*.*; DestDir: {app}\scripts; Flags: promptifolder
Source: translations\*.*; DestDir: {app}\translations; Flags: promptifolder
Source: templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs
Source: apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs
; Qt related files
Source: C:\Development\Qt\4.5.1\bin\*.dll; DestDir: {app}; Flags: confirmoverwrite promptifolder; Excludes: *d4.dll
Source: C:\Development\Qt\4.5.1\doc\qch\*.qch; DestDir: {app}\qt\doc\qch; Flags: promptifolder recursesubdirs
Source: C:\Development\Qt\4.5.1\plugins\*.dll; DestDir: {app}\qt\plugins; Flags: promptifolder recursesubdirs; Excludes: *d4.dll
Source: C:\Development\Qt\4.5.1\translations\*.*; DestDir: {app}\qt\translations; Flags: promptifolder recursesubdirs; Excludes: *.pri README

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
