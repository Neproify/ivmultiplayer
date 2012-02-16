//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Client.VFS.Utility
// Author(s): jenksta
// License: See LICENSE in root directory
//
//============================================================================== 

#include <vfs/CVFSInterface.h>
#include <vfs/CVFSModule.h>
#include <vfs/CVFSModule_Functions.h>
#include <string.h>
#include <stdio.h>
#ifndef LINUX
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#define _mkdir(x) mkdir(x, 0777)
#endif
#include <zlib.h>
#include <stdlib.h>

void add_(const char * path, const char * to)
{
	printf("Adding %s to archive [%s].", path, to);
	VFS_FILE * out = (VFS_FILE*)vfs_fopen(to, "wb");
	FILE * in = fopen(path, "rb");

	char buffer[100];
	int numr=0, numw=0;

	
	while(!feof(in))
	{
		numr = fread(buffer, 1, 100, in);
		vfs_fwrite(buffer, 1, numr, out);
	}
	vfs_fclose(out);
	fclose(in);
}

void extract_(const char * path, const char * to, bool isDir = false, bool oneFile = false)
{
	printf("Extracting %s to %s.\n", path, to);
	char expath[4096];
	sprintf(expath, "%s/%s", to, path);

	if(isDir)
	{
		_mkdir(expath);
		return;
	}
	VFS_FILE * in = (VFS_FILE*)vfs_fopen(path, "rb");
	FILE * out = NULL;

	if(!oneFile)
		out = fopen(expath, "wb");
	else
		out = fopen(to, "wb");



	char buffer[100];
	int numr=0, numw=0;

	
	while(numr != -1)
	{
		numr = vfs_fread(buffer, 1, 100, in);
		fwrite(buffer, 1, numr, out);
	}
	vfs_fclose(in);
	fclose(out);
}

void read_dirs(const char * path, bool output = true, bool extract = false, const char * extract_to = NULL)
{
	VFS_DIR dirstat;
	int filecount = 0;


	if (vfs_opendir(path, &dirstat))
	{
		vfs_dirent dirent;

		while (vfs_readdir(&dirstat, &dirent) == 0)
		{
			if (dirent.is_dir)
			{
				if(output)
					printf("%d - %s/%s <DIR> (0x%08lx)\r\n", ++filecount, path, dirent.filename, dirent.cluster);
				char readpath[2048];
				if(strlen(path) > 1)
					sprintf(readpath, "%s/%s", path, dirent.filename);
				else
					sprintf(readpath, "/%s", dirent.filename);
				if(extract)
					extract_(readpath, extract_to, true);
				read_dirs(readpath);
			}
			else
			{
				if(output)
					printf("%d - %s/%s [%d bytes] (0x%08lx)\r\n", ++filecount, path, dirent.filename, dirent.size, dirent.cluster);
				char readpath[2048];

				if(strlen(path) > 1)
					sprintf(readpath, "%s/%s", path, dirent.filename);
				else
					sprintf(readpath, "/%s", dirent.filename);
				if(extract)
					extract_(readpath, extract_to);
			}
		}

		vfs_closedir(&dirstat);
	}
}

int comp(const char * in, const char * out)
{
	printf("Compressing archive file %s to %s...", in, out);
	//input and output files
	VFS_FILE *FileIn = (VFS_FILE*)vfs_fopen(in, "rb");
	VFS_FILE *FileOut = (VFS_FILE*)vfs_fopen(out, "wb");

	//get the file size of the input file
	vfs_fseek(FileIn, 0, SEEK_END);
	unsigned long FileInSize = vfs_ftell(FileIn);

	//buffers for the raw and compressed data
	void *RawDataBuff = malloc(FileInSize);
	void *CompDataBuff = NULL;


	//zlib states that the source buffer must be at least 0.1
	//times larger than the source buffer plus 12 bytes
	//to cope with the overhead of zlib data streams
	uLongf CompBuffSize = (uLongf)(FileInSize + (FileInSize * 0.1) + 12);
	CompDataBuff = malloc((size_t)(CompBuffSize));

	//read in the contents of the file into the source buffer
	vfs_fseek(FileIn, 0, SEEK_SET);
	vfs_fread(RawDataBuff, FileInSize, 1, FileIn);

	//now compress the data
	uLongf DestBuffSize;
	compress2((Bytef*)CompDataBuff, (uLongf*)&DestBuffSize,
		(const Bytef*)RawDataBuff, (uLongf)FileInSize, Z_BEST_COMPRESSION);

	vfs_fwrite(&FileInSize, sizeof(uLongf), 1, FileOut);
	//write the compressed data to disk
	vfs_fwrite(CompDataBuff, DestBuffSize, 1, FileOut);

	vfs_fclose(FileIn);
	vfs_fclose(FileOut);

	return DestBuffSize;
}

