[CustomMessages]
MKS_VERSION=1.8.3.3
MKS_REVISION=3083
MKS_SETUP_NAME={cm:setup_mks_{cm:MKS_VERSION}-svn{cm:MKS_REVISION}-win32.exe}
MKS_NAME=Monkey Studio IDE
MKS_COPYRIGHTS=2005 - 2009 Azevedo Filipe & The Monkey Studio Team
MKS_URL=http://monkeystudio.org
MKS_FORUMS_URL=http://monkeystudio.org/forum
[Setup]
OutputDir=..\setups
SourceDir=..\bin
OutputBaseFilename=setup_mks_VERSION-svnREVISION-win32.exe
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
Source: monkeystudio.exe; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\GPL-2; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\GPL-3; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\LGPL-3; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\dev-readme; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: ..\readme.txt; DestDir: {app}; Flags: confirmoverwrite promptifolder
Source: plugins\*.dll; DestDir: {app}\plugins; Flags: promptifolder recursesubdirs
;Source: *.dll; DestDir: {app}; Flags: promptifolder
Source: translations\monkey_*.qm; DestDir: {app}\translations; Flags: promptifolder
Source: templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs
Source: apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs
[Icons]
Name: {group}\{cm:MKS_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\{cm:MKS_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home Page.url; WorkingDir: {app}
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}
[INI]
Filename: {app}\Home Page.url; Section: InternetShortcut; Key: URL; String: {cm:MKS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Components: 
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: {cm:MKS_FORUMS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
[UninstallDelete]
Type: files; Name: {app}\Home Page.url; Components: 
Type: files; Name: {app}\Forums.url
