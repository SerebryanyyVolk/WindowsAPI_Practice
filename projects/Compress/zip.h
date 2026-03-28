#ifndef ZIP_H
#define ZIP_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

typedef enum eZipPathType
{
	Path_NotExist,
	Path_File,
	Path_Directory,
	Path_Error
};

typedef enum eZipErrCode{
	eZip_OK,
	eZip_ErrSrcFile,//源文件无法正常打开
	eZip_ErrTgtFile,//目标文件无法正常打开或写入
	eZip_ErrTgtClose,//目标文件流无法正常关闭
	eZip_ErrUnknown,
};

typedef struct zip_t zip_t;

zip_t *zip_open(const char *filename);
int zip_write(zip_t *zip, const char *filename, const void *data, size_t size);
void zip_close(zip_t *zip);

eZipPathType GetPathType(LPTSTR path);
void GetDirectoryFromFilePath(LPTSTR filePath, LPTSTR outDir);
void GetFileNameWithoutExt(LPTSTR fullPath, LPTSTR outFileNameNoExt);

eZipErrCode CreateZipFromFile(LPTSTR src_file_path, LPTSTR zip_output_path);
eZipErrCode CreateZipFromFolder(LPTSTR src_folder_path, LPTSTR zip_output_path);



#endif
