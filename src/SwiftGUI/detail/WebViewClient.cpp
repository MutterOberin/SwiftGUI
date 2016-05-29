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
#include "WebViewClient.hpp"

#include "../Logger.hpp"
#include "ScopedTimer.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

WebViewClient::WebViewClient(int width, int height)
: width_(width)
, height_(height)
, last_draw_width_(0)
, last_draw_height_(0) {}

////////////////////////////////////////////////////////////////////////////////

void WebViewClient::SetDrawCallback(DrawSettings const& settings, DrawCallback const& callback) {
  settings_ = settings;
  callback_ = callback;
}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefLifeSpanHandler> WebViewClient::GetLifeSpanHandler() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefDisplayHandler> WebViewClient::GetDisplayHandler() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefRenderHandler> WebViewClient::GetRenderHandler() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefRequestHandler> WebViewClient::GetRequestHandler() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////

bool WebViewClient::OnProcessMessageReceived(
  CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {

  if (message->GetName() == "call_native") {

    std::vector<Any> args;

    for (int i(1); i<message->GetArgumentList()->GetSize(); ++i) {
      CefValueType type(message->GetArgumentList()->GetType(i));
      switch(type) {
        case VTYPE_DOUBLE:
          args.push_back(message->GetArgumentList()->GetDouble(i));
          break;
        case VTYPE_BOOL:
          args.push_back(message->GetArgumentList()->GetBool(i));
          break;
        case VTYPE_STRING:
          args.push_back(message->GetArgumentList()->GetString(i).ToString());
          break;
        default: break;
      }
    }

    std::string name(message->GetArgumentList()->GetString(0).ToString());
    auto callback(js_callbacks_.find(name));

    if (callback == js_callbacks_.end()) {
      SWIFT_WARNING << "Cannot call function '" << name
                    << "': No callback is registered for this function name!"
                    << std::endl;
      return true;
    }

    callback->second(args);

    return true;

  } else if (message->GetName() == "error") {
    SWIFT_WARNING << message->GetArgumentList()->GetString(0).ToString()
                  << std::endl;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////

bool WebViewClient::OnConsoleMessage(
    CefRefPtr<CefBrowser> browser,
    CefString const& message, CefString const& source, int line) {

  std::string path(source.ToString());
  int pos(path.find_last_of("/\\"));
  Logger::message(path.substr(pos == std::string::npos ? 0 : pos + 1).c_str(), line)
                  << message.ToString() << std::endl;
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void WebViewClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  browser_ = browser;
}

////////////////////////////////////////////////////////////////////////////////

CefRefPtr<CefResourceHandler> WebViewClient::GetResourceHandler(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefRequest> request) {

  std::string url(request->GetURL().ToString());

  if (url.find("file://") == 0) {
    std::string path(url.substr(7));
    std::string ext(url.substr(url.find_last_of('.')));

    std::ifstream input(path, std::ios::binary);

    if (!input) {
      SWIFT_WARNING << "Failed to open file '" << path << "'!" << std::endl;
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
      SWIFT_WARNING << "Opening file with unknown extension '" << ext << "'!" << std::endl;
    }

    return new CefStreamResourceHandler(mime, stream);
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void WebViewClient::Resize(int width, int height) {
  width_ = width;
  height_ = height;
  browser_->GetHost()->WasResized();
}

////////////////////////////////////////////////////////////////////////////////

bool WebViewClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
  rect = CefRect(0, 0, width_, height_);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void WebViewClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                            RectList const& dirtyRects, const void *b,
                            int width, int height) {

  if (callback_) {

    detail::ScopedTimer timer("DrawEvent");
    DrawEvent event;

    event.resized_ = width != last_draw_width_ || height != last_draw_height_;
    last_draw_width_ = width;
    last_draw_height_ = height;

    if (event.resized_ || settings_.full_redraws_) {
      event.x_       = 0;
      event.y_       = 0;
      event.width_   = width;
      event.height_  = height;
      event.data_    = (char*)b;

      if(settings_.flip_y_ || settings_.convert_to_rgba_) {
        std::vector<char> data(width*height*4);
        char* buffer = (char*)b;

        if(settings_.convert_to_rgba_ && settings_.flip_y_) {

          for(int y(0);y<height;++y) {
            for(int x(0);x<width;++x) {
              data[(height-y-1)*width*4+x*4+0] = buffer[y*width*4+x*4+2];
              data[(height-y-1)*width*4+x*4+1] = buffer[y*width*4+x*4+1];
              data[(height-y-1)*width*4+x*4+2] = buffer[y*width*4+x*4+0];
              data[(height-y-1)*width*4+x*4+3] = buffer[y*width*4+x*4+3];
            }
          }

        } else if (settings_.convert_to_rgba_) {

          for(int y(0);y<height;++y) {
            for(int x(0);x<width;++x) {
              data[y*width*4+x*4+0] = buffer[y*width*4+x*4+2];
              data[y*width*4+x*4+1] = buffer[y*width*4+x*4+1];
              data[y*width*4+x*4+2] = buffer[y*width*4+x*4+0];
              data[y*width*4+x*4+3] = buffer[y*width*4+x*4+3];
            }
          }

        } else {
          for(int y(0);y<height;++y) {
            std::memcpy(&data[(height-y-1)*width*4], buffer + y*width*4, width*4);
          }
        }

        event.data_ = data.data();
        callback_(event);
      } else {
        callback_(event);
      }

    } else {
      for(auto const& rect: dirtyRects) {
        event.x_       = rect.x;
        event.y_       = rect.y;
        event.width_   = rect.width;
        event.height_  = rect.height;
        std::vector<char> data(rect.width*rect.height*4);
        char* buffer = (char*)b;

        if(settings_.convert_to_rgba_ && settings_.flip_y_) {

          // for(int y(0);y<height;++y) {
          //   for(int x(0);x<width;++x) {
          //     data[(height-y-1)*width*4+x*4+0] = buffer[y*width*4+x*4+2];
          //     data[(height-y-1)*width*4+x*4+1] = buffer[y*width*4+x*4+1];
          //     data[(height-y-1)*width*4+x*4+2] = buffer[y*width*4+x*4+0];
          //     data[(height-y-1)*width*4+x*4+3] = buffer[y*width*4+x*4+3];
          //   }
          // }

        } else if (settings_.convert_to_rgba_) {

          // for(int y(0);y<height;++y) {
          //   for(int x(0);x<width;++x) {
          //     data[y*width*4+x*4+0] = buffer[y*width*4+x*4+2];
          //     data[y*width*4+x*4+1] = buffer[y*width*4+x*4+1];
          //     data[y*width*4+x*4+2] = buffer[y*width*4+x*4+0];
          //     data[y*width*4+x*4+3] = buffer[y*width*4+x*4+3];
          //   }
          // }

        } else {
          for(int y(0);y<rect.height;++y) {
            std::memcpy(&data[y*rect.width*4], buffer + ((y+rect.y)*width + rect.x)*4, rect.width*4);
          }
        }

        event.data_ = data.data();
        callback_(event);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void WebViewClient::RegisterJSCallback(std::string const& name, std::function<void(std::vector<Any> const&)> callback) {
  js_callbacks_[name] = callback;
}

////////////////////////////////////////////////////////////////////////////////

}
}

