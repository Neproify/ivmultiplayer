#include <stdio.h>

#include <iostream>

#include <string>

#include <string.h>

#include <fstream>

#include <sstream>



int main(int argc, char ** argv)

{
	std::ifstream fin("installer_template.nsi");
	std::ofstream fout("installer.nsi");
	std::string line;

	while(std::getline(fin, line))
	{
		const int pos = line.find("${VERSION}");

		if(pos != -1)
			line.replace(pos, strlen("${VERSION}"), argv[1]);


		fout << line << "\n";

	}


}
