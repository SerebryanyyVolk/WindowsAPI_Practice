#include "zip.h"
#include <vector>

struct zip_t
{
	FILE *fp;
};

typedef struct
{
	unsigned int sig;
	unsigned short version;
	unsigned short flags;
	unsigned short method;
	unsigned short time;
	unsigned short date;
	unsigned int crc;
	unsigned int csize;
	unsigned int usize;
	unsigned short fname_len;
	unsigned short extra_len;
} local_hdr_t;

typedef struct
{
	unsigned int sig;
	unsigned short version;
	unsigned short version_needed;
	unsigned short flags;
	unsigned short method;
	unsigned short time;
	unsigned short date;
	unsigned int crc;
	unsigned int csize;
	unsigned int usize;
	unsigned short fname_len;
	unsigned short extra_len;
	unsigned short comment_len;
	unsigned short disk;
	unsigned short attr;
	unsigned int attr_ex;
	unsigned int offset;
} central_hdr_t;

typedef struct
{
	unsigned int sig;
	unsigned short disk;
	unsigned short disk_cd;
	unsigned short entries;
	unsigned short entries_total;
	unsigned int size;
	unsigned int offset;
	unsigned short comment_len;
} end_hdr_t;

struct ZipCentralEntry
{
	long localPos;        // 本地文件头偏移
	DWORD crc;            // CRC32
	long fileSize;        // 文件大小
	WORD fnameLen;        // 文件名长度
	char fileName[512];   // 文件名
};

static unsigned int crc32(const unsigned char *data, size_t len)
{
	unsigned int c = 0xFFFFFFFF;
	for (size_t i = 0; i < len; i++)
	{
		c ^= data[i];
		for (int j = 0; j < 8; j++)
			c = (c >> 1) ^ ((c & 1) ? 0xEDB88320 : 0);
	}
	return c ^ 0xFFFFFFFF;
}

zip_t *zip_open(const char *filename)
{
	zip_t *z = (zip_t *)malloc(sizeof(zip_t));
	z->fp = fopen(filename, "wb");
	return z;
}

int zip_write(zip_t *zip, const char *filename, const void *data, size_t size)
{
	local_hdr_t lh = {0};
	lh.sig = 0x04034B50;
	lh.method = 0;
	lh.crc = crc32((const unsigned char *)data, size);
	lh.csize = size;
	lh.usize = size;
	lh.fname_len = (unsigned short)strlen(filename);

	fwrite(&lh, 1, 30, zip->fp);
	fwrite(filename, 1, lh.fname_len, zip->fp);
	fwrite(data, 1, size, zip->fp);

	central_hdr_t ch = {0};
	ch.sig = 0x02014B50;
	ch.method = 0;
	ch.crc = lh.crc;
	ch.csize = size;
	ch.usize = size;
	ch.fname_len = lh.fname_len;
	ch.offset = 0;

	fwrite(&ch, 1, 46, zip->fp);
	fwrite(filename, 1, lh.fname_len, zip->fp);

	return 1;
}

void zip_close(zip_t *zip)
{
	end_hdr_t eh = {0};
	eh.sig = 0x06054B50;
	eh.entries = 1;
	eh.entries_total = 1;
	eh.size = 46 + (unsigned short)strlen("test.txt");
	eh.offset = 30 + (unsigned short)strlen("test.txt") + 100;

	fwrite(&eh, 1, 22, zip->fp);
	fclose(zip->fp);
	free(zip);
}

PathType GetPathType(LPTSTR path)
{
	DWORD attr = GetFileAttributes(path);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND)
			return Path_NotExist;
		else
			return Path_Error;
	}

	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return Path_Directory;
	else
		return Path_File;
}

void GetDirectoryFromFilePath(LPTSTR fullpath, LPTSTR outDir)
{
	TCHAR diskW[20] = { 0 };
	TCHAR pathW[MAX_PATH] = {0};
	_wsplitpath_s(
		fullpath,
		diskW, 20,     // 盘
		pathW, MAX_PATH,     // 路径
		NULL, 0, // 文件名
		NULL, 0      // 后缀
		);
	_stprintf_s(outDir, MAX_PATH, TEXT("%s%s"), diskW, pathW);
}

void GetFileNameWithoutExt(LPTSTR fullPath, LPTSTR outFileNameNoExt)
{
	TCHAR fileName[MAX_PATH];
	_wsplitpath_s(
		fullPath,
		NULL, 0,     // 盘
		NULL, 0,     // 路径
		fileName, MAX_PATH,     // 文件名
		NULL, 0      // 后缀
		);
	_stprintf_s(outFileNameNoExt, MAX_PATH, TEXT("%s"), fileName);
}

