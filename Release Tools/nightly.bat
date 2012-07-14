cd ..
svn up
cd "Release Tools"

call redist_gen_nightly.bat
call sdk_redist_nightly.bat
call modules_redist_nightly.bat

call nightly_upload_adamix.bat