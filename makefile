all:
	make -C Shared/sqlite
	make -C Shared/tinyxml
	make -C Shared/Squirrel
	make -C Server/Core
	cd Binary && sh linux_redist.sh

clean:
	make -C Shared/sqlite clean
	make -C Shared/tinyxml clean
	make -C Shared/Squirrel clean
	make -C Server/Core clean

