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

#ifndef SWIFT_MOUSE_EVENT_HPP
#define SWIFT_MOUSE_EVENT_HPP

// includes  -------------------------------------------------------------------
#include "types.hpp"

namespace swift {

struct MouseEvent {

  enum class Type {
    MOVE, SCROLL, PRESS, RELEASE
  };

  //////////////////////////////////////////////////////////////////////////////
  // ---------------------------------------------------------- public interface
  MouseEvent()
  : type_(Type::MOVE)
  , x_(0)
  , y_(0)
  {}

  // either MOVE, SCROLL, PRESS or RELEASE
  Type type_;

  union {
    // x-position for MOVE, x-direction for SCROLL
    int    x_;

    // only used for PRESS and RELEASE
    Button button_;
  };

  // y-position for MOVE, y-direction for SCROLL
  int y_;
};

}

#endif // SWIFT_MOUSE_EVENT_HPP
