; DO NOT TRY TO COMPILE THIS FILE (installer_template.nsi).
; ${VERSION} will be replaced by custom script, no macro definition needed

;======================================================
; Includes
!include nsDialogs.nsh
!include "MUI2.nsh"
!include LogicLib.nsh

;======================================================
; Configuration
!define NAME 'IVMP'
!define VERSION 'RC2'

;======================================================
; Modern Interface Configuration
!define MUI_ICON .\icon.ico
!define MUI_INSTALLCOLORS "DDDDDD 333333"
!define MUI_INSTFILESPAGE_COLORS "DDDDDD 333333"
!define MUI_INSTFILESPAGE_PROGRESSBAR colored

;======================================================
; Installer Information
Name "${NAME} ${VERSION}"
OutFile "../${NAME}-${VERSION}.exe"
SetCompressor /SOLID lzma
CRCCheck force
BrandingText /TRIMCENTER "${NAME} ${VERSION} Setup"
ShowInstDetails show
ShowUninstDetails show
RequestExecutionLevel admin

;======================================================
; Initialize Function
Var GTAIVDirectory

Function .onInit
	; Try to get the GTAIV directory from the registry
	ReadRegStr $GTAIVDirectory HKLM "Software\Rockstar Games\Grand Theft Auto IV" "InstallFolder"

	; Did we find it?
	IfFileExists $GTAIVDirectory\LaunchGTAIV.exe done

	; Try to get the GTAIV directory from the registry
	ReadRegStr $GTAIVDirectory HKCU "Software\IVMP" "gtaivdir"

	; Did we find it?
	IfFileExists $GTAIVDirectory\LaunchGTAIV.exe done

	; Show a dialog to find the GTAIV directory
	nsDialogs::SelectFolderDialog "Please select your Grand Theft Auto IV directory" ""
	Pop $GTAIVDirectory

	; Did we find it?
	IfFileExists $GTAIVDirectory\LaunchGTAIV.exe done

	; GTAIV directory not found
	MessageBox MB_OK "Failed to find Grand Theft Auto IV directory. Grand Theft Auto IV must be installed in order to install IV:MP."
	Abort

done:
FunctionEnd

;======================================================
; Pages

; Get Install Directory Page
Page directory

DirText "Welcome to the installer for ${NAME} ${VERSION}.$\n$\nYou must have Grand Theft Auto IV installed in order to install ${NAME}." "Please select the directory to install IVMP to."
InstallDir "$PROGRAMFILES\IVMP"

; Options Page

Var OptionsWindow
Var OptionsPageText
Var CreateStartMenuShortcutsCheckbox
Var CreateDesktopShortcutCheckbox
Var CreateStartMenuShortcuts
Var CreateDesktopShortcut

Page custom OptionsPage OptionsPageLeave

Function OptionsPage
	nsDialogs::Create 1018
	Pop $OptionsWindow

	${If} $OptionsWindow == error
		Abort
	${EndIf}

	${NSD_CreateLabel} 0 0 100% 20u "Please select the following installation options, then click Install to proceed with the installation."
	Pop $OptionsPageText

	${NSD_CreateCheckbox} 0 40u 100% 10u "&Create Start Menu Shortcuts"
	Pop $CreateStartMenuShortcutsCheckbox

	${NSD_Check} $CreateStartMenuShortcutsCheckbox

	${NSD_CreateCheckbox} 0 55u 100% 10u "&Create Desktop Shortcut"
	Pop $CreateDesktopShortcutCheckbox

	nsDialogs::Show
FunctionEnd

Function OptionsPageLeave
	${NSD_GetState} $CreateStartMenuShortcutsCheckbox $CreateStartMenuShortcuts

	${NSD_GetState} $CreateDesktopShortcutCheckbox $CreateDesktopShortcut
FunctionEnd

;======================================================
; Installer

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;======================================================
; Sections

