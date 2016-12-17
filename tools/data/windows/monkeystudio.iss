; This script need Inno Setup version supporting pre processor

#define PROJECT_NAME GetEnv("PROJECT_NAME")
#define PROJECT_COPYRIGHTS GetEnv("PROJECT_COPYRIGHTS")
#define PROJECT_COMPANY GetEnv("PROJECT_COMPANY")
#define PROJECT_DESCRIPTION GetEnv("PROJECT_DESCRIPTION")
#define PROJECT_URL_HOMEPAGE GetEnv("PROJECT_URL_HOMEPAGE")
#define PROJECT_FORUMS_URL GetEnv("PROJECT_FORUMS_URL")
#define PROJECT_ISSUES_URL GetEnv("PROJECT_ISSUES_URL")
#define PROJECT_VERSION GetEnv("PROJECT_VERSION")
#define PROJECT_VERSION_STR GetEnv("PROJECT_VERSION_STR")
#define PROJECT_REVISION GetEnv( "PROJECT_REVISION" )

#define QT_WINDOWS_MINGW_DOWNLOAD GetEnv("QT_WINDOWS_MINGW_DOWNLOAD")
#define WINDOWS_MINGW_DOWNLOAD GetEnv("WINDOWS_MINGW_DOWNLOAD")
#define QT_DOWNLOAD GetEnv("QT_DOWNLOAD")

#define WINE_ROOT_DRIVE GetEnv("WINE_ROOT_DRIVE")
#define QT_WINDOWS_PATH WINE_ROOT_DRIVE +GetEnv("QT_WINDOWS_PATH")
#define WINDOWS_DLLS_PATH WINE_ROOT_DRIVE +GetEnv("WINDOWS_DLLS_PATH")

#define SETUP_ICON WINE_ROOT_DRIVE +GetEnv("SETUP_ICON")
#define SETUP_LEFT_BANNER WINE_ROOT_DRIVE +GetEnv("SETUP_LEFT_BANNER")
#define SETUP_TOP_BANNER WINE_ROOT_DRIVE +GetEnv("SETUP_TOP_BANNER")
#define SETUP_SOURCE_DIRECTORY WINE_ROOT_DRIVE +GetEnv("SETUP_SOURCE_DIRECTORY")
#define SETUP_OUTPUT_DIRECTORY WINE_ROOT_DRIVE +GetEnv("SETUP_OUTPUT_DIRECTORY")
#define SETUP_OUTPUT_NAME GetEnv("SETUP_OUTPUT_NAME")

#define SCRIPT_PATH WINE_ROOT_DRIVE +GetEnv("SCRIPT_PATH")

[InnoIDE_Settings]
LogFileOverwrite=false

[_ISTool]
UseAbsolutePaths=false

[CustomMessages]

