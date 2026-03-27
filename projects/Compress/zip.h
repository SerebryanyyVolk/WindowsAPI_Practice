#ifndef ZIP_H
#define ZIP_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

typedef enum PathType
{
	Path_NotExist,
	Path_File,
	Path_Directory,
	Path_Error
};

typedef struct zip_t zip_t;

zip_t *zip_open(const char *filename);
int zip_write(zip_t *zip, const char *filename, const void *data, size_t size);
void zip_close(zip_t *zip);

PathType GetPathType(LPTSTR path);
void GetDirectoryFromFilePath(LPTSTR filePath, LPTSTR outDir);
void GetFileNameWithoutExt(LPTSTR fullPath, LPTSTR outFileNameNoExt);

bool CreateZipFromFile(LPTSTR src_file_path, LPTSTR zip_output_path);
bool CreateZipFromFolder(LPTSTR src_folder_path, LPTSTR zip_output_path);



#endif