bool CreateZipFromFile(LPTSTR src_file_path, LPTSTR zip_output_path)
{
	// 打开原文件
	FILE* f = _wfopen(src_file_path, L"rb");
	if (!f)
		return FALSE;

	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char* fileData = (unsigned char*)malloc(fileSize);
	fread(fileData, 1, fileSize, f);
	fclose(f);

	// ==============================
	// 提取原文件文件名
	// ==============================
	TCHAR fileNameW[MAX_PATH] = { 0 };
	TCHAR extW[40] = {0};
	_wsplitpath_s(
		src_file_path,
		NULL, 0,     // 盘
		NULL, 0,     // 路径
		fileNameW, MAX_PATH, // 文件名
		extW, 40      // 后缀
		);
	lstrcat(fileNameW, extW); 

	// 把文件名转成窄字符
	char fileName[MAX_PATH*2] = { 0 };

	WideCharToMultiByte(CP_ACP, 0, fileNameW, -1, fileName, MAX_PATH, NULL, NULL);
	unsigned short fnameLen = (unsigned short)strlen(fileName);

	// 打开输出 ZIP
	char zipPath[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, zip_output_path, -1, zipPath, MAX_PATH, NULL, NULL);

	FILE* zip = fopen(zipPath, "wb");
	if (!zip)
	{
		free(fileData);
		return FALSE;
	}

	unsigned int crc = crc32(fileData, fileSize);

	// ------------------------------
	// 本地文件头
	// ------------------------------
	unsigned char localHdr[30] = {
		0x50, 0x4B, 0x03, 0x04,
		0x14, 0x00,
		0x00, 0x00,
		0x00, 0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,
		0x00,0x00
	};
	memcpy(&localHdr[14], &crc, 4);
	memcpy(&localHdr[18], &fileSize, 4);
	memcpy(&localHdr[22], &fileSize, 4);
	localHdr[26] = fnameLen & 0xFF;
	localHdr[27] = (fnameLen >> 8) & 0xFF;

	long localPos = ftell(zip);
	fwrite(localHdr, 1, 30, zip);
	fwrite(fileName, 1, fnameLen, zip);
	fwrite(fileData, 1, fileSize, zip);

	// ------------------------------
	// 中央目录
	// ------------------------------
	unsigned char central[46] = {
		0x50, 0x4B, 0x01, 0x02,
		0x14,0x00, 0x14,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00
	};
	memcpy(&central[16], &crc, 4);
	memcpy(&central[20], &fileSize, 4);
	memcpy(&central[24], &fileSize, 4);
	central[28] = fnameLen & 0xFF;
	central[29] = (fnameLen >> 8) & 0xFF;
	memcpy(&central[42], &localPos, 4);

	long centralPos = ftell(zip);
	fwrite(central, 1, 46, zip);
	fwrite(fileName, 1, fnameLen, zip);
	long centralEnd = ftell(zip);

	// ------------------------------
	// 结束目录
	// ------------------------------
	unsigned int centralSize = (unsigned int)(centralEnd - centralPos);
	unsigned int centralOff = (unsigned int)centralPos;

	unsigned char end[22] = {
		0x50, 0x4B, 0x05, 0x06,
		0x00,0x00, 0x00,0x00,
		0x01,0x00, 0x01,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00
	};
	memcpy(&end[12], &centralSize, 4);
	memcpy(&end[16], &centralOff, 4);

	fwrite(end, 1, 22, zip);
	fclose(zip);
	free(fileData);

	return true;
}


