!define PRODUCT_NAME "Digital Paintball"
!define PRODUCT_VERSION "0.18"
!define PRODUCT_PUBLISHER "Soulstrewn Studios LLC"
!define PRODUCT_WEB_SITE "http://www.digitalpaintball.net"

!define APPID 220
!define MODDIR "dpb"

!define LOCALDIR "C:\build"
!define MUI_ICON "e:\trunk\build\dpb.ico"
!define FULL_GAME_NAME "Digital Paintball 0.18 Build 684"
!define DESKICO "e:\trunk\build\dpb.ico"

;======DO NOT EDIT BEYOND THIS POINT======


var ICONDIR
var STEAMEXE

!include "MUI.nsh"
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
ShowInstDetails show

Section "Mod Files" FILES
	SetOverwrite ifdiff
	SetOutPath "$INSTDIR"
	File /r "${LOCALDIR}\${MODDIR}"

SectionEnd
Page custom Finish
Function Finish
  MessageBox MB_OK|MB_ICONEXCLAMATION "Steam must be restarted for the game to show on the games list!"
FunctionEnd
Function .onInit
	ReadRegStr $R0 HKCU "Software\Valve\Steam" SourceModInstallPath
	IfErrors lbl_error 0
	StrCpy $INSTDIR "$R0\"

	SectionSetFlags ${FILES} 17
	Return
	lbl_error:
		ClearErrors
		SectionSetFlags ${FILES} 17
FunctionEnd