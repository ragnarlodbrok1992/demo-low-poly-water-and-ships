#ifndef _H_LOGGER
#define _H_LOGGER

#include <iostream>
#include <fstream>
#include <string>

#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern const char* LOGGER_FILE_NAME;
extern std::ofstream LOGGER_STREAM;

void register_logger_file(const std::string& file_name, std::ofstream& of_stream);
void log(void* ptr);
void log(const char* msg);

#endif /* _H_LOGGER */

