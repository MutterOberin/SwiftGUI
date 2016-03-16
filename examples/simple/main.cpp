////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift3D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <iostream>

#include <SwiftGUI/SwiftGUI.hpp>

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

// -----------------------------------------------------------------------------
class RenderHandler : public CefRenderHandler
{
public:
  RenderHandler(int width=960, int height=640)
  : width_(width)
  , height_(height)
  {}

  // CefRenderHandler interface
  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
    rect = CefRect(0, 0, width_, height_);
    return true;
  }

  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
    std::cout << "draw" << std::endl;
    swift::WriteTga("test.tga", (char*)buffer, width_, height_);
  }

  // CefBase interface
 public:
  IMPLEMENT_REFCOUNTING(RenderHandler);

  int width_;
  int height_;
};


// -----------------------------------------------------------------------------
// for manual render handler
class BrowserClient : public CefClient
{
 public:
  BrowserClient(RenderHandler *renderHandler)
  : m_renderHandler(renderHandler)
  {}

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
    return m_renderHandler;
  }

  CefRefPtr<CefRenderHandler> m_renderHandler;

  IMPLEMENT_REFCOUNTING(BrowserClient);
};


// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  CefMainArgs args(argc, argv);

  int result = CefExecuteProcess(args, 0, 0);
  if (result >= 0) {
    return result;
  }

  CefSettings settings;

  if (!CefInitialize(args, settings, 0, 0)) {
    return -1;
  }

  RenderHandler* renderHandler = new RenderHandler();

  // create browser-window
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefWindowInfo window_info;
  CefBrowserSettings browserSettings;

  std::size_t windowHandle = 0;
  window_info.SetAsWindowless(windowHandle, true);

  browserClient = new BrowserClient(renderHandler);

  browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(),
              "https://www.youtube.com/watch?v=ghV21DlDOug",
              browserSettings, NULL);

  while (true) {
    // browser->GetHost()->SendKeyEvent(...);
    // browser->GetHost()->SendMouseMoveEvent(...);
    // browser->GetHost()->SendMouseClickEvent(...);
    // browser->GetHost()->SendMouseWheelEvent(...);
    CefDoMessageLoopWork();
  }

  CefShutdown();
  delete renderHandler;

  return 0;
}
