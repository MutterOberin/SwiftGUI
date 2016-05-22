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

#ifndef SWIFT_DETAIL_WEB_APP_HPP
#define SWIFT_DETAIL_WEB_APP_HPP

// includes  -------------------------------------------------------------------
#include <include/cef_app.h>
#include <include/cef_render_process_handler.h>

namespace swift {
namespace detail {

class WebApp : public CefApp,
               public CefRenderProcessHandler,
               public CefBrowserProcessHandler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // If hardware_accelerated is set to true, webgl will be available, but the
  // overall performance is likey to be worse.
  WebApp(bool hardware_accelerated);

  // CefApp interface ----------------------------------------------------------

  // Returns this.
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

  // This is called just before the command line arguments are parsed by CEF. We
  // use this opportunity to add some configuration parameters.
  virtual void OnBeforeCommandLineProcessing(CefString const& process_type,
                               CefRefPtr<CefCommandLine> command_line) override;

  // CefRenderProcessHandler interface -----------------------------------------

  // This is called for each new context. We use this callback to add the
  // call_native method to the window object.
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) override;

  // CefBrowserProcessHandler interface -----------------------------------------

  virtual void OnContextInitialized() override;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  IMPLEMENT_REFCOUNTING(WebApp);

  bool hardware_accelerated_;
};

}
}

#endif // SWIFT_DETAIL_WEB_APP_HPP
