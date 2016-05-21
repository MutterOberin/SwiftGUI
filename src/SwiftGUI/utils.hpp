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

#ifndef SWIFT_UTILS_HPP
#define SWIFT_UTILS_HPP

// includes  -------------------------------------------------------------------
#include <sstream>

namespace swift {
namespace utils {

template <typename T>
std::string to_string(T const& v) {
  std::ostringstream oss;
  oss << v;
  return oss.str();
}

}
}

#endif // SWIFT_UTILS_HPP