int uncomp(const char * in, const char * out)
{
	printf("Decompressing archive file %s to %s...", in, out);
	//the input file, this is the output file from part one
	VFS_FILE *FileIn = (VFS_FILE*)vfs_fopen(in, "rb");

	//output file
	VFS_FILE *FileOut = (VFS_FILE*)vfs_fopen(out, "wb");

	//get the file size of the input file
	vfs_fseek(FileIn, 0, SEEK_END);
	unsigned long FileInSize = vfs_ftell(FileIn);

	//buffers for the raw and uncompressed data
	void *RawDataBuff = malloc(FileInSize);
	void *UnCompDataBuff = NULL;

	//read in the contents of the file into the source buffer
	vfs_fseek(FileIn, 0, SEEK_SET);

	//allocate a buffer big enough to hold the uncompressed data, we can cheat here
	//because we know the file size of the original
	uLongf UnCompSize = 482000;
	vfs_fread(&UnCompSize, sizeof(uLongf), 1, FileIn);
	UnCompDataBuff = malloc(UnCompSize);

	vfs_fread(RawDataBuff, FileInSize, 1, FileIn);

	//all data we require is ready so compress it into the source buffer, the exact
	//size will be stored in UnCompSize
	uncompress((Bytef*)UnCompDataBuff, &UnCompSize, (const Bytef*)RawDataBuff, FileInSize);

	//write the decompressed data to disk
	vfs_fwrite(UnCompDataBuff, UnCompSize, 1, FileOut);

	vfs_fclose(FileIn);
	vfs_fclose(FileOut);

	return UnCompSize;
}

int main(int argc, char **argv)
{
	CVFSModule::Load();
	CVFSInterface * pInterface = CVFSModule::GetInterface();
	//printf("interface: %p\n", pInterface);

	printf("VFSModule.Util 0.1\n\n");

	if(argc < 3)
	{
		printf("%s -c [filename] [size in megabytes] - create disk.\n", argv[0]);
		printf("%s [image filename] -ls - list files in image.\n", argv[0]);
		printf("%s [image filename] -mkdir [directory] - make directory in image.\n", argv[0]);
		printf("%s [image filename] -a [file] [dest file] - add file to image.\n", argv[0]);
		printf("%s [image filename] -e [file] [dest file] - unpack file from image.\n", argv[0]);
		printf("%s [image filename] -comp [file] [dest file] - compress file in archive.\n", argv[0]);
		printf("%s [image filename] -uncomp [file] [dest file] - decompress file in archive.\n", argv[0]);
		printf("%s [image filename] -format - format the disk", argv[0]);
		return 0;
	}

	char szKey[512];

	printf("Please enter image key: ");
	gets(szKey);
	printf("\n\n");

	pInterface->SetKey(szKey);

	if(strcmp(argv[1], "-c") == false)
	{
		if(argc == 4)
			pInterface->CreateDisk(argv[2], atoi(argv[3]));
		return 0;
	}
	
	pInterface->ConnectDisk(argv[1]);

	if(!pInterface->IsDiskConnected())
	{
		printf("Error connecting disk.\n");
		return 0;
	}

	if(strcmp(argv[2], "-ls") == false)
	{
		if(argc > 3)
			read_dirs(argv[4]);
		else
			read_dirs("/");
	}
	else if(strcmp(argv[2], "-e") == false)
	{
		if(argc == 5)
		{
			extract_(argv[3], argv[4], false, true);
		}
		else if(argc == 4)
		{
			read_dirs("/", false, true, argv[3]);
		}
		else if(argc == 3)
		{
			read_dirs("/", false, true, "extracted");
		}
	}
	else if(strcmp(argv[2], "-a") == false)
	{
		if(argc == 5)
		{
			add_(argv[3], argv[4]);
		}
	}
	else if(strcmp(argv[2], "-comp") == false)
	{
		if(argc == 5)
		{
			comp(argv[3], argv[4]);
		}
	}
	else if(strcmp(argv[2], "-uncomp") == false)
	{
		if(argc == 5)
		{
			uncomp(argv[3], argv[4]);
		}
	}
	else if(strcmp(argv[2], "-mkdir") == false)
	{
		if(argc == 4)
		{
			vfs_createdirectory(argv[3]);
		}
	}
	else if(strcmp(argv[2], "-format") == false)
	{
		pInterface->FormatDisk();
	}

	pInterface->DisconnectDisk();
}
