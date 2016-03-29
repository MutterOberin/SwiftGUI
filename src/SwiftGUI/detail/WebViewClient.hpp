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

#include <iostream>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

namespace swift {
namespace detail {

class WebViewClient : public CefClient, public CefRenderHandler {
 public:
  WebViewClient(int width=960, int height=640)
  : width_(width)
  , height_(height) {}

  void SetDrawCallback(const DrawCallback& callback) {
    callback_ = callback;
  }

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
    return this;
  }

  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
    rect = CefRect(0, 0, width_, height_);
    return true;
  }

  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
    if (callback_) {
      std::vector<Rect> rects(dirtyRects.size());
      for (int i(0); i<dirtyRects.size(); ++i) {
        rects[i].x = dirtyRects[i].x;
        rects[i].y = dirtyRects[i].y;
        rects[i].width = dirtyRects[i].width;
        rects[i].height = dirtyRects[i].height;
      }
      callback_(rects, (char*)buffer);
    }
  }

 private:

  IMPLEMENT_REFCOUNTING(WebViewClient);

  int width_;
  int height_;

  DrawCallback callback_;
};

}
}

#endif // SWIFT_DETAIL_WEBVIEWCLIENT_HPP