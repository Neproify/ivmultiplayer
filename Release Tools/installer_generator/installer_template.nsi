; DO NOT TRY TO COMPILE THIS FILE (installer_template.nsi).
; ${VERSION} will be replaced by custom script, no macro needed

;======================================================
; Includes
!include "MUI2.nsh"

;======================================================
; Configuration
!define NAME 'IVMP'

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
; Pages
; Check For GTAIV Existence

; Remove Any Old Installations

; Get Install Directory
Page directory

DirText "Welcome to the installer for ${NAME} ${VERSION}.$\n$\nYou must have Grand Theft Auto IV installed in order to install ${NAME}." "Please select the directory to install IVMP to."
InstallDir "$PROGRAMFILES\IVMP"

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;======================================================
; Sections

Section "Install" SecDummy
	SetOverwrite on

	; Copy New Files

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
	
	File ..\..\Binary\CEGUI\fonts\bankgothic.ttf
	File ..\..\Binary\CEGUI\fonts\DejaVuSans.ttf
	File ..\..\Binary\CEGUI\fonts\pricedown.ttf
	File ..\..\Binary\CEGUI\fonts\tahoma.ttf
	File ..\..\Binary\CEGUI\fonts\tahoma-bold.ttf

	SetOutPath "$INSTDIR\CEGUI\imagesets"

	File ..\..\Binary\CEGUI\imagesets\ReadMe.txt
	File ..\..\Binary\CEGUI\imagesets\WindowsLook.imageset
	File ..\..\Binary\CEGUI\imagesets\WindowsLook.png
	File ..\..\Binary\CEGUI\imagesets\IVMPLogoHiRes.png
	File ..\..\Binary\CEGUI\imagesets\IVMPBackground.png

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

	; Create Start Menu Shortcuts

	CreateShortCut "$SMPROGRAMS\IVMP\${NAME}.lnk" "$INSTDIR\Client.Launcher.exe"

	CreateShortCut "$SMPROGRAMS\IVMP\Uninstall ${NAME}.lnk" "$INSTDIR\Uninstall_${NAME}.exe"

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
	
	; Delete Installer

	Delete "$INSTDIR\Uninstall_${NAME}.exe"
SectionEnd
