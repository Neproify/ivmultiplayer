cd ../
svn up
make
cd "Release Tools"
GZIPFILE=IVMP-r`svnversion`-Linux-Nightly-Server.tar.gz
tar cfvz $GZIPFILE ../Binary/ivmp-svr ../Binary/Network.Core.so ../Binary/webserver ../Binary/scripts ../Binary/resources ../Binary/modules ../Binary/files ../Binary/clientscripts ../Binary/settings.xml LICENSE --exclude=.svn
cp $GZIPFILE /www/adamix.iv-m.com/nightly/
