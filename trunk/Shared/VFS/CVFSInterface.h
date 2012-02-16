//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFSInterface.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//============================================================================== 

#pragma once

#include "CVFSInterface_Common.h"

#ifndef VFS_INTERNAL

#define VFS_MAX_LONG_FILENAME				260 // should be increased in the fat-io library too

struct cluster_lookup
{
	int ClusterIdx;
	int CurrentCluster;
};

struct sector_buffer;

struct sector_buffer
{
	unsigned char			sector[512];
	int					address; 
	int						dirty;

	// Next in chain of sector buffers
	struct sector_buffer  *next;
};

typedef struct sVFS_FILE
{
	unsigned long			parentcluster;
	unsigned long			startcluster;
	unsigned long			bytenum;
	unsigned long			filelength;
	int						filelength_changed;
	char					path[VFS_MAX_LONG_FILENAME];
	char					filename[VFS_MAX_LONG_FILENAME];
	unsigned char			shortfilename[11];

	// Cluster Lookup
	struct cluster_lookup	last_fat_lookup;

	// Read/Write sector buffer
	struct sector_buffer	file_data;

	// File fopen flags
	unsigned char			flags;
#define FILE_READ	(1 << 0)
#define FILE_WRITE	(1 << 1)
#define FILE_APPEND	(1 << 2)
#define FILE_BINARY	(1 << 3)
#define FILE_ERASE	(1 << 4)
#define FILE_CREATE	(1 << 5)

	struct sVFS_FILE			*next;
} VFS_FILE;


struct fs_dir_list_status
{
	int						sector;
	int						cluster;
	unsigned char			offset;
};

struct fs_dir_ent
{
	char					filename[VFS_MAX_LONG_FILENAME];
	unsigned char			is_dir;
	int						cluster;
	int						size;
};

typedef struct fs_dir_list_status	VFS_DIR;
typedef struct fs_dir_ent			vfs_dirent;
#else
#define VFS_DIR FL_DIR
#define vfs_dirent fl_dirent
#endif

typedef void*				(* fopen_t)(const char *path, const char *modifiers);
typedef void				(* fclose_t)(void *file);
typedef int					(* fflush_t)(void *file);
typedef int					(* fgetc_t)(void *file);
typedef int					(* fputc_t)(int c, void *file);
typedef int					(* fputs_t)(const char * str, void *file);
typedef int					(* fwrite_t)(const void * data, int size, int count, void *file );
typedef int					(* fread_t)(void * data, int size, int count, void *file );
typedef int					(* fseek_t)(void *file , long offset , int origin );
typedef int					(* fgetpos_t)(void *file , unsigned long * position);
typedef long				(* ftell_t)(void *f);
typedef int					(* feof_t)(void *f);
typedef int					(* remove_t)( const char * filename );	



typedef VFS_DIR*			(* opendir_t)(const char* path, VFS_DIR *dir);
typedef int					(* readdir_t)(VFS_DIR *dirls, vfs_dirent *entry);
typedef int					(* closedir_t)(VFS_DIR* dir);

typedef int					(* createdirectory_t)(const char *path);
typedef int					(* is_dir_t)(const char *path);

struct SVFSFunctions
{
	fopen_t f_fopen;
	fclose_t f_fclose;
	fflush_t f_fflush;
	fgetc_t f_fgetc;
	fputc_t f_fputc;
	fputs_t f_fputs;
	fwrite_t f_fwrite;
	fread_t f_fread;
	fseek_t f_fseek;
	fgetpos_t f_fgetpos;
	ftell_t f_ftell;
	feof_t f_feof;
	remove_t f_remove;
	
	opendir_t f_opendir;
	readdir_t f_readdir;
	closedir_t f_closedir;
	createdirectory_t f_createdirectory;
	is_dir_t f_is_dir;
};

class CVFSInterface
{
public:
	virtual SVFSFunctions * GetFunctions() = 0;
	virtual void SetKey(char * szKey) = 0;
	virtual void ConnectDisk(const char * szFileName) = 0;
	virtual void DisconnectDisk() = 0;
	virtual void CreateDisk(const char *szFileName, int sizeInMB) = 0;
	virtual void FormatDisk() = 0;
	virtual bool IsDiskConnected() = 0;
};
