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

////////////////////////////////////////////////////////////////////////////////

WebView::WebView(const std::string& url, int width, int height)
: browser_(new detail::Browser())
, client_(new detail::WebViewClient(width, height))
, devToolsOpen_(false) {

  CefRefPtr<detail::WebViewClient> browserClient;
  CefWindowInfo info;
  CefBrowserSettings browserSettings;

  std::size_t windowHandle = 0;
  info.SetAsWindowless(windowHandle, true);
  browserSettings.windowless_frame_rate = 60;

  browser_->Get() = CefBrowserHost::CreateBrowserSync(info, client_,
            url,
            browserSettings, NULL);
}

////////////////////////////////////////////////////////////////////////////////

WebView::~WebView() {
  delete browser_;
  delete client_;
}

////////////////////////////////////////////////////////////////////////////////

void WebView::SetDrawCallback(const DrawCallback& callback) {
  client_->SetDrawCallback(callback);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::Resize(int width, int height) const {
  client_->Resize(width, height);
  browser_->Get()->GetHost()->WasResized();
}

////////////////////////////////////////////////////////////////////////////////

void WebView::Reload(bool ignoreCache) const {
  if (ignoreCache) browser_->Get()->ReloadIgnoreCache();
  else             browser_->Get()->Reload();
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectMouseMove(int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  browser_->Get()->GetHost()->SendMouseMoveEvent(event, false);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectMouseWheel(int direction, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  browser_->Get()->GetHost()->SendMouseWheelEvent(event, 0, direction);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectButtonDown(int button, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  browser_->Get()->GetHost()->SendMouseClickEvent(event, (cef_mouse_button_type_t)button, false, 1);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectButtonUp(int button, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  browser_->Get()->GetHost()->SendMouseClickEvent(event, (cef_mouse_button_type_t)button, true, 1);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectKeyDown(unsigned char key) const {
  CefKeyEvent event;
  event.character = key;
  event.type = KEYEVENT_CHAR;

  browser_->Get()->GetHost()->SendKeyEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectKeyUp(unsigned char key) const {
  CefKeyEvent event;
  event.character = key;
  event.type = KEYEVENT_KEYUP;

  browser_->Get()->GetHost()->SendKeyEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::call_javascript_impl(std::string const& function, std::vector<std::string> const& args) const {
  std::string call(function + "( ");
  for (auto& s: args) {
    call += s + ",";
  }
  call.back() = ')';

  CefRefPtr<CefFrame> frame = browser_->Get()->GetMainFrame();
  frame->ExecuteJavaScript(call, frame->GetURL(), 0);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::register_js_callback_impl(std::string const& name, std::function<void(std::vector<Any> const&)> callback) {
  client_->register_js_callback(name, callback);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::ToggleDevTools() {
  if (devToolsOpen_) CloseDevTools();
  else               ShowDevTools();
}

////////////////////////////////////////////////////////////////////////////////

void WebView::ShowDevTools() {
  CefWindowInfo windowInfo;
  CefRefPtr<CefClient> client;
  CefBrowserSettings settings;

  // MainContext::Get()->GetRootWindowManager()->CreateRootWindowAsPopup(
  //     !is_devtools, is_osr(), popupFeatures, windowInfo, client, settings);

  browser_->Get()->GetHost()->ShowDevTools(windowInfo, browser_->Get()->GetHost()->GetClient(), settings, CefPoint());
  devToolsOpen_ = true;
}

////////////////////////////////////////////////////////////////////////////////

void WebView::CloseDevTools() {
  browser_->Get()->GetHost()->CloseDevTools();
  devToolsOpen_ = false;
}

////////////////////////////////////////////////////////////////////////////////

}