Section "Install" SecDummy
	SetOverwrite on

	; Copy New Files

	SetOutPath "$GTAIVDirectory"
	
	File ..\..\Binary\bass.dll
	
	SetOutPath "$GTAIVDirectory\common\data\"
	File ..\..\Binary\gamefiles\loadingscreens_ivmp.dat
	
	SetOutPath "$GTAIVDirectory\pc\textures\"
	File ..\..\Binary\gamefiles\loadingscreens_ivmp_textures.wtd
	
	SetOutPath "$INSTDIR"

	File ..\LICENSE
	File ..\..\Binary\Client.Launcher.exe
	File ..\..\Binary\Client.LaunchHelper.dll
	File ..\..\Binary\Client.Core.dll
	File ..\..\Binary\Network.Core.dll

	CreateDirectory "$INSTDIR\CEGUI"
	CreateDirectory "$INSTDIR\CEGUI\fonts"
	CreateDirectory "$INSTDIR\CEGUI\imagesets"
	CreateDirectory "$INSTDIR\CEGUI\layouts"
	CreateDirectory "$INSTDIR\CEGUI\looknfeel"
	CreateDirectory "$INSTDIR\CEGUI\schemes"
	CreateDirectory "$INSTDIR\CEGUI\xml_schemas"
	
	SetOutPath "$INSTDIR\CEGUI\fonts"
	
	File ..\..\Binary\CEGUI\fonts\*

	SetOutPath "$INSTDIR\CEGUI\imagesets"

	File ..\..\Binary\CEGUI\imagesets\*

	SetOutPath "$INSTDIR\CEGUI\layouts"

	;

	SetOutPath "$INSTDIR\CEGUI\looknfeel"
	
	File ..\..\Binary\CEGUI\looknfeel\WindowsLook.looknfeel

	SetOutPath "$INSTDIR\CEGUI\schemes"
	
	File ..\..\Binary\CEGUI\schemes\WindowsLook.scheme
	File ..\..\Binary\CEGUI\schemes\WindowsLookWidgets.scheme

	SetOutPath "$INSTDIR\CEGUI\xml_schemas"
	
	File ..\..\Binary\CEGUI\xml_schemas\CEGUIConfig.xsd
	File ..\..\Binary\CEGUI\xml_schemas\Falagard.xsd
	File ..\..\Binary\CEGUI\xml_schemas\Font.xsd
	File ..\..\Binary\CEGUI\xml_schemas\GUILayout.xsd
	File ..\..\Binary\CEGUI\xml_schemas\GUIScheme.xsd
	File ..\..\Binary\CEGUI\xml_schemas\Imageset.xsd

	; Create Start Menu Folder

	CreateDirectory "$SMPROGRAMS\IVMP"

	; Create Start Menu Shortcuts If Requested

	${If} $CreateStartMenuShortcuts == ${BST_CHECKED}
		CreateShortCut "$SMPROGRAMS\IVMP\${NAME}.lnk" "$INSTDIR\Client.Launcher.exe"

		CreateShortCut "$SMPROGRAMS\IVMP\Uninstall ${NAME}.lnk" "$INSTDIR\Uninstall_${NAME}.exe"
	${EndIf}

	; Create Desktop Shortcut If Requested
	
	${If} $CreateDesktopShortcut == ${BST_CHECKED}
		CreateShortCut "$DESKTOP\${NAME}.lnk" "$INSTDIR\Client.Launcher.exe"
	${EndIf}

	; Create Uninstaller

	WriteUninstaller "$INSTDIR\Uninstall_${NAME}.exe"
	
SectionEnd

Section "Uninstall"
	; Delete Files

	Delete "$INSTDIR\LICENSE"
	Delete "$INSTDIR\Client.Launcher.exe"
	Delete "$INSTDIR\Client.LaunchHelper.dll"
	Delete "$INSTDIR\Client.Core.dll"
	Delete "$INSTDIR\Network.Core.dll"

	; Remove Directories

	RMDIR "$INSTDIR\CEGUI"
	RMDIR "$INSTDIR\CEGUI\fonts"
	RMDIR "$INSTDIR\CEGUI\imagesets"
	RMDIR "$INSTDIR\CEGUI\layouts"
	RMDIR "$INSTDIR\CEGUI\looknfeel"
	RMDIR "$INSTDIR\CEGUI\schemes"
	RMDIR "$INSTDIR\CEGUI\xml_schemas"

	; Remove Program Files Folder

	RMDIR "$INSTDIR\"

	; Remove Start Menu Folder

	RMDIR "$SMPROGRAMS\IVMP"

	; Delete The Desktop Shortcut

	Delete "$DESKTOP\${NAME}.lnk"
	
	; Delete the bass library
	Delete "$GTAIVDirectory\bass.dll"
	
	; Delete Installer

	Delete "$INSTDIR\Uninstall_${NAME}.exe"
SectionEnd
