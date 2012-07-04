@echo off
for /f %%i in ('svnversion') do set VERSION=r%%i-Win32-Nightly

call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
cd ..
devenv.exe IVMP.sln /build Release
cd Network.Release
rem call build.bat
cd ..
cd "Release Tools"


7z a IVMP-%VERSION%-Server.zip ../Binary/ivmp-svr.exe
7z a IVMP-%VERSION%-Server.zip ../Binary/Network.Core.dll
7z a IVMP-%VERSION%-Server.zip ../Binary/settings.xml
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/files
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/modules
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/scripts
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/clientscripts
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/resources
7z a -xr!?svn\ IVMP-%VERSION%-Server.zip ../Binary/webserver
7z a IVMP-%VERSION%-Server.zip LICENSE
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
