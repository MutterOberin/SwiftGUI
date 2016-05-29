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
#include <iostream>
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "utils.hpp"
#include "Any.hpp"

namespace swift {

class WebView {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  WebView(const std::string& url, int width, int height);
  virtual ~WebView();

  void SetDrawCallback(DrawCallback const& callback);

  template<typename ...Args>
  void CallJavascript(std::string const& function, Args&& ... a) const {
    std::vector<std::string> args = {(utils::to_string(a))...};
    CallJavascriptImpl(function, args);
  }

  void RegisterCallback(std::string const& name, std::function<void()> callback) {
    RegisterJSCallbackImpl(name, [this, callback](std::vector<Any> const& args) {
      callback();
    });
  }

  template<typename A>
  void RegisterCallback(std::string const& name, std::function<void(A)> callback) {
    RegisterJSCallbackImpl(name, [this, callback](std::vector<Any> const& args){
      callback(args[0].cast<A>());
    });
  }

  template<typename A, typename B>
  void RegisterCallback(std::string const& name, std::function<void(A, B)> callback) {
    RegisterJSCallbackImpl(name, [this, callback](std::vector<Any> const& args){
      callback(args[0].cast<A>(), args[1].cast<B>());
    });
  }

  template<typename A, typename B, typename C>
  void RegisterCallback(std::string const& name, std::function<void(A, B, C)> callback) {
    RegisterJSCallbackImpl(name, [this, callback](std::vector<Any> const& args){
      callback(args[0].cast<A>(), args[1].cast<B>(), args[2].cast<C>());
    });
  }

  template<typename A, typename B, typename C, typename D>
  void RegisterCallback(std::string const& name, std::function<void(A, B, C, D)> callback) {
    RegisterJSCallbackImpl(name, [this, callback](std::vector<Any> const& args){
      callback(args[0].cast<A>(), args[1].cast<B>(), args[2].cast<C>(), args[3].cast<D>());
    });
  }


  void Resize(int width, int height) const;
  void Reload(bool ignoreCache = false) const;

  void InjectMouseEvent(MouseEvent const& event);
  void InjectKeyEvent(KeyEvent const& event);

  void ToggleDevTools();
  void ShowDevTools();
  void CloseDevTools();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void CallJavascriptImpl(std::string const& function, std::vector<std::string> const& args) const;

  void RegisterJSCallbackImpl(std::string const& name, std::function<void(std::vector<Any> const&)> callback);

  detail::WebViewClient* client_;

  bool devToolsOpen_;

  // input state
  int mouse_x_;
  int mouse_y_;
  int mouse_modifiers_;
};

}

#endif // SWIFT_WEBVIEW_HPP
