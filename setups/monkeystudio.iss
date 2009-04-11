[Setup]
OutputDir=..\setups
SourceDir=..\bin
OutputBaseFilename=setup_monkeystudio_mingw
VersionInfoVersion=2.0.0.0
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible cross-platform IDE
VersionInfoTextVersion=2.0.0.0
VersionInfoCopyright=2005, 2006, 2007 Azevedo Filipe & The Monkey Studio Team
AppCopyright=2005, 2006, 2007 Azevedo Filipe & The Monkey Studio Team
AppName=Monkey Studio IDE
AppVerName=Monkey Studio IDE 1.8.3.0
InfoAfterFile=..\dev-readme
InfoBeforeFile=..\readme.txt
LicenseFile=..\GPL-3
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\Monkey Studio IDE
EnableDirDoesntExistWarning=true
AllowNoIcons=true
DefaultGroupName=Monkey Studio IDE
AlwaysUsePersonalGroup=true
;SetupIconFile=..\monkey\src\resources\icons\application\monkey2.ico
AppPublisher=2005, 2006, 2007, 2008 Azevedo Filipe, and The Monkey Studio Team
AppPublisherURL=http://www.monkeystudio.org
AppSupportURL=http://forums.monkeystudio.org
AppVersion=1.8.3.0
AppComments=Thanks using Monkey Studio IDE
AppContact=http://forums.monkeystudio.org
UninstallDisplayName=Monkey Studio IDE
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
Source: plugins\GNUdbg2\file\gdbparsing.txt; DestDir: {app}\plugins; Flags: confirmoverwrite promptifolder
Source: plugins\*.dll; DestDir: {app}\plugins; Flags: promptifolder recursesubdirs
;Source: *.dll; DestDir: {app}; Flags: promptifolder
Source: translations\monkey_*.qm; DestDir: {app}\translations; Flags: promptifolder
Source: templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs
Source: apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs
[Icons]
Name: {group}\Monkey Studio IDE; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\Monkey Studio IDE; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home Page.url; WorkingDir: {app}
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}
[INI]
Filename: {app}\Home Page.url; Section: InternetShortcut; Key: URL; String: http://www.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Components: 
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: http://forums.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
[UninstallDelete]
Type: files; Name: {app}\Home Page.url; Components: 
Type: files; Name: {app}\Forums.url
