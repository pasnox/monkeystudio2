[Setup]
OutputDir=..\setups
SourceDir=..\bin
OutputBaseFilename=setup_monkeystudio_msvc
VersionInfoVersion=2.0.0.0
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible Qt RAD
VersionInfoTextVersion=2.0.0.0
VersionInfoCopyright=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppCopyright=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppName=Monkey Studio RAD
AppVerName=Monkey Studio RAD 1.8.0.0
InfoAfterFile=..\dev-readme
InfoBeforeFile=..\readme.txt
LicenseFile=..\license.gpl
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\Monkey Studio RAD
EnableDirDoesntExistWarning=true
AllowNoIcons=true
DefaultGroupName=Monkey Studio RAD
AlwaysUsePersonalGroup=true
SetupIconFile=..\monkey\src\resources\icons\application\monkey2.ico
AppPublisher=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppPublisherURL=http://www.monkeystudio.org
AppSupportURL=http://forums.monkeystudio.org
AppVersion=1.8.0.0
AppComments=Thanks using Monkey Studio RAD
AppContact=http://forums.monkeystudio.org
UninstallDisplayName=Monkey Studio RAD
[_ISTool]
UseAbsolutePaths=false
[Files]
Source: monkeystudio.exe; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: ..\license.gpl; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: ..\dev-readme; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: plugins\base\AStyle.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: plugins\base\Ctags2Api.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: plugins\base\FileBrowser.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: plugins\base\MessageBox.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base Qt_Plugins
Source: plugins\base\Navigator.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: plugins\base\ProjectHeaders.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: plugins\builder\GNUMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Builder
Source: plugins\builder\MSVCMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Builder Qt_Plugins
Source: plugins\child\QtAssistant.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Child Qt_Plugins
Source: plugins\child\QtDesigner.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Child Qt_Plugins
Source: plugins\compiler\G++.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler
Source: plugins\compiler\GccParser.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler
Source: plugins\compiler\Gcc.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler
Source: plugins\compiler\MSVC.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler Qt_Plugins
Source: plugins\project\MonkeyProject.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Project
Source: plugins\project\QMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Project Qt_Plugins
Source: translations\monkey_*.qm; DestDir: {app}\translations; Flags: promptifolder; Components: Monkey_Studio
Source: templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs; Components: Monkey_Studio
Source: apis\*.*; DestDir: {app}\apis; Flags: promptifolder recursesubdirs createallsubdirs; Components: Monkey_Studio
[Icons]
Name: {group}\Monkey Studio RAD; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\Monkey Studio RAD; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home Page.url; WorkingDir: {app}; Comment: Monkey Studio Home Page
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}; Comment: Monkey Studio Forums
[Components]
Name: Monkey_Studio; Description: Monkey Studio Application; Types: full custom compact; Flags: fixed
Name: All_Plugins; Description: All Plugins; Types: full; Flags: exclusive
Name: Qt_Plugins; Description: Qt Plugins; Types: compact; Flags: exclusive
Name: Plugins; Description: Custom Plugins; Types: custom; Flags: exclusive
Name: Plugins\Base; Description: Base Plugins; Types: custom
Name: Plugins\Builder; Description: Builder Plugins; Types: custom
Name: Plugins\Child; Description: Child Plugins; Types: custom
Name: Plugins\Compiler; Description: Compiler Plugins; Types: custom
Name: Plugins\Project; Description: Project Plugins; Types: custom
[INI]
Filename: {app}\Home Page.url; Section: InternetShortcut; Key: URL; String: http://www.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: http://forums.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
[UninstallDelete]
Type: files; Name: {app}\Home Page.url; Components: 
Type: files; Name: {app}\Forums.url
