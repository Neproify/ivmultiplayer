@echo off
FOR /F "tokens=* USEBACKQ" %%F IN (`git rev-list --count HEAD`) DO (
SET VERSION=r%%F
)
7z a IVMP-%VERSION%-Win32-Server.zip ../Binary/ivmp-svr.exe
7z a IVMP-%VERSION%-Win32-Server.zip ../Binary/Server.Core.dll
7z a IVMP-%VERSION%-Win32-Server.zip ../Binary/Network.Core.dll
7z a IVMP-%VERSION%-Win32-Server.zip ../Binary/settings.xml
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/files
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/modules
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/scripts
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/clientscripts
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/resources
7z a -xr!?svn\ IVMP-%VERSION%-Win32-Server.zip ../Binary/webserver
7z a IVMP-%VERSION%-Win32-Server.zip LICENSE
cd installer_generator
Build_Install_Script.exe %VERSION%
cd nsis
makensis.exe ../installer.nsi
cd ..
Del installer.nsi
cd ..
7z a IVMP-%VERSION%-Client.zip IVMP-%VERSION%.exe
7z a IVMP-%VERSION%-Client.zip LICENSE
Del IVMP-%VERSION%.exe
