#include "sys_utils_tool.h"


#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <Windows.h>
#include <process.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#endif//


#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <string>


NS_PLATON_MPC_BEG

void SysUtilsTool::ms_sleep(unsigned long milisec)
{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
	Sleep(milisec);
#else
	usleep(1000 * milisec);
#endif//
}

int SysUtilsTool::getTid()
{
#if defined(_WIN32) || defined(_WIN64)
	return (long)GetCurrentThreadId();
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__ANDROID__)
	void* ptr;
	int nBits = sizeof(ptr);
	if (nBits == 4)
		return (long int)syscall(224);
	else//64bits
		return (long int)syscall(186);
#else
	return (long)getpid();
#endif//
}

int SysUtilsTool::getPid()
{
#if defined(_WIN32) || defined(_WIN64)
	return _getpid();
#else
	return (int)getpid();
#endif//
}

char* SysUtilsTool::getErrStr(int error_no)
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
	static char lpMsgBuf[128];
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&lpMsgBuf,
		sizeof(lpMsgBuf), NULL);

	return lpMsgBuf;
#else
	return strerror(error_no);
#endif
}

char* SysUtilsTool::getErrStr()
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
	static char lpMsgBuf[128];
	FormatMessage(
		/*FORMAT_MESSAGE_ALLOCATE_BUFFER | */FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&lpMsgBuf,
		sizeof(lpMsgBuf), NULL);

	return lpMsgBuf;
#else
	return strerror(errno);
#endif
}

int SysUtilsTool::getErrno()
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
	return GetLastError();
#else
	return errno;
#endif//
}

uint64_t  SysUtilsTool::getSystemTime(struct timeval *tp, void *tzp)
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = (long)wtm.wMilliseconds * 1000;
	return 0;
#else
	return gettimeofday(tp, NULL);
#endif//
}

uint64_t  SysUtilsTool::getSysCurrSec()
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);

	return clock;
#else
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec;
#endif//
}


//获取文件大小
uint64_t SysUtilsTool::getFileSize(const char* szFile)
{
	if (!SysUtilsTool::checkFileExists(szFile))
		return -1;

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	return getAppleFileSize(szFile);
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
	if (TRUE != GetFileAttributesExA(szFile, GetFileExInfoStandard, (void*)&fileInfo))
		return -1;

	LARGE_INTEGER size;
	size.HighPart = fileInfo.nFileSizeHigh;
	size.LowPart = fileInfo.nFileSizeLow;
	return size.QuadPart;

#else//linux, android
	struct stat stFile;
	if (stat(szFile, &stFile) != 0)
	{
		return -1;
	}

	return stFile.st_size;
#endif
}

//获取文件更新数据
uint64_t SysUtilsTool::getFileUpdateTime(const char* szFile)
{
	if (!SysUtilsTool::checkFileExists(szFile))
		return -1;

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	return getAppleFileUpdateTime(szFile);
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
	if (TRUE != GetFileAttributesExA(szFile, GetFileExInfoStandard, (void*)&fileInfo))
		return -1;

	SYSTEMTIME objSysTime;
	FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &objSysTime);

	time_t nTimeStamp;
	struct tm tm;

	GetLocalTime(&objSysTime);
	tm.tm_year = objSysTime.wYear - 1900;
	tm.tm_mon = objSysTime.wMonth - 1;
	tm.tm_mday = objSysTime.wDay;
	tm.tm_hour = objSysTime.wHour;
	tm.tm_min = objSysTime.wMinute;
	tm.tm_sec = objSysTime.wSecond;
	tm.tm_isdst = -1;
	nTimeStamp = mktime(&tm);

	return nTimeStamp;
#else//linux, android
	struct stat   stFile;
	if (stat(szFile, &stFile) != 0)
	{
		return -1;
	}

	return stFile.st_mtime;
#endif//
}

//检查目录是否存在
bool SysUtilsTool::checkFileExists(const std::string& filePath)
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	DWORD fileType = GetFileAttributesA(filePath.c_str());
	if (fileType == INVALID_FILE_ATTRIBUTES)
		return false;//something is wrong with your path!

	return true;
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	int ret = checkAppleFileExists(filePath.c_str());
	return ret == 1 ? true : false;

#else
	int         iRet;
	struct stat stDir;

	iRet = stat(filePath.c_str(), &stDir);
	if (iRet != 0)
	{
		return false;
	}

	if (stDir.st_mode & S_IFREG)//常规文件
	{
		return true;
	}

	return false;
#endif//
}

//检查文件是否存在
bool SysUtilsTool::checkDirExists(const std::string& dirPath)
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	DWORD fileType = GetFileAttributesA(dirPath.c_str());
	if (fileType == INVALID_FILE_ATTRIBUTES)
		return false;//something is wrong with your path!

	if (fileType & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;// this is not a directory!

#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	int ret = checkAppleDirExists(dirPath.c_str());
	return ret == 1 ? true : false;

#else
	int			ret;
	struct	stat stDir;

	ret = stat(dirPath.c_str(), &stDir);
	if (ret != 0)
	{
		return false;
	}

	if (stDir.st_mode & S_IFDIR)
	{
		return true;
	}

	return false;
#endif//
}

bool SysUtilsTool::createFileDir(const std::string& dirPath)
{
	if (dirPath.size() <= 0)
		return false;

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	if (FALSE == CreateDirectoryA(dirPath.c_str(), NULL))
		return false;
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	if (0 != createAppleMsDir(dirPath.c_str()))
		return false;
#else//android,  linux
	if (0 != mkdir(dirPath.c_str(), 0755))
		return false;
#endif

	return true;
}
NS_PLATON_MPC_END
