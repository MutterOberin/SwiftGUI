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
#include "WebView.hpp"

#include "detail/WebViewClient.hpp"
#include "detail/Browser.hpp"

namespace swift {

WebView::WebView(const std::string& url, int width, int height) {
  client_ = new detail::WebViewClient(width, height);
  browser_ = new detail::Browser();

  CefRefPtr<detail::WebViewClient> browserClient;
  CefWindowInfo info;
  CefBrowserSettings browserSettings;

  std::size_t windowHandle = 0;
  info.SetAsWindowless(windowHandle, true);

  browser_->Get() = CefBrowserHost::CreateBrowserSync(info, client_,
            url,
            browserSettings, NULL);
}

WebView::~WebView() {
  delete browser_;
  delete client_;
}

void WebView::SetDrawCallback(const DrawCallback& callback) {
  client_->SetDrawCallback(callback);
}

void WebView::Resize(int width, int height) {
  client_->Resize(width, height);
  browser_->Get()->GetHost()->WasResized();
}

void WebView::Reload(bool ignoreCache) {
  if (ignoreCache) browser_->Get()->ReloadIgnoreCache();
  else             browser_->Get()->Reload();
}

class FooClient:  public CefClient {
  private:
    IMPLEMENT_REFCOUNTING(FooClient);
};

void WebView::ShowDevTools() {
  CefWindowInfo info;
  CefBrowserSettings settings;
  browser_->Get()->GetHost()->ShowDevTools(info, new FooClient(), settings, CefPoint(0,0));
}

}
