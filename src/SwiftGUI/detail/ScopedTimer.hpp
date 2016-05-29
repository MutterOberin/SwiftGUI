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

#ifndef SWIFT_SCOPED_TIMER_HPP
#define SWIFT_SCOPED_TIMER_HPP

// includes  -------------------------------------------------------------------
#include <string>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// Can be used to measure time taken by some part of code.                    //
////////////////////////////////////////////////////////////////////////////////

class ScopedTimer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  ScopedTimer(std::string const& name);
  virtual ~ScopedTimer();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  double GetNow();

  std::string name_;
  double      start_time_;
};

}
}

#endif  // SWIFT_SCOPED_TIMER_HPP
