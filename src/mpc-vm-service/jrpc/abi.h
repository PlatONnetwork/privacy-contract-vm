#pragma once

#include <stdint.h>
#include <inttypes.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <iostream>
#include <string>

using std::string;

void set_result(const char *taskId, uint64_t status, const char *data);

const char* get_ir_data();
const char* get_participants();
const char* get_urls();
const char* get_url_by_id(const char *address);
const char* get_invitor();

uint64_t get_status(const char* taskid);
uint64_t get_fee(const char* method);
