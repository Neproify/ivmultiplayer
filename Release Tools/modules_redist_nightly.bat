for /f %%i in ('git rev-list --count HEAD') do set VERSION=r%%i-Win32-Nightly
cd ..\Modules\
call build.bat
cd "..\Release Tools\"
7z.exe a IVMP-%VERSION%-Modules.zip ..\Modules\Binary\