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

#ifndef SWIFT_TYPES_HPP
#define SWIFT_TYPES_HPP

// includes  -------------------------------------------------------------------
#include <vector>
#include <functional>

namespace swift {

typedef std::function<void(int, int, int, int, bool, const char*)> DrawCallback;

enum class Button: int {
  LEFT          = 0,
  MIDDLE        = 1,
  RIGHT         = 2,
  BUTTON_4      = 3,
  BUTTON_5      = 4,
  BUTTON_6      = 5,
  BUTTON_7      = 6,
  BUTTON_8      = 7
};

enum class Modifier: int {
  NONE          = 0,
  CAPS_LOCK     = 1 << 0,
  SHIFT         = 1 << 1,
  CONTROL       = 1 << 2,
  ALT           = 1 << 3,
  LEFT_BUTTON   = 1 << 4,
  MIDDLE_BUTTON = 1 << 5,
  RIGHT_BUTTON  = 1 << 6,
  COMMAND       = 1 << 7,
  NUM_LOCK      = 1 << 8,
  IS_KEY_PAD    = 1 << 9,
  IS_LEFT       = 1 << 10,
  IS_RIGHT      = 1 << 11
};

namespace detail {

class Browser;
class WebViewClient;

}
}

#endif // SWIFT_TYPES_HPP
