#include "platon_logger.h"
#include "sys_utils_tool.h"
#include "logger_file.h"
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <Winsock2.h>
#include <direct.h>
#elif defined(__ANDROID__)
#include <android/log.h>
#elif defined(__linux__) || defined(__linux) || defined(linux)
#include <unistd.h>
//#elif (defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__))
//#include "MSIOS.h"
#endif

NS_PLATON_MPC_BEG


static char sMessage[4096] = { 0 };
static char sPath[256] = { 0 };
static std::recursive_mutex sLocker;

static const int FORMAT_STRING_LEN = strlen("%04d-%02d-%02d %02d:%02d:%02d :");

PlatonLogger::PlatonLogger(int type) : m_level(PlatonLogger::LOG_INFO), m_type(type)
{
	initLog();
}

PlatonLogger::~PlatonLogger()
{
	uninitLoggerFile();
}

PlatonLogger* PlatonLogger::getInstance()
{
	static PlatonLogger theManager;
	return &theManager;
}

void PlatonLogger::setLogLevel(int level)
{
	std::lock_guard<std::recursive_mutex> autGuard(sLocker);
	if (level >= PlatonLogger::LOG_TRACE && level <= PlatonLogger::LOG_FATAL)
		m_level = level;
}

void PlatonLogger::initLog()
{
	printf("\n++++++++++++++++++      PlatonLogger::initLog +++++++++++++++++++ \n \n");
	std::lock_guard<std::recursive_mutex> autGuard(sLocker);

#if defined(DEBUG) || defined(_DEBUG)
	m_level = PlatonLogger::LOG_DEBUG;
#else
	/*bool bEnableShowLog = MsRunConfig::getShowlogEnable();
	if (bEnableShowLog)
	{*/
	m_level = PlatonLogger::LOG_INFO;
	//}
#endif//

	memset(sPath, 0, sizeof(sPath));
	//get writeable path
	if (0 != getLogWorkDir(sPath, sizeof(sPath)))
		return;

	//create ./log directory
	if (!SysUtilsTool::checkDirExists(sPath))
	{
		if (!SysUtilsTool::createFileDir(sPath))
			return;
	}

	//init log files
	if (strlen(sPath) > 0)
	{
		logStream = initLoggerFile(sPath);
		if (nullptr == logStream)
		{
			printf("init logger file failed\n");
		}
	}
}

const char* PlatonLogger::getLevelTag(int level)
{
	const char* pTag;
	switch (level)
	{
	case PlatonLogger::LOG_DEBUG:
		pTag = "DEBUG";
		break;

	case PlatonLogger::LOG_INFO:
		pTag = "INFO";
		break;

	case PlatonLogger::LOG_WARN:
		pTag = "WARN";
		break;

	case PlatonLogger::LOG_ERROR:
		pTag = "ERROR";
		break;

	case PlatonLogger::LOG_TRACE:
		pTag = "TRACE";
		break;

	default:
		pTag = "INFO";
		break;
	}

	return pTag;
}

const char* PlatonLogger::getDateString()
{
	static thread_local char s_dateStr[128] = { 0 };

	time_t nTime = time(NULL);
	struct tm *tp = (struct tm *)localtime(&nTime);

	struct timeval objTime;
	SysUtilsTool::getSystemTime(&objTime, NULL);

	int tid = SysUtilsTool::getTid();

	//std::lock_guard<std::recursive_mutex> objLock(sLocker);
	snprintf(s_dateStr, sizeof(s_dateStr), "[%02d:%02d:%02d.%03d] [%-5d]", \
		tp->tm_hour, tp->tm_min, tp->tm_sec, (int)objTime.tv_usec / 1000, \
		tid);

	return s_dateStr;
}

void PlatonLogger::logFormat(int level, const char* file, const char* func, int line, const char *fmt, ...)
{
	if (level < m_level)
		return;

	char fmtMsg[4096] = { 0 };
	memset(fmtMsg, 0, 4096);
	va_list argp;
	va_start(argp, fmt);
	vsprintf(fmtMsg, fmt, argp);
	va_end(argp);

	time_t nTime = time(NULL);
	struct tm *tp = (struct tm *)localtime(&nTime);

	struct timeval objTime;
	SysUtilsTool::getSystemTime(&objTime, NULL);

	int tid = SysUtilsTool::getTid();

	std::lock_guard<std::recursive_mutex> objLock(sLocker);
    char _sMessage[4096] = { 0 };
	snprintf(_sMessage, sizeof(_sMessage), "[%02d:%02d:%02d.%03d] [%-5d] [%-5s] [%s:%d:%s]\t%s", \
		tp->tm_hour, tp->tm_min, tp->tm_sec, (int)objTime.tv_usec / 1000, \
		tid, PlatonLogger::getLevelTag(level), file, line, func, fmtMsg);

	writeLoggerFile(_sMessage);

#if (defined(DEBUG) || defined(NDEBGU) || defined(_DEBUG))
	printf("%s\n", fmtMsg);
#endif
}
NS_PLATON_MPC_END
