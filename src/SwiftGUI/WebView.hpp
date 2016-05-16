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

#ifndef SWIFT_WEBVIEW_HPP
#define SWIFT_WEBVIEW_HPP

// includes  -------------------------------------------------------------------
#include "types.hpp"
#include "helpers.hpp"

namespace swift {

class WebView {

 public:

  WebView(const std::string& url, int width, int height);
  virtual ~WebView();

  void SetDrawCallback(const DrawCallback& callback);

  template<typename ...Args>
  void CallJavascript(std::string const& method, Args&& ... a) const {
    std::vector<std::string> args = {(to_string(a))...};
    call_javascript_impl(method, args);
  }

  void Resize(int width, int height)                 const;
  void Reload(bool ignoreCache = false)              const;

  void InjectMouseMove(int x, int y)                 const;
  void InjectMouseWheel(int direction, int x, int y) const;

  void InjectButtonDown(int button, int x, int y)    const;
  void InjectButtonUp(int button, int x, int y)      const;

  void InjectKeyDown(unsigned char key)              const;
  void InjectKeyUp(unsigned char key)                const;

 private:
  void call_javascript_impl(std::string const& method, std::vector<std::string> const& args) const;

  detail::Browser*        browser_;
  detail::WebViewClient*  client_;

};

}

#endif // SWIFT_WEBVIEW_HPP
