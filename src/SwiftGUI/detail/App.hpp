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

namespace swift {
namespace detail {

class App : public CefApp,
            public CefRenderProcessHandler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
    return this;
  }

  virtual void OnBeforeCommandLineProcessing(
                                const CefString& process_type,
                                CefRefPtr<CefCommandLine> command_line) override;

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) override ;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  IMPLEMENT_REFCOUNTING(App);
};

}
}

#endif // SWIFT_DETAIL_APP_HPP
