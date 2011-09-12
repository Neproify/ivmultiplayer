mkdir sdk_release
cd ..\Server\Core\Interfaces
call copy.bat
cd ..\..\..\Release Tools
xcopy /Y /E ..\Binary\sdk sdk_release\
"Redistrubutable Generator"\7z.exe a ivmp_sdk.zip sdk_release
pause