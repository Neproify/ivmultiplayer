@echo off
for /f %%i in ('svnversion') do set VERSION=r%%i-Win32-Nightly
set CLIENT=IVMP-%VERSION%-Client.zip
set SERVER=IVMP-%VERSION%-Server.zip
set SDK=IVMP-%VERSION%-SDK.zip
copy /Y %CLIENT% W:\adamix.iv-m.com\nightly\
copy /Y %SERVER% W:\adamix.iv-m.com\nightly\
copy /Y %SDK% W:\adamix.iv-m.com\nightly\
