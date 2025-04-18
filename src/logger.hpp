#ifndef _H_LOGGER
#define _H_LOGGER

#include <iostream>
#include <fstream>
#include <string>

#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>

void register_logger_file(const std::string& file_name);

#endif /* _H_LOGGER */

