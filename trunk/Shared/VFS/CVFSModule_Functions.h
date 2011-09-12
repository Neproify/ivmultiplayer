//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFSModule_Functions.h
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//==============================================================================


#ifdef VFS_MAP_CNAMES

#define FILE VFS_FILE

#define fopen CVFSModule::GetInterface()->GetFunctions()->f_fopen
#define fclose CVFSModule::GetInterface()->GetFunctions()->f_fclose
#define fflush CVFSModule::GetInterface()->GetFunctions()->f_fflush
#define fgetc CVFSModule::GetInterface()->GetFunctions()->f_fgetc
#define fputc CVFSModule::GetInterface()->GetFunctions()->f_fputc
#define fputs CVFSModule::GetInterface()->GetFunctions()->f_fputs
#define fwrite CVFSModule::GetInterface()->GetFunctions()->f_fwrite
#define fread CVFSModule::GetInterface()->GetFunctions()->f_fread
#define fseek CVFSModule::GetInterface()->GetFunctions()->f_fseek
#define fgetpos CVFSModule::GetInterface()->GetFunctions()->f_fgetpos
#define ftell CVFSModule::GetInterface()->GetFunctions()->f_ftell
#define feof CVFSModule::GetInterface()->GetFunctions()->f_feof
#define remove CVFSModule::GetInterface()->GetFunctions()->f_remove
#define opendir CVFSModule::GetInterface()->GetFunctions()->f_opendir
#define readdir CVFSModule::GetInterface()->GetFunctions()->f_readdir
#define closedir CVFSModule::GetInterface()->GetFunctions()->f_closedir
#define createdirectory CVFSModule::GetInterface()->GetFunctions()->f_createdirectory
#define is_dir CVFSModule::GetInterface()->GetFunctions()->f_is_dir 

#else

#define vfs_fopen CVFSModule::GetInterface()->GetFunctions()->f_fopen
#define vfs_fclose CVFSModule::GetInterface()->GetFunctions()->f_fclose
#define vfs_fflush CVFSModule::GetInterface()->GetFunctions()->f_fflush
#define vfs_fgetc CVFSModule::GetInterface()->GetFunctions()->f_fgetc
#define vfs_fputc CVFSModule::GetInterface()->GetFunctions()->f_fputc
#define vfs_fputs CVFSModule::GetInterface()->GetFunctions()->f_fputs
#define vfs_fwrite CVFSModule::GetInterface()->GetFunctions()->f_fwrite
#define vfs_fread CVFSModule::GetInterface()->GetFunctions()->f_fread
#define vfs_fseek CVFSModule::GetInterface()->GetFunctions()->f_fseek
#define vfs_fgetpos CVFSModule::GetInterface()->GetFunctions()->f_fgetpos
#define vfs_ftell CVFSModule::GetInterface()->GetFunctions()->f_ftell
#define vfs_feof CVFSModule::GetInterface()->GetFunctions()->f_feof
#define vfs_remove CVFSModule::GetInterface()->GetFunctions()->f_remove
#define vfs_opendir CVFSModule::GetInterface()->GetFunctions()->f_opendir
#define vfs_readdir CVFSModule::GetInterface()->GetFunctions()->f_readdir
#define vfs_closedir CVFSModule::GetInterface()->GetFunctions()->f_closedir
#define vfs_createdirectory CVFSModule::GetInterface()->GetFunctions()->f_createdirectory
#define vfs_is_dir CVFSModule::GetInterface()->GetFunctions()->f_is_dir 

#endif