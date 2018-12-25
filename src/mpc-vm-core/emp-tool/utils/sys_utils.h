#pragma once


#include <string>
#include <time.h>
#include <cstdint>

#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <Winsock2.h>
#else
#include <sys/time.h>
#endif


namespace platon
{
	namespace mpc
	{

		class SysUtils
		{
		public:
			static void ms_sleep(unsigned long milisec);

			static char* getErrStr(int error_no);
			static char* getErrStr();
			static int getErrno();

			static uint64_t getSystemTime(struct timeval *tp, void *tzp);

			static uint64_t getSysCurrSec();

			//检查目录是否存在
			static bool checkFileExists(const std::string& filePath);
			//检查文件是否存在
			static bool checkDirExists(const std::string& dirPath);
			//创建文件夹
			static bool createFileDir(const std::string& dirPath);
			//获取文件大小
			static uint64_t getFileSize(const char* szFile);
			//获取文件更新数据
			static uint64_t getFileUpdateTime(const char* szFile);

			static int getTid();

		};

	}
}
