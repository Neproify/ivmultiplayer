echo -n "Enter the package name and press [ENTER]: "


read VER
export
FILE=$VER.zip
export
TARFILE=$VER.tar

zip
-9r $FILE ../Binary/ivmp-svr
zip
-9r $FILE ../Binary/webserver -x *.svn*
zip
-9r $FILE ../Binary/scripts -x *.svn*
zip
-9r $FILE ../Binary/resources -x *.svn*
zip
-9r $FILE ../Binary/modules -x *.svn*
zip
-9r $FILE ../Binary/files -x *.svn*
zip
-9r $FILE ../Binary/clientscripts -x *.svn*
zip
-9rj $FILE ../Binary/settings.xml "LICENSE"

unzip $FILE -d $VER
tar cf $TARFILE $VER
gzip $TARFILE
rm -R $VER
