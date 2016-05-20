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

#ifndef SWIFT_DETAIL_JS_HANDLER_HPP
#define SWIFT_DETAIL_JS_HANDLER_HPP

// includes  -------------------------------------------------------------------
#include <include/cef_v8.h>

namespace swift {
namespace detail {

class JSHandler : public CefV8Handler {

 public:

  JSHandler(CefRefPtr<CefBrowser> browser)
  : browser_(browser) {}

  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) override {

    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(name);

    msg->GetArgumentList()->SetString(0, arguments[0]->GetStringValue());

    for (int i(1); i<arguments.size(); ++i) {
      if (arguments[i]->IsDouble()) {
        msg->GetArgumentList()->SetDouble(i, arguments[i]->GetDoubleValue());
      } else if (arguments[i]->IsInt()) {
        msg->GetArgumentList()->SetInt(i, arguments[i]->GetIntValue());
      } else if (arguments[i]->IsBool()) {
        msg->GetArgumentList()->SetBool(i, arguments[i]->GetBoolValue());
      } else if (arguments[i]->IsString()) {
        msg->GetArgumentList()->SetString(i, arguments[i]->GetStringValue());
      } else {
        // TODO
      }
    }
    // args->SetInt(0, 10);

    browser_->SendProcessMessage(PID_BROWSER, msg);

    return true;
  }

 private:
  IMPLEMENT_REFCOUNTING(JSHandler);

  CefRefPtr<CefBrowser> browser_;
};

}
}

#endif // SWIFT_DETAIL_JS_HANDLER_HPP
