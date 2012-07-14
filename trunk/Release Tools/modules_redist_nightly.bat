for /f %%i in ('svnversion') do set VERSION=r%%i-Win32-Nightly
cd ..\Modules\
call build.bat
cd "..\Release Tools\"
7z.exe a IVMP-%VERSION%-Modules.zip ..\Modules\Binary\