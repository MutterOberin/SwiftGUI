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

#ifndef SWIFT_DETAIL_APP_HPP
#define SWIFT_DETAIL_APP_HPP

// includes  -------------------------------------------------------------------
#include <include/cef_app.h>
#include <iostream>

namespace swift {
namespace detail {

class App : public CefApp {
 public:

  // CefBrowserProcessHandler methods:
  virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {

    if (process_type.empty()) {
      command_line->AppendSwitch("enable-overlay-scrollbar");
      command_line->AppendSwitch("enable-begin-frame-scheduling");
      command_line->AppendSwitch("disable-gpu");
      command_line->AppendSwitch("disable-gpu-compositing");
    }
  }

 private:
  IMPLEMENT_REFCOUNTING(App);
};

}
}

#endif // SWIFT_DETAIL_APP_HPP
