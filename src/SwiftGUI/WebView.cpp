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

#include "Logger.hpp"
#include "detail/WebViewClient.hpp"

namespace swift {

class DevToolsClient : public CefClient
{
public:
  IMPLEMENT_REFCOUNTING(DevToolsClient);
};


////////////////////////////////////////////////////////////////////////////////

WebView::WebView(const std::string& url, int width, int height)
: client_(new detail::WebViewClient(width, height))
, devToolsOpen_(false) {

  CefWindowInfo info;
  info.width = width;
  info.height = height;

  CefBrowserSettings browserSettings;
  std::size_t windowHandle = 0;
  info.SetAsWindowless(windowHandle, true);
  browserSettings.windowless_frame_rate = 60;

  CefBrowserHost::CreateBrowserSync(info, client_, url, browserSettings, nullptr);
}

////////////////////////////////////////////////////////////////////////////////

WebView::~WebView() {
  delete client_;
}

////////////////////////////////////////////////////////////////////////////////

void WebView::SetDrawCallback(DrawCallback const& callback) {
  client_->SetDrawCallback(callback);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::Resize(int width, int height) const {
  client_->Resize(width, height);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::Reload(bool ignoreCache) const {
  if (ignoreCache) client_->GetBrowser()->ReloadIgnoreCache();
  else             client_->GetBrowser()->Reload();
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectMouseMove(int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  client_->GetBrowser()->GetHost()->SendMouseMoveEvent(event, false);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectMouseWheel(int direction, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  client_->GetBrowser()->GetHost()->SendMouseWheelEvent(event, 0, direction);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectButtonDown(int button, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  client_->GetBrowser()->GetHost()->SendMouseClickEvent(event, (cef_mouse_button_type_t)button, false, 1);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectButtonUp(int button, int x, int y) const {
  CefMouseEvent event;
  event.modifiers = 0;
  event.x = x;
  event.y = y;

  client_->GetBrowser()->GetHost()->SendMouseClickEvent(event, (cef_mouse_button_type_t)button, true, 1);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectKeyDown(unsigned int key) const {
  CefKeyEvent event;
  event.native_key_code = key;
  event.unmodified_character = key;
  event.type = KEYEVENT_KEYDOWN;

  client_->GetBrowser()->GetHost()->SendKeyEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectKeyUp(unsigned int key) const {
  CefKeyEvent event;
  event.native_key_code = key;
  event.unmodified_character = key;
  event.type = KEYEVENT_KEYUP;

  client_->GetBrowser()->GetHost()->SendKeyEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectChar(unsigned int key) const {
  CefKeyEvent event;
  event.character = key;
  event.type = KEYEVENT_CHAR;

  client_->GetBrowser()->GetHost()->SendKeyEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::CallJavascriptImpl(std::string const& function, std::vector<std::string> const& args) const {
  std::string call(function + "( ");
  for (auto& s: args) {
    call += s + ",";
  }
  call.back() = ')';

  CefRefPtr<CefFrame> frame = client_->GetBrowser()->GetMainFrame();
  frame->ExecuteJavaScript(call, frame->GetURL(), 0);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::RegisterJSCallbackImpl(std::string const& name, std::function<void(std::vector<Any> const&)> callback) {
  client_->RegisterJSCallback(name, callback);
}

////////////////////////////////////////////////////////////////////////////////

void WebView::ToggleDevTools() {
  if (devToolsOpen_) CloseDevTools();
  else               ShowDevTools();
}

////////////////////////////////////////////////////////////////////////////////

void WebView::ShowDevTools() {
  CefWindowInfo windowInfo;
  windowInfo.height = 500;
  windowInfo.width = 500;
  windowInfo.parent_window = 0;

  CefRefPtr<CefClient> client;
  CefBrowserSettings settings;

  // MainContext::Get()->GetRootWindowManager()->CreateRootWindowAsPopup(
  //     !is_devtools, is_osr(), popupFeatures, windowInfo, client, settings);

  client_->GetBrowser()->GetHost()->ShowDevTools(windowInfo, new DevToolsClient(), settings, CefPoint());
  devToolsOpen_ = true;
}

////////////////////////////////////////////////////////////////////////////////

void WebView::CloseDevTools() {
  client_->GetBrowser()->GetHost()->CloseDevTools();
  devToolsOpen_ = false;
}

////////////////////////////////////////////////////////////////////////////////

}
