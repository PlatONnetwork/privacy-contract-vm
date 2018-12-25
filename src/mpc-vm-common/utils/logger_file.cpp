#include "logger_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "sys_utils_tool.h"
#include "platon_logger.h"
#include <errno.h>
#include <cstddef>


NS_PLATON_MPC_BEG

const static int MS_MAX_LOG_FILE_SIZE = (512 * 1024);
static char sCurrentLogFile[256];
static char sCurrentLogDir[256];

static const char* sLogFileName = "platon_mpc";
static FILE*	sLogFile = nullptr;

FILE* initLoggerFile(const char *pPathDir)
{
	printf("\n++++++++++++++++++      initLoggerFile +++++++++++++++++++ \n \n");
	char chSlash = '/';
#if defined(_WIN32) || defined(_WIN64)
	chSlash = '\\';
#endif//

	getLogFilePath(pPathDir, sCurrentLogFile);
	sLogFile = fopen(sCurrentLogFile, "a");
	if (sLogFile == NULL)
	{
		fprintf(stderr, "cannot create file: %s, errno=%d", sCurrentLogFile, errno);
		return nullptr;
	}
	std::setvbuf(sLogFile, NULL, _IONBF, 0);
	std::setvbuf(stdout, NULL, _IONBF, 0);
	std::setvbuf(stderr, NULL, _IONBF, 0);
	return sLogFile;
}

int uninitLoggerFile()
{
	if (sLogFile)
	{
		fclose(sLogFile);
		sLogFile = nullptr;
	}

	return 0;
}


int writeLoggerFile(const char *pLog)
{
	if (sLogFile == nullptr)
	{
		initLoggerFile("./");
	}

	FILE* fp = sLogFile;

#if defined(_WIN32) || defined(_WIN64)
	fprintf(fp, "%s \r\n", pLog);
#else
	fprintf(fp, "%s \n", pLog);
#endif//

	return 0;
}

void writeTerminal(int level, const char* pLog)
{
	if (!pLog)
	{
		return;
	}

#if defined(_WIN32) || defined(_WIN64)
	fprintf(stdout, "%s \r\n", pLog);
#else
	fprintf(stdout, "%s \n", pLog);
#endif//

	//fflush(stdout);
	return;
}

//log 文件夹完整路径
int getLogWorkDir(char* szPath, int size)
{
	//get writeable path
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
	_getcwd(szPath, size);
	strncat(szPath, "\\log", strlen("\\log"));
#else//linux, android
	getcwd(szPath, size);
	strncat(szPath, "/log", strlen("/log"));
#endif

	return 0;
}

int getLogFilePath(const char *szDir, char* szFilePath)
{
	std::string strFile;
	strFile.assign(szDir);

#if defined(_WIN32) || defined(_WIN64)
	strFile.append("\\");
#else
	strFile.append("/");
#endif//

	char szBuffer[256];
	sprintf(szBuffer, "%s_%d.log", sLogFileName, SysUtilsTool::getPid());

	strFile += szBuffer;
	strcpy(szFilePath, strFile.data());
	return 0;
}
NS_PLATON_MPC_END
