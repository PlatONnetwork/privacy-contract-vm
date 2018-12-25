#pragma once

#include <fstream>
#include <cstring>


#include "mpc_common.h"
NS_PLATON_MPC_BEG
class   PlatonLogger
{
public:
	enum LoggerLevel
	{
		LOG_TRACE = 0,
		LOG_DEBUG = 1,	//debug级别
		LOG_INFO = 2,	//通知显示级别
		LOG_WARN = 3,	//警告
		LOG_ERROR = 4,	//错误
		LOG_FATAL = 5//致命
	};

	enum OutputType
	{
		STD_OUT = 0,	//to stdout or stderr
		FILE_OUT = 1,	//to file
		NET_OUT = 2,	//to net stream
		INVALID_OUT = 3,	//错误
	};

public:
	~PlatonLogger();

private:
	PlatonLogger(int type = STD_OUT);
	PlatonLogger(const PlatonLogger& obj) = delete;
	PlatonLogger& operator=(const PlatonLogger& obj) = delete;

	void initLog();

public:
	static PlatonLogger* getInstance();

	FILE*  getStream()
	{
		return logStream;
	}

	void setLogLevel(int level);

	void logFormat(int level, const char* file, const char* func, int line, const char *fmt, ...);

	static const char* getDateString();

private:
	static const char* getLevelTag(int level);

private:
	int					m_level;
	int					m_type;
	FILE*				logStream = nullptr;

};
NS_PLATON_MPC_END

#ifdef WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif//FILENAME

#define LOGD(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define LOGI(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define LOGW(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_WARN,   __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define LOGE(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_ERROR,   __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define LOGF(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_FATAL,   __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define LOGT(message, ...)			platon::mpc::PlatonLogger::getInstance()->logFormat(platon::mpc::PlatonLogger::LOG_TRACE, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

#define LOGPrint(message, ...)		fprintf(platon::mpc::PlatonLogger::getInstance()->getStream(), message, ##__VA_ARGS__)


//#define LOGD(message, ...)			fprintf(stdout, message"\n", ##__VA_ARGS__)
//#define LOGI(message, ...)				fprintf(stdout, message"\n", ##__VA_ARGS__)
//#define LOGW(message, ...)			fprintf(stdout, message"\n", ##__VA_ARGS__)
//#define LOGE(message, ...)				fprintf(stdout, message"\n", ##__VA_ARGS__)
//#define LOGF(message, ...)				fprintf(stdout, message"\n", ##__VA_ARGS__)
//#define LOGT(message, ...)				fprintf(stdout, message"\n", ##__VA_ARGS__)

