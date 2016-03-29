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

#ifndef SWIFT_DETAIL_BROWSER_HPP
#define SWIFT_DETAIL_BROWSER_HPP

// includes  -------------------------------------------------------------------
#include <include/cef_client.h>

namespace swift {
namespace detail {

class Browser {
 public:
  CefRefPtr<CefBrowser>& Get() {
    return browser_;
  }

  CefRefPtr<CefBrowser> const& Get() const {
    return browser_;
  }

 private:
  CefRefPtr<CefBrowser> browser_;

};

}
}

#endif // SWIFT_DETAIL_BROWSER_HPP