// ==============================
// 工具函数：递归遍历文件夹并写入ZIP
// ==============================
// 递归遍历目录，只写本地文件头 + 文件数据，缓存中央目录信息
bool AddDirectoryToZip(
	FILE* zipFile, 
	LPTSTR baseDir, 
	LPTSTR currentDir,
	std::vector<ZipCentralEntry>& centralList,  // 缓存中央目录
	unsigned int& fileCount)
{
	TCHAR searchPath[MAX_PATH];
	_stprintf_s(searchPath, MAX_PATH, TEXT("%s\\*.*"), currentDir);

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(searchPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (_tcscmp(findData.cFileName, TEXT(".")) == 0 || 
			_tcscmp(findData.cFileName, TEXT("..")) == 0)
			continue;

		TCHAR fullPath[MAX_PATH];
		_stprintf_s(fullPath, MAX_PATH, TEXT("%s\\%s"), currentDir, findData.cFileName);

		// 递归子目录
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!AddDirectoryToZip(zipFile, baseDir, fullPath, centralList, fileCount))
			{
				FindClose(hFind);
				return false;
			}
		}
		else
		{
			// 打开文件
			FILE* f = _wfopen(fullPath, L"rb");
			if (!f) continue;

			fseek(f, 0, SEEK_END);
			long fileSize = ftell(f);
			fseek(f, 0, SEEK_SET);

			unsigned char* fileData = (unsigned char*)malloc(fileSize);
			if (!fileData) { fclose(f); continue; }
			fread(fileData, 1, fileSize, f);
			fclose(f);

			// 生成相对路径
			TCHAR zipRelativePath[MAX_PATH] = {0};
			unsigned int baseLen = _tcslen(baseDir);
			if (_tcslen(fullPath) > baseLen)
				_tcscpy_s(zipRelativePath, MAX_PATH, fullPath + baseLen + 1);
			else
				_tcscpy_s(zipRelativePath, MAX_PATH, findData.cFileName);

			// 转窄字符
			char zipFileName[512] = {0};
			WideCharToMultiByte(CP_ACP, 0, zipRelativePath, -1, zipFileName, 512, NULL, NULL);
			WORD fnameLen = (WORD)strlen(zipFileName);

			// CRC
			DWORD crc = crc32(fileData, fileSize);

			// ==========================
			// 写入：本地文件头 + 文件名 + 数据
			// ==========================
			unsigned char localHdr[30] = {
				0x50, 0x4B, 0x03, 0x04,
				0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,
				0x00,0x00, 0x00,0x00
			};
			memcpy(&localHdr[14], &crc, 4);
			memcpy(&localHdr[18], &fileSize, 4);
			memcpy(&localHdr[22], &fileSize, 4);
			localHdr[26] = fnameLen & 0xFF;
			localHdr[27] = (fnameLen >> 8) & 0xFF;

			long localPos = ftell(zipFile);
			fwrite(localHdr, 1, 30, zipFile);
			fwrite(zipFileName, 1, fnameLen, zipFile);
			fwrite(fileData, 1, fileSize, zipFile);

			// ==========================
			// 只缓存，不立即写中央目录
			// ==========================
			ZipCentralEntry entry;
			entry.localPos = localPos;
			entry.crc = crc;
			entry.fileSize = fileSize;
			entry.fnameLen = fnameLen;
			strcpy_s(entry.fileName, zipFileName);
			centralList.push_back(entry);

			free(fileData);
			fileCount++;
		}

	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);
	return true;
}

// ==============================
// 核心函数：压缩整个文件夹到ZIP
// ==============================
bool CreateZipFromFolder(LPTSTR src_folder_path, LPTSTR zip_output_path)
{
	if (!src_folder_path || !zip_output_path || _tcslen(src_folder_path) == 0)
		return false;

	TCHAR baseDir[MAX_PATH];
	_tcscpy_s(baseDir, src_folder_path);
	size_t len = _tcslen(baseDir);
	if (len > 0 && baseDir[len-1] == TEXT('\\'))
		baseDir[len-1] = 0;

	// 转窄字符路径
	char zipPath[MAX_PATH*2] = {0};
	WideCharToMultiByte(CP_ACP, 0, zip_output_path, -1, zipPath, MAX_PATH*2, NULL, NULL);

	FILE* zip = fopen(zipPath, "wb");
	if (!zip) return false;

	std::vector<ZipCentralEntry> centralList;
	unsigned int fileCount = 0;

	// ==========================
	// 第一步：写入所有文件（只写本地头+数据）
	// ==========================
	bool ret = AddDirectoryToZip(zip, baseDir, baseDir, centralList, fileCount);
	if (!ret || fileCount == 0)
	{
		fclose(zip);
		remove(zipPath);
		return false;
	}

	// ==========================
	// 第二步：统一写入所有中央目录
	// ==========================
	long centralStart = ftell(zip);

	for (std::vector<ZipCentralEntry>::iterator it = centralList.begin(); it != centralList.end(); ++it)
	{
		ZipCentralEntry e = *it;

		unsigned char central[46] = {
			0x50, 0x4B, 0x01, 0x02,
			0x14,0x00, 0x14,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00, 0x00,0x00,
			0x00,0x00
		};

		memcpy(&central[16], &e.crc, 4);
		memcpy(&central[20], &e.fileSize, 4);
		memcpy(&central[24], &e.fileSize, 4);
		central[28] = e.fnameLen & 0xFF;
		central[29] = (e.fnameLen >> 8) & 0xFF;
		memcpy(&central[42], &e.localPos, 4);

		fwrite(central, 1, 46, zip);
		fwrite(e.fileName, 1, e.fnameLen, zip);
	}

	// ==========================
	// 第三步：写入结束目录 EOCD
	// ==========================
	long centralEnd = ftell(zip);
	DWORD centralSize = (DWORD)(centralEnd - centralStart);
	DWORD centralOff = (DWORD)centralStart;

	unsigned char end[22] = {
		0x50, 0x4B, 0x05, 0x06,
		0x00,0x00, 0x00,0x00,
		0x00,0x00, 0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00
	};

	memcpy(end+8,  &fileCount, 2);
	memcpy(end+10, &fileCount, 2);
	memcpy(end+12, &centralSize, 4);
	memcpy(end+16, &centralOff, 4);

	fwrite(end, 1, 22, zip);
	fclose(zip);
	return true;
}



