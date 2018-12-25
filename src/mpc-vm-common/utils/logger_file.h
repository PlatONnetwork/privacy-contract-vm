#pragma once

#include <stdio.h>

#include "mpc_common.h"
NS_PLATON_MPC_BEG

FILE* initLoggerFile(const char *pPathDir);
int uninitLoggerFile();

int writeLoggerFile(const char *pLog);
void writeTerminal(int level, const char* pLog);

//log �ļ�������·��
int getLogWorkDir(char* szPath, int size);
//��ȡcycle����־�ļ�·�� ��matchsdk_0.log �� matchsdk_3.logѭ���л���
int getLogFilePath(const char *szDir, char* szFilePath);

NS_PLATON_MPC_END
