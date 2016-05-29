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

// includes  -------------------------------------------------------------------
#include "ScopedTimer.hpp"

#include "../Logger.hpp"
#include <chrono>
#include <iostream>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

ScopedTimer::ScopedTimer(std::string const& name)
: name_(name)
, start_time_(GetNow()) {}

////////////////////////////////////////////////////////////////////////////////

ScopedTimer::~ScopedTimer() {
    double now = GetNow();
    SWIFT_DEBUG << name_ << ": " << now - start_time_ << " ms " << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

double ScopedTimer::GetNow() {
  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count() * 0.001;
}

////////////////////////////////////////////////////////////////////////////////

}
}
