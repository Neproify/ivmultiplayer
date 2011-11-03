echo "Enter the package name and press [ENTER]: " 

read VER 
export GZIPFILE=$VER.tar.gz 

tar cfvz $GZIPFILE ../Binary/ivmp-svr ../Binary/Network.Core.so ../Binary/webserver ../Binary/scripts ../Binary/resources ../Binary/modules ../Binary/files ../Binary/clientscripts ../Binary/settings.xml LICENSE --exclude=.svn 