[Setup]
SourceDir={#SETUP_SOURCE_DIRECTORY}
OutputDir={#SETUP_OUTPUT_DIRECTORY}
OutputBaseFilename={#SETUP_OUTPUT_NAME}
VersionInfoProductName={#PROJECT_NAME}
;VersionInfoVersion={#PROJECT_VERSION}
VersionInfoCompany={#PROJECT_COMPANY}
VersionInfoDescription={#PROJECT_DESCRIPTION}
VersionInfoTextVersion={#PROJECT_VERSION_STR}
VersionInfoCopyright={#PROJECT_COPYRIGHTS}
;VersionInfoProductVersion={#PROJECT_VERSION}
AppCopyright={#PROJECT_COPYRIGHTS}
AppName={#PROJECT_NAME}
AppVerName={#PROJECT_NAME} {#PROJECT_VERSION_STR}
InfoAfterFile=dev-readme
InfoBeforeFile=readme.txt
LicenseFile=GPL-3
ChangesAssociations=true
PrivilegesRequired=none
DefaultDirName={pf}\{#PROJECT_NAME}
EnableDirDoesntExistWarning=false
AllowNoIcons=true
DefaultGroupName={#PROJECT_NAME}
AlwaysUsePersonalGroup=true
AppPublisher={#PROJECT_COMPANY}
AppPublisherURL={#PROJECT_URL_HOMEPAGE}
AppSupportURL={#PROJECT_FORUMS_URL}
AppUpdatesURL={#PROJECT_URL_HOMEPAGE}
AppVersion={#PROJECT_VERSION}
AppComments=Thanks using {#PROJECT_NAME}
AppContact={#PROJECT_FORUMS_URL}
UninstallDisplayName={#PROJECT_NAME}
ShowLanguageDialog=yes
UsePreviousLanguage=no
;SolidCompression=true
;Compression=lzma2/Ultra64
;InternalCompressLevel=Ultra64
MinVersion=4.10.1998,5.1.2600
SetupIconFile={#SETUP_ICON}
WizardImageFile={#SETUP_LEFT_BANNER}
WizardSmallImageFile={#SETUP_TOP_BANNER}

[Files]
; Project related files
Source: bin\*; DestDir: {app}; Flags: promptifolder recursesubdirs createallsubdirs
; Qt related files
Source: {#QT_WINDOWS_PATH}\bin\*.dll; DestDir: {app}; Flags: promptifolder; Excludes: *d4.dll
Source: {#QT_WINDOWS_PATH}\plugins\*.dll; DestDir: {app}\qt\plugins; Flags: promptifolder recursesubdirs; Excludes: *d4.dll
; Others related files
Source: {#WINDOWS_DLLS_PATH}\mingwm10.dll; DestDir: {app}; Flags: promptifolder
Source: {#WINDOWS_DLLS_PATH}\libgcc_s_dw2-1.dll; DestDir: {app}; Flags: promptifolder skipifsourcedoesntexist
Source: {#WINDOWS_DLLS_PATH}\libstdc++-6.dll; DestDir: {app}; Flags: promptifolder skipifsourcedoesntexist
Source: {#WINDOWS_DLLS_PATH}\libeay32.dll; DestDir: {app}; Flags: promptifolder
Source: {#WINDOWS_DLLS_PATH}\libssl32.dll; DestDir: {app}; Flags: promptifolder
Source: {#WINDOWS_DLLS_PATH}\ssleay32.dll; DestDir: {app}; Flags: promptifolder

[Icons]
Name: {group}\{#PROJECT_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {userdesktop}\{#PROJECT_NAME}; Filename: {app}\monkeystudio.exe; WorkingDir: {app}; IconFilename: {app}\monkeystudio.exe; IconIndex: 0
Name: {group}\Home Page; Filename: {app}\Home page.url; WorkingDir: {app}
Name: {group}\Forums; Filename: {app}\Forums.url; WorkingDir: {app}
Name: {group}\Issues Tracker; Filename: {app}\Issues tracker.url; WorkingDir: {app}
Name: {group}\{cm:UninstallProgram, {#PROJECT_NAME}}; Filename: {uninstallexe}; WorkingDir: {app}
Name: {group}\Downloads Qt Libraries; Filename: {app}\Downloads Qt Libraries.url; WorkingDir: {app}
Name: {group}\Downloads MinGW; Filename: {app}\Downloads MinGW.url; WorkingDir: {app}
Name: {group}\Qt downloads page; Filename: {app}\Qt downloads page.url; WorkingDir: {app}

[INI]
Filename: {app}\Home page.url; Section: InternetShortcut; Key: URL; String: {#PROJECT_URL_HOMEPAGE}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Forums.url; Section: InternetShortcut; Key: URL; String: {#PROJECT_FORUMS_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Issues tracker.url; Section: InternetShortcut; Key: URL; String: {#PROJECT_ISSUES_URL}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Downloads Qt Libraries.url; Section: InternetShortcut; Key: URL; String: {#QT_WINDOWS_MINGW_DOWNLOAD}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty; Languages: 
Filename: {app}\Downloads MinGW.url; Section: InternetShortcut; Key: URL; String: {#WINDOWS_MINGW_DOWNLOAD}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty
Filename: {app}\Qt downloads page.url; Section: InternetShortcut; Key: URL; String: {#QT_DOWNLOAD}; Flags: createkeyifdoesntexist uninsdeleteentry uninsdeletesectionifempty

[UninstallDelete]
Name: {group}\{#PROJECT_NAME}; Type: filesandordirs
Name: {app}\*; Type: filesandordirs
Name: {app}; Type: dirifempty

[Run]
Filename: {app}\monkeystudio.exe; WorkingDir: {app}; Flags: postinstall skipifsilent; Description: {#PROJECT_NAME}

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
