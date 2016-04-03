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

namespace swift {

class WebView {

 public:

  WebView(const std::string& url, int width, int height);
  virtual ~WebView();

  void SetDrawCallback(const DrawCallback& callback);

  void Resize(int width, int height);
  void Reload(bool ignoreCache = false);

  void ShowDevTools();

 private:
  detail::Browser*        browser_;
  detail::WebViewClient*  client_;

};

}

#endif // SWIFT_WEBVIEW_HPP
