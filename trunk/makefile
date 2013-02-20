all:
	make -C Vendor/sqlite
	make -C Vendor/tinyxml
	make -C Vendor/Squirrel
	make -C Server/Core
	make -C Server/Launcher
	make -C Network/Core pch
	make -C Network/Core

clean:
	make -C Vendor/sqlite clean
	make -C Vendor/tinyxml clean
	make -C Vendor/Squirrel clean
	make -C Server/Core clean
	make -C Server/Launcher clean
	make -C Network/Core clean

