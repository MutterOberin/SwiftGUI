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
#include <include/cef_render_process_handler.h>
#include <iostream>

#include "JSHandler.hpp"

namespace swift {
namespace detail {

class App : public CefApp,
            public CefRenderProcessHandler {

 public:

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
    return this;
  }

  // CefBrowserProcessHandler methods:
  virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override {

    if (process_type.empty()) {
      command_line->AppendSwitch("enable-overlay-scrollbar");
      command_line->AppendSwitch("enable-begin-frame-scheduling");
      command_line->AppendSwitch("disable-gpu");
      command_line->AppendSwitch("disable-gpu-compositing");
    }
  }

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) override {

    CefRefPtr<CefV8Value> object = context->GetGlobal();
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("call_native", new JSHandler(browser));
    object->SetValue("call_native", func, V8_PROPERTY_ATTRIBUTE_NONE);
  }

  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) override {

  }

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                 CefProcessId source_process,
                                 CefRefPtr<CefProcessMessage> message) override {
    return false;
  }

 private:
  IMPLEMENT_REFCOUNTING(App);
};

}
}

#endif // SWIFT_DETAIL_APP_HPP
