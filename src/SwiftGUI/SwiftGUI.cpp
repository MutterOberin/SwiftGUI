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
#include "SwiftGUI.hpp"

#include "detail/App.hpp"

#include <stdlib.h>
#include <include/cef_app.h>

#include <iostream>

namespace swift {

void Gui::Init(int argc, char *argv[]) {

  CefRefPtr<detail::App> app(new detail::App());

  CefMainArgs args(argc, argv);
  int result = CefExecuteProcess(args, app.get(), 0);
  if (result >= 0) {
    exit(result);
  }

  CefSettings settings;
  settings.remote_debugging_port = 8999;
  if (!CefInitialize(args, settings, app, 0)) {
    std::cout << "Warning!" << std::endl;;
  }
}

void Gui::CleanUp() {
  CefShutdown();
}

void Gui::Update() {
  CefDoMessageLoopWork();
}



}
