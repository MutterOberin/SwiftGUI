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
#include "WebApp.hpp"

#include "JSHandler.hpp"

#include <iostream>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

WebApp::WebApp(bool hardware_accelerated)
: hardware_accelerated_(hardware_accelerated) {}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefRenderProcessHandler> WebApp::GetRenderProcessHandler() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////

void WebApp::OnBeforeCommandLineProcessing(const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {

  if (process_type.empty()) {
    command_line->AppendSwitch("enable-overlay-scrollbar");
    command_line->AppendSwitch("enable-begin-frame-scheduling");

    if (!hardware_accelerated_) {
      command_line->AppendSwitch("disable-gpu");
      command_line->AppendSwitch("disable-gpu-compositing");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void WebApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) {

  CefRefPtr<CefV8Value> object = context->GetGlobal();
  CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("call_native", new JSHandler(browser));
  object->SetValue("call_native", func, V8_PROPERTY_ATTRIBUTE_NONE);
}

////////////////////////////////////////////////////////////////////////////////

void WebApp::OnContextInitialized() {
  std::cout << "OnContextInitialized" << std::endl;

}

////////////////////////////////////////////////////////////////////////////////

}
}

