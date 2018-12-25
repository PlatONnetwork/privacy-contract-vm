#pragma once

#include <stdio.h>

#include "mpc_common.h"
NS_PLATON_MPC_BEG

FILE* initLoggerFile(const char *pPathDir);
int uninitLoggerFile();

int writeLoggerFile(const char *pLog);
void writeTerminal(int level, const char* pLog);

//log 文件夹完整路径
int getLogWorkDir(char* szPath, int size);
//获取cycle的日志文件路径 （matchsdk_0.log 到 matchsdk_3.log循环切换）
int getLogFilePath(const char *szDir, char* szFilePath);

NS_PLATON_MPC_END
