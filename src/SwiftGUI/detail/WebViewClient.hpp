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
#include <iostream>
#include <fstream>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_v8.h>
#include <include/wrapper/cef_stream_resource_handler.h>

namespace swift {
namespace detail {

class WebViewClient : public CefClient,
                      public CefRenderHandler,
                      public CefRequestHandler,
                      public CefDisplayHandler {
 public:
  WebViewClient(int width, int height)
  : width_(width)
  , height_(height) {}

  void SetDrawCallback(const DrawCallback& callback) {
    callback_ = callback;
  }

  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override {
    return this;
  }

  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override {
    return this;
  }

  virtual bool OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) override {

    if (message->GetName() == "call_native") {

      std::vector<Any> args;

      for (int i(1); i<message->GetArgumentList()->GetSize(); ++i) {
        CefValueType type(message->GetArgumentList()->GetType(i));
        switch(type) {
          case VTYPE_DOUBLE:
            args.push_back(message->GetArgumentList()->GetDouble(i));
            break;
          case VTYPE_INT:
            args.push_back(message->GetArgumentList()->GetInt(i));
            break;
          case VTYPE_BOOL:
            args.push_back(message->GetArgumentList()->GetBool(i));
            break;
          case VTYPE_STRING:
            args.push_back(message->GetArgumentList()->GetString(i).ToString());
            break;
          default: break;
            // TODO
        }
      }

      std::string name(message->GetArgumentList()->GetString(0).ToString());
      js_callbacks_[name](args);

      return true;
    }

    return false;
  }

  virtual bool OnConsoleMessage(
      CefRefPtr<CefBrowser> browser,
      CefString const& message, CefString const& source, int line) override {

    std::cout << "[" << source.ToString() << ":" << line << "] " << message.ToString() << std::endl;
    return true;
  }

  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) override {

    std::string url(request->GetURL().ToString());

    if (url.find("file://") == 0) {
      std::string path(url.substr(7));
      std::string ext(url.substr(url.find_last_of('.')));

      std::ifstream input(path, std::ios::binary);

      if (!input) {
        std::cout << "Failed to open file " << path << std::endl;
        return nullptr;
      }

      std::vector<char> buffer(
              (std::istreambuf_iterator<char>(input)),
              (std::istreambuf_iterator<char>()));

      CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(
        static_cast<void*>(buffer.data()), buffer.size());

      std::string mime("text/html");
      if      (ext == ".png")   mime = "image/png";
      else if (ext == ".jpg")   mime = "image/jpg";
      else if (ext == ".jpeg")  mime = "image/jpg";
      else if (ext == ".js")    mime = "text/javascript";
      else if (ext == ".css")   mime = "text/css";
      else if (ext == ".woff")  mime = "application/x-font-woff";
      else if (ext == ".woff2") mime = "application/x-font-woff";
      else if (ext != ".html") {
        std::cout << "Warning: Openeing file with unknown extension " << ext << std::endl;
      }

      return new CefStreamResourceHandler(mime, stream);
    }

    return nullptr;
  }

  void Resize(int width, int height) {
    width_ = width;
    height_ = height;
  }

  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override {
    rect = CefRect(0, 0, width_, height_);
    return true;
  }

  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override {
    if (callback_) {
      std::vector<Rect> rects(dirtyRects.size());
      for (int i(0); i<dirtyRects.size(); ++i) {
        rects[i].x = dirtyRects[i].x;
        rects[i].y = dirtyRects[i].y;
        rects[i].width = dirtyRects[i].width;
        rects[i].height = dirtyRects[i].height;
      }
      callback_(width, height, rects, (char*)buffer);
    }
  }

  void register_js_callback(std::string const& name, std::function<void(std::vector<Any> const&)> callback) {
    js_callbacks_[name] = callback;
  }

 private:

  IMPLEMENT_REFCOUNTING(WebViewClient);

  int width_;
  int height_;

  DrawCallback callback_;
  std::unordered_map<std::string, std::function<void(std::vector<Any> const&)>> js_callbacks_;
};

}
}

#endif // SWIFT_DETAIL_WEBVIEWCLIENT_HPP
