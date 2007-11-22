[Setup]
OutputDir=setups
SourceDir=../
OutputBaseFilename=setup_monkeystudio_mingw
VersionInfoVersion=2.0.0.0
VersionInfoCompany=Monkey Studio Team
VersionInfoDescription=Free, Fast and Flexible Qt RAD
VersionInfoTextVersion=2.0.0.0
VersionInfoCopyright=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppCopyright=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppName=Monkey Studio RAD
AppVerName=Monkey Studio RAD 1.8.0.0
InfoAfterFile=dev-readme
InfoBeforeFile=readme.txt
LicenseFile=license.gpl
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\Monkey Studio RAD
EnableDirDoesntExistWarning=true
AllowNoIcons=true
DefaultGroupName=Monkey Studio RAD
AlwaysUsePersonalGroup=true
SetupIconFile=monkey\src\resources\icons\application\monkey2.ico
AppPublisher=2005, 2006, 2007 Azevedo Filipe, and The Monkey Studio Team
AppPublisherURL=http://www.monkeystudio.org
AppSupportURL=http://forums.monkeystudio.org
AppVersion=1.8.0.0
AppComments=Thanks using Monkey Studio RAD
AppContact=http://forums.monkeystudio.org
UninstallDisplayName=Monkey Studio RAD
ShowLanguageDialog=yes
[_ISTool]
UseAbsolutePaths=false
[Files]
Source: bin\monkey.exe; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: license.gpl; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: dev-readme; DestDir: {app}; Flags: confirmoverwrite promptifolder; Components: Monkey_Studio
Source: bin\plugins\base\AStyle.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: bin\plugins\base\Ctags2Api.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: bin\plugins\base\FileBrowser.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: bin\plugins\base\MessageBox.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base Qt_Plugins
Source: bin\plugins\base\Navigator.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: bin\plugins\base\ProjectHeaders.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Base
Source: bin\plugins\builder\GNUMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Builder Qt_Plugins
Source: bin\plugins\builder\MSVCMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Builder
Source: bin\plugins\child\QtAssistant.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Child Qt_Plugins
Source: bin\plugins\child\QtDesigner.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Child Qt_Plugins
Source: bin\plugins\compiler\G++.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler Qt_Plugins
Source: bin\plugins\compiler\GccParser.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler Qt_Plugins
Source: bin\plugins\compiler\Gcc.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler
Source: bin\plugins\compiler\MSVC.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Compiler
Source: bin\plugins\project\MonkeyProject.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Project
Source: bin\plugins\project\QMake.dll; DestDir: {app}\plugins; Flags: promptifolder; Components: All_Plugins Plugins\Project Qt_Plugins
Source: translations\monkey_*.qm; DestDir: {app}\translations; Flags: promptifolder; Components: Monkey_Studio
Source: templates\*.*; DestDir: {app}\templates; Flags: promptifolder recursesubdirs createallsubdirs; Components: Monkey_Studio
Source: C:\Documents and Settings\LSI\.Monkey Studio\Monkey Studio.ini; DestDir: {userappdata}\..\.Monkey Studio; Flags: ignoreversion; Components: 
[Icons]
Name: {group}\Monkey Studio RAD; Filename: {app}\monkey.exe; WorkingDir: {app}; IconFilename: {app}\monkey.exe; IconIndex: 0
Name: {userdesktop}\Monkey Studio RAD; Filename: {app}\monkey.exe; WorkingDir: {app}; IconFilename: {app}\monkey.exe; IconIndex: 0
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
Filename: {app}\Home Page.url; Section: InternetShortcut; Key: URL; String: http://www.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Components: 
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: http://forums.monkeystudio.org/; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
[UninstallDelete]
Type: files; Name: {app}\Home Page.url; Components: 
Type: files; Name: {app}\Forums.url
