////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT_KEY_EVENT_HPP
#define SWIFT_KEY_EVENT_HPP

// includes  -------------------------------------------------------------------
#include "types.hpp"

namespace swift {

struct KeyEvent {

  enum class Type {
    PRESS, RELEASE, CHARACTER
  };

  //////////////////////////////////////////////////////////////////////////////
  // ---------------------------------------------------------- public interface
  KeyEvent()
  : type_(Type::PRESS)
  , modifiers_(0)
  , key_(Key::UNKNOWN)
  {}

  // PRESS, RELEASE, CHARACTER
  Type   type_;

  // bitwise or of any Modifier defined in types.hpp
  uint32 modifiers_;

  union {
    // only used for PRESS and RELEASE
    Key    key_;

    // only used for CHARACTER
    uint16 character_;
  };
};

}

#endif // SWIFT_KEY_EVENT_HPP
