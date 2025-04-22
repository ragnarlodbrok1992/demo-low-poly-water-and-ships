#include "logger.hpp"

void register_logger_file(const std::string& file_name, std::ofstream& of_stream) {
  // Registers file where application was started from, not where binary is
  char buffer[512];
  GetModuleFileName(NULL, buffer, 512);
  std::string::size_type pos = std::string(buffer).find_last_of("\\/");
  std::string file_path = std::string(buffer).substr(0, pos + 1).c_str();
  std::string full_logger_path = file_path + file_name;

  of_stream.open(full_logger_path);

  if (of_stream.is_open()) {
    // We do not keep std::cout buffer, since we won't ever use it in console, we write to a file.
    // std::streambuf* cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(of_stream.rdbuf());

    std::cout << "Logger file registered!" << std::endl;

    // We do not close file, since file will be open through all of lifetime of application
    // logger_file_stream.close();
  }
}

void log(const char* msg) {
  if (LOGGER_STREAM.is_open()) {
    std::cout.rdbuf(LOGGER_STREAM.rdbuf());

    std::cout << msg << std::endl;
  }
};
