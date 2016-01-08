for /f %%i in ('git rev-list --count HEAD') do set VERSION=r%%i
mkdir sdk_release
cd ..\Server\Core\Interfaces
call copy.bat
cd ..\..\..\Release Tools
xcopy /Y /E ..\Binary\sdk sdk_release\
7z.exe a IVMP-%VERSION%-SDK.zip sdk_release
pause