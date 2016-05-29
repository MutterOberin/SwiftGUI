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
, devToolsOpen_(false)
, mouse_x_(0)
, mouse_y_(0)
, mouse_modifiers_(0) {

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

void WebView::InjectMouseEvent(MouseEvent const& event) {

  CefMouseEvent cef_event;
  cef_event.modifiers = mouse_modifiers_;
  cef_event.x = mouse_x_;
  cef_event.y = mouse_y_;

  switch(event.type_) {
    case MouseEvent::Type::MOVE:
      cef_event.x = mouse_x_ = event.x_;
      cef_event.y = mouse_y_ = event.y_;
      client_->GetBrowser()->GetHost()->SendMouseMoveEvent(cef_event, false);
      break;

    case MouseEvent::Type::SCROLL:
      client_->GetBrowser()->GetHost()->SendMouseWheelEvent(cef_event, event.x_, event.y_);
      break;

    case MouseEvent::Type::PRESS:
      if      (event.button_ == Button::LEFT)   mouse_modifiers_ |= (int)Modifier::LEFT_BUTTON;
      else if (event.button_ == Button::RIGHT)  mouse_modifiers_ |= (int)Modifier::RIGHT_BUTTON;
      else if (event.button_ == Button::MIDDLE) mouse_modifiers_ |= (int)Modifier::MIDDLE_BUTTON;
      cef_event.modifiers = mouse_modifiers_;
      client_->GetBrowser()->GetHost()->SendMouseClickEvent(cef_event, (cef_mouse_button_type_t)event.button_, false, 1);
      break;

    case MouseEvent::Type::RELEASE:
      if      (event.button_ == Button::LEFT)   mouse_modifiers_ &= ~(int)Modifier::LEFT_BUTTON;
      else if (event.button_ == Button::RIGHT)  mouse_modifiers_ &= ~(int)Modifier::RIGHT_BUTTON;
      else if (event.button_ == Button::MIDDLE) mouse_modifiers_ &= ~(int)Modifier::MIDDLE_BUTTON;
      cef_event.modifiers = mouse_modifiers_;
      client_->GetBrowser()->GetHost()->SendMouseClickEvent(cef_event, (cef_mouse_button_type_t)event.button_, true, 1);
      break;

  }

}

////////////////////////////////////////////////////////////////////////////////

void WebView::InjectKeyEvent(KeyEvent const& event) {
  CefKeyEvent cef_event;
  cef_event.modifiers               = event.modifiers_;
  // cef_event.native_key_code         = event.native_code_;
  // cef_event.unmodified_character    = event.character_;
  cef_event.character               = event.character_;
  cef_event.is_system_key           = false;
  cef_event.windows_key_code        = (int)event.key_;
  cef_event.focus_on_editable_field = true;

  if      (event.type_ == KeyEvent::Type::PRESS)   cef_event.type = KEYEVENT_KEYDOWN;
  else if (event.type_ == KeyEvent::Type::RELEASE) cef_event.type = KEYEVENT_KEYUP;
  else                                             cef_event.type = KEYEVENT_CHAR;

  client_->GetBrowser()->GetHost()->SendKeyEvent(cef_event);
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
