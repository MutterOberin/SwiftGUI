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

#ifndef SWIFT_LOGGER_HPP
#define SWIFT_LOGGER_HPP

#include <iosfwd>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Prints beautiful messages to the console output.                           //
////////////////////////////////////////////////////////////////////////////////

class Logger {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // If any of these is set to false, the corresponding messages are discarded.
  static bool enable_trace;
  static bool enable_debug;
  static bool enable_message;
  static bool enable_warning;
  static bool enable_error;

  static std::ostream& trace  (const char* file, int line);
  static std::ostream& debug  (const char* file, int line);
  static std::ostream& message(const char* file, int line);
  static std::ostream& warning(const char* file, int line);
  static std::ostream& error  (const char* file, int line);
};

// Use these macros in your code like this:
//   LOG_MESSAGE << "hello world" << std::endl;
#define SWIFT_TRACE   Logger::trace  (__FILE__, __LINE__)
#define SWIFT_DEBUG   Logger::debug  (__FILE__, __LINE__)
#define SWIFT_MESSAGE Logger::message(__FILE__, __LINE__)
#define SWIFT_WARNING Logger::warning(__FILE__, __LINE__)
#define SWIFT_ERROR   Logger::error  (__FILE__, __LINE__)

}

#endif  // SWIFT_LOGGER_HPP
