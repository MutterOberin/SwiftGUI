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

#ifndef SWIFT_DETAIL_WEBVIEWCLIENT_HPP
#define SWIFT_DETAIL_WEBVIEWCLIENT_HPP

// includes  -------------------------------------------------------------------
#include "../types.hpp"
#include "../Any.hpp"

#include <unordered_map>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_v8.h>
#include <include/wrapper/cef_stream_resource_handler.h>

namespace swift {
namespace detail {

class WebViewClient : public CefClient,
                      public CefLifeSpanHandler,
                      public CefRenderHandler,
                      public CefRequestHandler,
                      public CefDisplayHandler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Set the initial size of the internal render target.
  WebViewClient(int width, int height);

  void SetDrawCallback(DrawCallback const& callback);

  void Resize(int width, int height);

  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

  void RegisterJSCallback(std::string const& name,
    std::function<void(std::vector<Any> const&)> callback);

  CefRefPtr<CefBrowser> const& GetBrowser() const {
    return browser_;
  }

  // CefClient interface -------------------------------------------------------

  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
  virtual CefRefPtr<CefDisplayHandler>  GetDisplayHandler() override;
  virtual CefRefPtr<CefRenderHandler>   GetRenderHandler() override;
  virtual CefRefPtr<CefRequestHandler>  GetRequestHandler() override;


  virtual bool OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) override;

  // CefLifeSpanHandler interface ----------------------------------------------

  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

  // CefDisplayHandler interface -----------------------------------------------

  virtual bool OnConsoleMessage(
      CefRefPtr<CefBrowser> browser,
      CefString const& message, CefString const& source, int line) override;

  // CefRenderHandler interface ------------------------------------------------

  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
    const RectList &dirtyRects, const void *buffer,
    int width, int height) override;

  // CefRequestHandler interface -----------------------------------------------

  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) override;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  IMPLEMENT_REFCOUNTING(WebViewClient);

  CefRefPtr<CefBrowser> browser_;

  int width_;
  int height_;

  int last_draw_width_;
  int last_draw_height_;

  DrawCallback callback_;

  std::unordered_map<std::string, std::function<void(std::vector<Any> const&)>> js_callbacks_;
};

}
}

#endif // SWIFT_DETAIL_WEBVIEWCLIENT_HPP
