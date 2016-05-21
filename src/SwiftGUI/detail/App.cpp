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
#include "App.hpp"

#include "JSHandler.hpp"

#include <iostream>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

void App::OnBeforeCommandLineProcessing(const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {

  if (process_type.empty()) {
    command_line->AppendSwitch("enable-overlay-scrollbar");
    command_line->AppendSwitch("enable-begin-frame-scheduling");
    command_line->AppendSwitch("disable-gpu");
    command_line->AppendSwitch("disable-gpu-compositing");
  }
}

////////////////////////////////////////////////////////////////////////////////

void App::OnContextCreated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) {

  CefRefPtr<CefV8Value> object = context->GetGlobal();
  CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("call_native", new JSHandler(browser));
  object->SetValue("call_native", func, V8_PROPERTY_ATTRIBUTE_NONE);
}

////////////////////////////////////////////////////////////////////////////////

}
}

