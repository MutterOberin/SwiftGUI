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

// math types ------------------------------------------------------------------

// same typedefs as in steam api
typedef char          int8;
typedef unsigned char uint8;

#if defined( _WIN32 )
  typedef          __int16 int16;
  typedef unsigned __int16 uint16;
  typedef          __int32 int32;
  typedef unsigned __int32 uint32;
  typedef          __int64 int64;
  typedef unsigned __int64 uint64;
  typedef          int64   lint64;
  typedef          uint64  ulint64;
#else // _WIN32
  typedef short int16;
  typedef unsigned short uint16;
  typedef int int32;
  typedef unsigned int uint32;
  typedef long long int64;
  typedef unsigned long long uint64;

  // [u]int64 are actually defined as 'long long' and gcc 64-bit
  // doesn't automatically consider them the same as 'long int'.
  // Changing the types for [u]int64 is complicated by
  // there being many definitions, so we just
  // define a 'long int' here and use it in places that would
  // otherwise confuse the compiler.
  typedef long int lint64;
  typedef unsigned long int ulint64;
#endif // else _WIN32

// function types --------------------------------------------------------------

typedef std::function<void(int, int, int, int, bool, const char*)> DrawCallback;

// input enum types ------------------------------------------------------------

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

// codes based on the chromium key at
// https://github.com/adobe/webkit/blob/master/Source/WebCore/platform/WindowsKeyboardCodes.h
enum class Key: uint16 {
  UNKNOWN                   = 0,

  BACKSPACE                 = 0x08,
  TAB                       = 0x09,
  CLEAR                     = 0x0C,
  RETURN                    = 0x0D,
  SHIFT                     = 0x10,
  CONTROL                   = 0x11,
  LEFT_ALT                  = 0x12,
  PAUSE                     = 0x13,
  CAPS_LOCK                 = 0x14,
  KANA                      = 0x15,
  JUNJA                     = 0x17,
  FINAL                     = 0x18,
  HANJA                     = 0x19,
  ESCAPE                    = 0x1B,
  CONVERT                   = 0x1C,
  NONCONVERT                = 0x1D,
  ACCEPT                    = 0x1E,
  MODECHANGE                = 0x1F,
  SPACE                     = 0x20,
  PAGE_UP                   = 0x21,
  PAGE_DOWN                 = 0x22,
  END                       = 0x23,
  HOME                      = 0x24,
  LEFT                      = 0x25,
  UP                        = 0x26,
  RIGHT                     = 0x27,
  DOWN                      = 0x28,
  SELECT                    = 0x29,
  PRINT                     = 0x2A,
  EXECUTE                   = 0x2B,
  PRINT_SCREEN              = 0x2C,
  INSERT                    = 0x2D,
  DELETE                    = 0x2E,
  HELP                      = 0x2F,

  KEY_0                     = 0x30,
  KEY_1                     = 0x31,
  KEY_2                     = 0x32,
  KEY_3                     = 0x33,
  KEY_4                     = 0x34,
  KEY_5                     = 0x35,
  KEY_6                     = 0x36,
  KEY_7                     = 0x37,
  KEY_8                     = 0x38,
  KEY_9                     = 0x39,

  KEY_A                     = 0x41,
  KEY_B                     = 0x42,
  KEY_C                     = 0x43,
  KEY_D                     = 0x44,
  KEY_E                     = 0x45,
  KEY_F                     = 0x46,
  KEY_G                     = 0x47,
  KEY_H                     = 0x48,
  KEY_I                     = 0x49,
  KEY_J                     = 0x4A,
  KEY_K                     = 0x4B,
  KEY_L                     = 0x4C,
  KEY_M                     = 0x4D,
  KEY_N                     = 0x4E,
  KEY_O                     = 0x4F,
  KEY_P                     = 0x50,
  KEY_Q                     = 0x51,
  KEY_R                     = 0x52,
  KEY_S                     = 0x53,
  KEY_T                     = 0x54,
  KEY_U                     = 0x55,
  KEY_V                     = 0x56,
  KEY_W                     = 0x57,
  KEY_X                     = 0x58,
  KEY_Y                     = 0x59,
  KEY_Z                     = 0x5A,

