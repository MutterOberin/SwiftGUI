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
#include "JSHandler.hpp"

#include <sstream>

namespace swift {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

bool JSHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& exception) {

  if (name != "call_native") {
    SendError("Unknown Javascript function name!");
    return false;
  }

  if (arguments.size() == 0) {
    SendError("window.call_native function requires at least one argument!");
    return false;
  }

  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(name);
  msg->GetArgumentList()->SetString(0, arguments[0]->GetStringValue());

  for (int i(1); i<arguments.size(); ++i) {
    if (arguments[i]->IsDouble()) {
      msg->GetArgumentList()->SetDouble(i, arguments[i]->GetDoubleValue());
    } else if (arguments[i]->IsBool()) {
      msg->GetArgumentList()->SetBool(i, arguments[i]->GetBoolValue());
    } else if (arguments[i]->IsString()) {
      msg->GetArgumentList()->SetString(i, arguments[i]->GetStringValue());
    } else {
      std::stringstream sstr;
      sstr << "Failed to handle window.call_native call. Argument " << i
           << " has an unsupported type. Only Double, Bool and String are "
           << "supported.";
      SendError(sstr.str());
    }
  }

  browser_->SendProcessMessage(PID_BROWSER, msg);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void JSHandler::SendError(std::string const& message) const {
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("error");
  msg->GetArgumentList()->SetString(0, message);
  browser_->SendProcessMessage(PID_BROWSER, msg);
}

////////////////////////////////////////////////////////////////////////////////

}
}

