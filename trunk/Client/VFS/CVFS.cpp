//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFS.cpp
// Project: Client.VFS
// Author(s): jenksta
// License: See LICENSE in root directory
//
//============================================================================== 

#include <stdio.h>
#include <string.h>
#ifndef LINUX
#include <direct.h>
#endif
#ifndef _MSC_VER
#include <sys/io.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include "Rijndael.h"
#include <stdlib.h>


#include <time.h>

CRijndael encAES;

unsigned char g_buf[513];

extern "C"
{
	#include <fat_filelib.h>
	#include <fat_format.h>
}

#define VFS_INTERNAL
#include "CVFS.h"

int disk = 0;

#define AES_ENCRYPTION
#define VFS_VERSION 0x01

struct vfs_header
{
	unsigned char version;
	char header[64];
	unsigned char encryption;
};

int media_init(const char * szDisk)
{
#ifndef LINUX
	disk = open(szDisk, O_RDWR | O_BINARY);
#else
	disk = open(szDisk, O_RDWR);
#endif
	return 1;
}

int media_read(unsigned long sector, unsigned char *buffer)
{
    if (lseek(disk, (sector*FAT_SECTOR_SIZE)+sizeof(vfs_header), SEEK_SET) == -1L)
    {
       printf("Read seek fail\n");
       return -1; 
    }
	
	read(disk, buffer, FAT_SECTOR_SIZE);
#ifdef XOR_ENCRYPTION
	for(int i = 0; i < FAT_SECTOR_SIZE; i++)
		buffer[i] ^= xorkey_512[i];
#else if AES_ENCRYPTION
	encAES.ResetChain();
	encAES.Decrypt((char*)buffer, (char*)g_buf, FAT_SECTOR_SIZE);
	memcpy(buffer, g_buf, FAT_SECTOR_SIZE);
#endif
	return 1;
}

int media_write(unsigned long sector, unsigned char *buffer)
{
    if (lseek(disk, (sector*FAT_SECTOR_SIZE)+sizeof(vfs_header), SEEK_SET) == -1L)
    {
       printf("Read seek fail\n");
       return -1; 
    }

#ifdef XOR_ENCRYPTION
	for(int i = 0; i < FAT_SECTOR_SIZE; i++)
		buffer[i] ^= xorkey_512[i];
#else if AES_ENCRYPTION
	encAES.ResetChain();
	encAES.Encrypt((char*)buffer, (char*)g_buf, FAT_SECTOR_SIZE);
	memcpy(buffer, g_buf, FAT_SECTOR_SIZE);
#endif
	write(disk, buffer, FAT_SECTOR_SIZE);
#ifdef XOR_ENCRYPTION
	for(int i = 0; i < FAT_SECTOR_SIZE; i++)
		buffer[i] ^= xorkey_512[i];
#else if AES_ENCRYPTION
	encAES.ResetChain();
	encAES.Decrypt((char*)buffer, (char*)g_buf, FAT_SECTOR_SIZE);
	memcpy(buffer, g_buf, FAT_SECTOR_SIZE);
#endif
	return 1;
}

void generate_xor(const char *szFileName, int num)
{
	FILE * f = fopen(szFileName, "wb");
	srand((unsigned int)time(NULL));
	fprintf(f, "unsigned char xorkey_%d[] = { \n", num);
	for(int i = 0; i < num; i++)
	{
		fprintf(f, " 0x%x,", rand() % 256);
	}
	fprintf(f, "\n }; \n\n");
	fclose(f);
}

CVFS::CVFS()
{
	m_funcs = new SVFSFunctions;
	memset(m_funcs, 0, sizeof(SVFSFunctions));

	m_funcs->f_fopen = fl_fopen;
	m_funcs->f_fclose = fl_fclose;
	m_funcs->f_fflush = fl_fflush;
	m_funcs->f_fgetc = fl_fgetc;
	m_funcs->f_fputc = fl_fputc;
	m_funcs->f_fputs = fl_fputs;
	m_funcs->f_fwrite = fl_fwrite;
	m_funcs->f_fread = fl_fread;
	m_funcs->f_fseek = fl_fseek;
	m_funcs->f_fgetpos = fl_fgetpos;
	m_funcs->f_ftell = fl_ftell;
	m_funcs->f_feof = fl_feof;
	m_funcs->f_remove = fl_remove;

	m_funcs->f_opendir = fl_opendir;
	m_funcs->f_readdir = fl_readdir;
	m_funcs->f_closedir = fl_closedir;
	m_funcs->f_createdirectory = fl_createdirectory;
	m_funcs->f_is_dir = fl_is_dir;

	m_bConnected = false;
}

CVFS::~CVFS()
{
	this->DisconnectDisk();
}

void CVFS::SetKey(char * szKey)
{
	char key[32];
	strcpy(key, szKey);
	if(strlen(key) != 16)
	{
		//printf("Invalid key.\n");
		//return 0;
		int len = strlen(key);
		if(len > 16)
			key[16] = 0;
		else
		{
			for(int i = 0; i < 16-len; i++)
			{
				key[i+len] = '0';
			}
		}
		key[16] = 0;
	}

	encAES.MakeKey(key, CRijndael::sm_chain0, 16, 16);
}

void CVFS::ConnectDisk(const char * szFileName)
{
	media_init(szFileName);

	fl_init();

	if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
	{
		//printf("ERROR: Media attach failed\n");
		return; 
	}
	m_bConnected = true;
}

void CVFS::DisconnectDisk()
{
	fl_shutdown();
	close(disk);
}

void CVFS::CreateDisk(const char *szFileName, int mbSize)
{
	if(m_bConnected || mbSize > 2048)
		return;
	FILE * f = fopen(szFileName, "wb");
	if(!f)
		return;

	// create header
	vfs_header header;
	header.version = VFS_VERSION;
	strcpy(header.header, "Our VFS // Kye fag // dont try to decrypt me");
	// write header
	fwrite(&header, sizeof(vfs_header), 1, f);
	fclose(f);

	// create image
	media_init(szFileName);
	fl_get_fs()->disk_io.read_sector = media_read;
	fl_get_fs()->disk_io.write_sector = media_write;
	fatfs_format_fat16(fl_get_fs(), mbSize*2048, "vfs");
}

void CVFS::FormatDisk()
{
	if(m_bConnected)
		fatfs_format_fat16(fl_get_fs(), 1048576, "vfs");
}

void CVFS::DecryptDisk(const char * szNewFileName)
{

}