  LEFT_SUPER                = 0x5B,
  RIGHT_SUPER               = 0x5C,
  APPS                      = 0x5D,
  SLEEP                     = 0x5F,

  KP_0                      = 0x60,
  KP_1                      = 0x61,
  KP_2                      = 0x62,
  KP_3                      = 0x63,
  KP_4                      = 0x64,
  KP_5                      = 0x65,
  KP_6                      = 0x66,
  KP_7                      = 0x67,
  KP_8                      = 0x68,
  KP_9                      = 0x69,
  KP_MULTIPLY               = 0x6A,
  KP_ADD                    = 0x6B,
  KP_SEPARATOR              = 0x6C,
  KP_SUBTRACT               = 0x6D,
  KP_DECIMAL                = 0x6E,
  KP_DIVIDE                 = 0x6F,

  F1                        = 0x70,
  F2                        = 0x71,
  F3                        = 0x72,
  F4                        = 0x73,
  F5                        = 0x74,
  F6                        = 0x75,
  F7                        = 0x76,
  F8                        = 0x77,
  F9                        = 0x78,
  F10                       = 0x79,
  F11                       = 0x7A,
  F12                       = 0x7B,
  F13                       = 0x7C,
  F14                       = 0x7D,
  F15                       = 0x7E,
  F16                       = 0x7F,
  F17                       = 0x80,
  F18                       = 0x81,
  F19                       = 0x82,
  F20                       = 0x83,
  F21                       = 0x84,
  F22                       = 0x85,
  F23                       = 0x86,
  F24                       = 0x87,

  NUM_LOCK                  = 0x90,
  SCROLL_LOCK               = 0x91,
  LEFT_SHIFT                = 0xA0,
  RIGHT_SHIFT               = 0xA1,
  LEFT_CONTROL              = 0xA2,
  RIGHT_CONTROL             = 0xA3,
  LEFT_MENU                 = 0xA4,
  RIGHT_MENU                = 0xA5,

  BROWSER_BACK              = 0xA6,
  BROWSER_FORWARD           = 0xA7,
  BROWSER_REFRESH           = 0xA8,
  BROWSER_STOP              = 0xA9,
  BROWSER_SEARCH            = 0xAA,
  BROWSER_FAVORITES         = 0xAB,
  BROWSER_HOME              = 0xAC,
  VOLUME_MUTE               = 0xAD,
  VOLUME_DOWN               = 0xAE,
  VOLUME_UP                 = 0xAF,
  MEDIA_NEXT_TRACK          = 0xB0,
  MEDIA_PREV_TRACK          = 0xB1,
  MEDIA_STOP                = 0xB2,
  MEDIA_PLAY_PAUSE          = 0xB3,
  MEDIA_LAUNCH_MAIL         = 0xB4,
  MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
  MEDIA_LAUNCH_APP1         = 0xB6,
  MEDIA_LAUNCH_APP2         = 0xB7,

  PLUS                      = 0xBB,
  COMMA                     = 0xBC,
  MINUS                     = 0xBD,
  PERIOD                    = 0xBE,

  OEM_1                     = 0xBA, // The ';:' key
  OEM_2                     = 0xBF, // The '/?' key
  OEM_3                     = 0xC0, // The '`~' key
  OEM_4                     = 0xDB, // The '[{' key
  OEM_5                     = 0xDC, // The '\|' key
  OEM_6                     = 0xDD, // The ']}' key
  OEM_7                     = 0xDE, // The 'single-quote/double-quote' key
  OEM_8                     = 0xDF, // Used for miscellaneous characters; it can vary by keyboard.
  OEM_102                   = 0xE2, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
};

std::ostream& operator<<(std::ostream& os, Key key);

namespace detail {

class Browser;
class WebViewClient;

}
}

#endif // SWIFT_TYPES_HPP
