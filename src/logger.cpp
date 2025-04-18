#include "logger.hpp"

void register_logger_file(const std::string& file_name) {
  // Registers file where application was started from, not where binary is
  char buffer[512];
  GetModuleFileName(NULL, buffer, 512);
  std::string::size_type pos = std::string(buffer).find_last_of("\\/");
  std::string file_path = std::string(buffer).substr(0, pos + 1).c_str();
  std::string full_logger_path = file_path + file_name;

  std::ofstream logger_file_stream(full_logger_path);

  if (logger_file_stream.is_open()) {
    // We do not keep std::cout buffer, since we won't ever use it in console, we write to a file.
    // std::streambuf* cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(logger_file_stream.rdbuf());

    std::cout << "Logger file registered!" << std::endl;

    // We do not close file, since file will be open through all of lifetime of application
    // logger_file_stream.close();
  }
}

