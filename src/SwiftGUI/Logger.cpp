////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of SwiftGUI.                                             //
//                                                                            //
// Copyright: (c) 2015-2016 Simon Schneegans                                  //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include "Logger.hpp"

#include <sstream>
#include <iostream>

// no colors on windows!
#if defined( _WIN32 )
#define PRINT_RED ""
#define PRINT_GREEN ""
#define PRINT_YELLOW ""
#define PRINT_BLUE ""
#define PRINT_PURPLE ""
#define PRINT_TURQUOISE ""

#define PRINT_RED_BOLD ""
#define PRINT_GREEN_BOLD ""
#define PRINT_YELLOW_BOLD ""
#define PRINT_BLUE_BOLD ""
#define PRINT_PURPLE_BOLD ""
#define PRINT_TURQUOISE_BOLD ""

#define PRINT_RESET ""
#else // _WIN32
#define PRINT_RED              "\x001b[0;31m"
#define PRINT_GREEN            "\x001b[0;32m"
#define PRINT_YELLOW           "\x001b[0;33m"
#define PRINT_BLUE             "\x001b[0;34m"
#define PRINT_PURPLE           "\x001b[0;35m"
#define PRINT_TURQUOISE        "\x001b[0;36m"

#define PRINT_RED_BOLD         "\x001b[1;31m"
#define PRINT_GREEN_BOLD       "\x001b[1;32m"
#define PRINT_YELLOW_BOLD      "\x001b[1;33m"
#define PRINT_BLUE_BOLD        "\x001b[1;34m"
#define PRINT_PURPLE_BOLD      "\x001b[1;35m"
#define PRINT_TURQUOISE_BOLD   "\x001b[1;36m"

#define PRINT_RESET            "\x001b[0m"
#endif // else _WIN32

namespace swift {

bool Logger::enable_trace   = false;
bool Logger::enable_debug   = true;
bool Logger::enable_message = true;
bool Logger::enable_warning = true;
bool Logger::enable_error   = true;

namespace {

  class NullBuffer : public std::streambuf {
   public:
    int overflow(int c) { return c; }
  };

  NullBuffer null_buffer;
  std::ostream dev_null(&null_buffer);

  std::string location_string(const char* f, int l) {
    std::stringstream sstr;
    std::string path(f);
    int pos(path.find_last_of("/\\"));
    sstr << "[" << path.substr(pos == std::string::npos ? 0 : pos + 1)
         << ":" << l << "]";
    return sstr.str();
  }

  std::ostream& print(bool enable, std::string const& header,
                      std::string const& color,
                      const char* file, int line) {
    if (enable) {
      return std::cout << color << header
                       << location_string(file, line)
                       << PRINT_RESET << " ";
    } else {
      return dev_null;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::trace(const char* file, int line) {
  return print(enable_trace, "[GUI][T]", PRINT_TURQUOISE, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::debug(const char* file, int line) {
  return print(enable_debug, "[GUI][D]", PRINT_BLUE, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::message(const char* file, int line) {
  return print(enable_message, "[GUI][M]", PRINT_GREEN, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::warning(const char* file, int line) {
  return print(enable_warning, "[GUI][W]", PRINT_YELLOW, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::error(const char* file, int line) {
  return print(enable_error, "[GUI][E]", PRINT_RED, file, line);
}

////////////////////////////////////////////////////////////////////////////////

}
